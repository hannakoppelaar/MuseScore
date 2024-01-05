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
#include "dom/pitchspelling.h"
#include "log.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace mu::engraving::tuning;


void TuningConfiguration::init(const mu::io::path_t& tuningFile) {

    if (m_accidentalNameMap.size() < (int)AccidentalType::END) {
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

    // TODO add validation

    // TODO implement non-octave and non-periodic tunings

    QJsonObject rootObj = QJsonDocument::fromJson(json).object();
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
    double period = firstNominalMapJson.value("NONE").toDouble();
    for (int i = 0; i < mapJson.toArray().size(); i++) {
        QJsonObject accidentalsJson = mapJson.toArray().at(i).toObject();
        QJsonValue scalaValueJson = accidentalsJson.value("NONE");
        double nominalScalaValue;
        try {
            nominalScalaValue = parseScalaValue(scalaValueJson);
        } catch (std::invalid_argument e) {
            LOGE() << "Bad scala value in tuning file: " << scalaValueJson;
            return;
        }
        double nominalOffset = nominalScalaValue - getNominal(i) * 100;
        if (i == 0) {
            nominalOffset = 0;
            nominalScalaValue = 0;
        }
        std::map<int, double> accidentalMap;
        m_offsetMap[getNominal(i)] = accidentalMap;
        foreach(QString accidentalName, accidentalsJson.keys()) {
            if (accidentalName == "NONE") {
                m_offsetMap.at(getNominal(i))[m_accidentalNameMap.at("NONE")] = nominalOffset;
            } else {
                double accidentalOffset;
                QJsonValue scalaValueJson = accidentalsJson.value(accidentalName);
                try {
                    accidentalOffset = parseScalaValue(scalaValueJson) - nominalScalaValue;
                } catch (std::invalid_argument e) {
                    LOGE() << "Bad scala value in tuning file: " << scalaValueJson;
                    return;
                }
                m_offsetMap.at(getNominal(i))[m_accidentalNameMap.at(accidentalName.toStdString())] = accidentalOffset;
            }
        }
    }


    for (const auto& [nominal, nominalMap] : m_offsetMap) {
        LOGE() << "Nominal: " << nominal;
        for (const auto& [accidentalName, offset] : nominalMap) {
            LOGE() << "Accidental name: " << accidentalName << ", offset: " << offset;
        }
    }


    m_valid = true;

}

/**
 * Convert array indices from the "map" array (in the JSON file) to nominal values (i.e. 0 for C, 2 for D...)
 */
int TuningConfiguration::getNominal(int index) {
    switch (index) {
        case 0:
            return 0;
        case 1:
            return 2;
        case 2:
            return 4;
        case 3:
            return 5;
        case 4:
            return 7;
        case 5:
            return 9;
        case 6:
            return 11;
    }
}

double TuningConfiguration::parseScalaValue(QJsonValue scalaValueJson) {
    if (scalaValueJson.isDouble()) {
        return scalaValueJson.toDouble();
    } else if (!scalaValueJson.isString()) {
        throw std::invalid_argument("Expected string or double");
    }

    std::string scalaValue = scalaValueJson.toString().toStdString();

    // Values containing a "." are interpreted as decimal numbers (Scala spec),
    // all other numbers are integers in simple mathematical expressions (e.g. "2" or "5/4")
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

double TuningConfiguration::getOffset(int pitch, int tpc, int accidentalType) const {

    // determine nominal
    int nominal = pitch;
    int tpcAccidental = 0;
    if (tpc > 5 && tpc < 13) {
        tpcAccidental = (int)AccidentalType::FLAT;
        nominal += 1;
    } else if (tpc > -2 && tpc < 6) {
        tpcAccidental = (int)AccidentalType::FLAT2;
        nominal += 2;
    } else if (tpc > 19 && tpc < 34) {
        tpcAccidental = (int)AccidentalType::SHARP;
        nominal -= 1;
    } else if (tpc > 33 && tpc < 41) {
        tpcAccidental = (int)AccidentalType::SHARP2;
        nominal -= 2;
    }

    // initial offset: the difference in pitch between the original note number and the note number of the nominal
    double offset = (nominal - pitch) * 100;

    // retune the nominal
    int relativeNominal = nominal % 12;
    if (m_offsetMap.find(relativeNominal) == m_offsetMap.end()) {
        LOGI() << "Nominal " << nominal << " has not been mapped in this tuning";
        return 0;
    }
    std::map<int, double> accidentalMap = m_offsetMap.at(relativeNominal);
    offset += accidentalMap.at((int)AccidentalType::NONE);

    // process accidental (including any implied by the key signature)
    int actualAccidentalType = accidentalType;
    if (accidentalType != (int)AccidentalType::NATURAL && accidentalType != (int)AccidentalType::NONE) {
        if (tpcAccidental) {
            // Got key sig
            actualAccidentalType = tpcAccidental;
        }
    }
    if (accidentalMap.find(actualAccidentalType) == accidentalMap.end()) {
        LOGE() << "Accidental type " << actualAccidentalType << " has not been mapped for nominal " <<
                                                                        nominal << " in this tuning";
    } else {
        offset += accidentalMap.at(actualAccidentalType);
    }
    return offset;

}
