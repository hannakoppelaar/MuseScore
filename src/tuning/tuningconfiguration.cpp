/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2023 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tuningconfiguration.h"
#include "dom/types.h"
#include "log.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace mu::tuning;


void TuningConfiguration::init(const mu::io::path_t& tuningFile)
{

    if (m_accidentalNameMap.size() < (int)engraving::AccidentalType::END) {
        LOGI() << "New accidentals are available in MuseScore, please update tuningconfiguration.h";
    }

    QFile *file = new QFile(tuningFile.toQString());
    if (!file->exists()) {
        LOGE() << "Tuning file does not exist: " << tuningFile.toStdString().c_str();
        return;
    }

    QByteArray json;
    if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        json = file->readAll();
        file->close();
    } else {
        LOGE() << "Error opening file " << file->fileName();
        return;
    }

    QJsonObject rootObj = QJsonDocument::fromJson(json).object();
    QJsonValue name = rootObj.value("name");
    if (!name.isNull() && name.isString()) {
        m_name = name.toString().toStdString();
    }

    QJsonValue mapJson = rootObj.value("map");
    if (mapJson.isNull() || !mapJson.isArray() || mapJson.toArray().empty()) {
        LOGE() << "Invalid tuning file " << tuningFile.toStdString();
        return;
    }
    QJsonObject firstNominalMapJson = mapJson.toArray().at(0).toObject();
    if (firstNominalMapJson.value("NONE").isNull()) {
        LOGE() << "Invalid tuning file " << tuningFile.toStdString();
        return;
    }

    // First parse the tuning file and create a map holding the scala values for one period
    int numNominals = mapJson.toArray().size();
    double period;
    try {
        period = parseScalaValue(firstNominalMapJson.value("NONE"));
    } catch (...) {
        LOGE() << "Bad scala value in tuning file: " << firstNominalMapJson.value("NONE");
        return;
    }
    int midiRoot = 60;
    int tuningRoot = 0;
    QJsonValue rootJson = rootObj.value("root");
    if (!rootJson.isNull() && rootJson.isArray()) {
        if (rootJson.toArray().size() != 2) {
            LOGE() << "Bad value for 'root' in tuning file: expected array of size 2";
            return;
        }
        tuningRoot = rootJson.toArray().at(0).toInt();
        midiRoot = rootJson.toArray().at(1).toInt();
        int relativeRoot = midiRoot % 12;
        if (relativeRoot == 1 || relativeRoot == 3 || relativeRoot == 6 || relativeRoot == 8 || relativeRoot == 10) {
            LOGE() << "Bad value for 'root' in tuning file: " << midiRoot << " is not a nominal";
            return;
        }
        if (tuningRoot > numNominals) {
            LOGE() << "Bad value for 'root' in tuning file: " << tuningRoot << " is larger than the number of nominals";
            return;
        }
    }
    std::map<int, std::map<int, double>> scalaMap;
    for (int i = 0; i < numNominals; i++) {
        QJsonObject accidentalsJson = mapJson.toArray().at(i).toObject();
        std::map<int, double> accidentalMap;
        scalaMap[getNominal(i)] = accidentalMap;
        foreach(QString accidentalName, accidentalsJson.keys()) {
            QJsonValue scalaValueJson = accidentalsJson.value(accidentalName);
            double accidentalValue;
            try {
                accidentalValue = parseScalaValue(scalaValueJson);
            } catch (...) {
                LOGE() << "Bad scala value in tuning file: " << scalaValueJson;
                return;
            }
            if (m_accidentalNameMap.find(accidentalName.toStdString()) == m_accidentalNameMap.end()) {
                LOGE() << "Unknown accidental type in tuning file: " << accidentalName.toStdString();
                return;
            }
            scalaMap.at(getNominal(i))[m_accidentalNameMap.at(accidentalName.toStdString())] = accidentalValue;
        }
    }


    // Now create the map with offsets for all 75 nominals

    // Modulo bookkeeping to make sure tuningRoot and midiRoot line up
    int numNominalsBelowRoot = round(midiRoot * 7/12.0);
    int firstNominalIndex = (tuningRoot - (numNominalsBelowRoot % numNominals) + numNominals) % numNominals;

    double scalaValueAtRoot = scalaMap.at(getNominal(tuningRoot)).at(m_accidentalNameMap.at("NONE"));
    if (tuningRoot == 0) {
        scalaValueAtRoot = 0;
    }
    // This offset assures that the offset value for the nominal at midiRoot is zero
    double rootOffset = midiRoot * 100 - (period * floor(numNominalsBelowRoot / float(numNominals)) + scalaValueAtRoot);

    bool initialPeriod = (firstNominalIndex == 0) ? true : false;
    double periodOffset = 0;
    for (int i = 0; i < 75; i++) {
        int nominalIndex = (firstNominalIndex + i) % numNominals;
        int nominal = getNominal(nominalIndex);
        int midiNominal = getNominal(i);
        double nominalScalaValue = nominal * 100; // if this nominal is not part of the tuning, use the 12-EDO value
        if (scalaMap.at(nominal).find(m_accidentalNameMap.at("NONE")) != scalaMap.at(nominal).end()) {
            nominalScalaValue = scalaMap.at(nominal).at(m_accidentalNameMap.at("NONE"));
        }
        if (nominalIndex == 0) {
            nominalScalaValue = 0;
            if (initialPeriod) {
                initialPeriod = false;
            } else {
                periodOffset += period;
            }
        }
        double nominalOffset = (periodOffset + nominalScalaValue - midiNominal * 100) + rootOffset;
        std::map<int, double> accidentalMap;
        m_offsetMap[midiNominal] = accidentalMap;
        m_offsetMap.at(midiNominal)[m_accidentalNameMap.at("NONE")] = nominalOffset;
        foreach(std::pair accidental, scalaMap.at(nominal)) {
            if (accidental.first != m_accidentalNameMap.at("NONE")) {
                m_offsetMap.at(midiNominal)[accidental.first] = accidental.second - nominalScalaValue;
            }
        }
    }

    m_valid = true;
}

