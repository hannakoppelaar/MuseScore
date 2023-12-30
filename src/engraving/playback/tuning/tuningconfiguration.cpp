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

    QFile *file = new QFile(tuningFile.toQString());
    if (!file->exists()) {
        LOGE() << "Tuning file does not exist: " << tuningFile.toStdString().c_str();
        return;
    }

    QByteArray json;
    if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        json = file->readAll();
    } else {
        LOGE() << "Error opening file " << file->fileName();
        return;
    }

    // TODO add validation

    // TODO implement non-octave and non-periodic tunings

    QJsonObject rootObj = QJsonDocument::fromJson(json).object();
    QJsonArray offsets = rootObj.value("offsets").toArray();
    for (int i = 0; i < offsets.size(); i++) {
        QJsonObject offset = offsets.at(i).toObject();
        QString nominal = offset.keys().at(0);
        QJsonArray list = offset.value(nominal).toArray();
        std::map<int, double> accidentalMap;
        m_offsetMap[nominal.toInt()] = accidentalMap;
        for (int j = 0; j < list.size(); j++) {
            QJsonObject pair = list.at(j).toObject();
            QString accidental = pair.keys().at(0);
            double accidentalOffset = pair.value(accidental).toDouble();
            m_offsetMap.at(nominal.toInt())[m_accidentalNameMap.at(accidental.toStdString())] = accidentalOffset;
        }
    }

    m_valid = true;

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