/**
 * Convert array indices from the "map" array (in the JSON file) to nominal values (i.e. 0 for C, 2 for D...)
 */
int TuningConfiguration::getNominal(int index)
{

    int nominal = 0;
    for (int i = 0; i < index; i++) {
        nominal += m_standardNominalSteps.at(i % 7);
    }
    return nominal;
}

double TuningConfiguration::parseScalaValue(QJsonValue scalaValueJson)
{
    if (scalaValueJson.isDouble()) {
        return scalaValueJson.toDouble();
    } else if (!scalaValueJson.isString()) {
        throw "Expected string or double";
    }

    std::string scalaValue = scalaValueJson.toString().toStdString();

    // As per the Scala spec, values containing a "." are interpreted as cents values expressed using decimal numbers,
    // all other values are assumed to represent relative frequencies (e.g. "2" or "5/4")
    if (scalaValue.find(".") != std::string::npos) {
        return std::stod(scalaValue);
    } else {
        size_t pos = scalaValue.find("/");
        if (pos != std::string::npos) {
            std::string numeratorStr = scalaValue.substr(0, pos);
            std::string denominatorStr = scalaValue.substr(pos + 1);
            long numerator = std::stol(numeratorStr);
            long demoninator = std::stol(denominatorStr);
            return 1200 * std::log(numerator / (double)demoninator) / std::log(2);
        } else {
            return 1200 * std::log(std::stod(scalaValue)) / std::log(2);
        }
    }
}

double TuningConfiguration::getOffset(int pitch, int tpc, int accidentalType) const
{

    if (!m_valid) {
        return 0;
    }

    // determine nominal
    int nominal = pitch;
    int tpcAccidental = 0;
    if (tpc > 5 && tpc < 13) {
        tpcAccidental = m_accidentalNameMap.at("FLAT");
        nominal += 1;
    } else if (tpc > -2 && tpc < 6) {
        tpcAccidental = m_accidentalNameMap.at("FLAT2");
        nominal += 2;
    } else if (tpc > 19 && tpc < 34) {
        tpcAccidental = m_accidentalNameMap.at("SHARP");
        nominal -= 1;
    } else if (tpc > 33 && tpc < 41) {
        tpcAccidental = m_accidentalNameMap.at("SHARP2");
        nominal -= 2;
    }

    // initial offset: the difference in pitch between the original note number and the note number of its nominal
    double offset = (nominal - pitch) * 100;

    // retune the nominal
    offset += m_offsetMap.at(nominal).at(m_accidentalNameMap.at("NONE"));

    // process accidental (including any implied by the key signature)
    std::map<int, double> accidentalMap = m_offsetMap.at(nominal);
    int actualAccidentalType = accidentalType;
    if (accidentalType == m_accidentalNameMap.at("NONE")) {
        if (tpcAccidental) {
            // Got key sig
            actualAccidentalType = tpcAccidental;
        }
    }
    if (actualAccidentalType == m_accidentalNameMap.at("NATURAL") || actualAccidentalType == m_accidentalNameMap.at("NONE")) {
        return offset;
    }
    if (accidentalMap.find(actualAccidentalType) == accidentalMap.end()) {
        LOGE() << "Accidental type " << actualAccidentalType << " has not been mapped for nominal " <<
                                                                        nominal << " in this tuning";
    } else {
        offset += accidentalMap.at(actualAccidentalType);
    }
    return offset;
}
