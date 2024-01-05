
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
#ifndef MU_ENGRAVING_TUNING_TUNINGCONFIGURATION_H
#define MU_ENGRAVING_TUNING_TUNINGCONFIGURATION_H

#include "ituningconfiguration.h"
#include "io/path.h"
#include "dom/types.h"
#include <QJsonValue>

namespace mu::engraving::tuning {
class TuningConfiguration : public ITuningConfiguration
{

public:
    void init(const io::path_t& tuningFile) override;
    bool valid() const override { return m_valid; };
    std::string getName() const override { return "31-EDO"; };
    double getOffset (int pitch, int tpc, int accidentalType) const override;
private:
    double parseScalaValue(QJsonValue scalaValueJson);
    int getNominal(int index);

    bool m_valid = false;
    std::map<int, double> m_nominalOffsetMap;
    std::map<int, double> m_accidentalOffsetMap;

    // maps nominals to a map of accidentals to offsets
    std::map<int, std::map<int, double>> m_offsetMap;

    // Map accidental identifiers from the JSON format to accidentals.
    // There's no way to get the enum symbol as a string in C++17, so
    // we'll need to keep this in sync with types.h manually
    std::map<std::string, int> m_accidentalNameMap {
        {"NONE", (int)AccidentalType::NONE},
        {"FLAT", (int)AccidentalType::FLAT},
        {"NATURAL", (int)AccidentalType::NATURAL},
        {"SHARP", (int)AccidentalType::SHARP},
        {"SHARP2", (int)AccidentalType::SHARP2},
        {"FLAT2", (int)AccidentalType::FLAT2},
        {"SHARP3", (int)AccidentalType::SHARP3},
        {"FLAT3", (int)AccidentalType::FLAT3},
        {"NATURAL_FLAT", (int)AccidentalType::NATURAL_FLAT},
        {"NATURAL_SHARP", (int)AccidentalType::NATURAL_SHARP},
        {"SHARP_SHARP", (int)AccidentalType::SHARP_SHARP},
        {"FLAT_ARROW_UP", (int)AccidentalType::FLAT_ARROW_UP},
        {"FLAT_ARROW_DOWN", (int)AccidentalType::FLAT_ARROW_DOWN},
        {"NATURAL_ARROW_UP", (int)AccidentalType::NATURAL_ARROW_UP},
        {"NATURAL_ARROW_DOWN", (int)AccidentalType::NATURAL_ARROW_DOWN},
        {"SHARP_ARROW_UP", (int)AccidentalType::SHARP_ARROW_UP},
        {"SHARP_ARROW_DOWN", (int)AccidentalType::SHARP_ARROW_DOWN},
        {"SHARP2_ARROW_UP", (int)AccidentalType::SHARP2_ARROW_UP},
        {"SHARP2_ARROW_DOWN", (int)AccidentalType::SHARP2_ARROW_DOWN},
        {"FLAT2_ARROW_UP", (int)AccidentalType::FLAT2_ARROW_UP},
        {"FLAT2_ARROW_DOWN", (int)AccidentalType::FLAT2_ARROW_DOWN},
        {"ARROW_DOWN", (int)AccidentalType::ARROW_DOWN},
        {"ARROW_UP", (int)AccidentalType::ARROW_UP},
        {"MIRRORED_FLAT", (int)AccidentalType::MIRRORED_FLAT},
        {"MIRRORED_FLAT2", (int)AccidentalType::MIRRORED_FLAT2},
        {"SHARP_SLASH", (int)AccidentalType::SHARP_SLASH},
        {"SHARP_SLASH4", (int)AccidentalType::SHARP_SLASH4},
        {"FLAT_SLASH2", (int)AccidentalType::FLAT_SLASH2},
        {"FLAT_SLASH", (int)AccidentalType::FLAT_SLASH},
        {"SHARP_SLASH3", (int)AccidentalType::SHARP_SLASH3},
        {"SHARP_SLASH2", (int)AccidentalType::SHARP_SLASH2},
        {"DOUBLE_FLAT_ONE_ARROW_DOWN", (int)AccidentalType::DOUBLE_FLAT_ONE_ARROW_DOWN},
        {"FLAT_ONE_ARROW_DOWN", (int)AccidentalType::FLAT_ONE_ARROW_DOWN},
        {"NATURAL_ONE_ARROW_DOWN", (int)AccidentalType::NATURAL_ONE_ARROW_DOWN},
        {"SHARP_ONE_ARROW_DOWN", (int)AccidentalType::SHARP_ONE_ARROW_DOWN},
        {"DOUBLE_SHARP_ONE_ARROW_DOWN", (int)AccidentalType::DOUBLE_SHARP_ONE_ARROW_DOWN},
        {"DOUBLE_FLAT_ONE_ARROW_UP", (int)AccidentalType::DOUBLE_FLAT_ONE_ARROW_UP},
        {"FLAT_ONE_ARROW_UP", (int)AccidentalType::FLAT_ONE_ARROW_UP},
        {"NATURAL_ONE_ARROW_UP", (int)AccidentalType::NATURAL_ONE_ARROW_UP},
        {"SHARP_ONE_ARROW_UP", (int)AccidentalType::SHARP_ONE_ARROW_UP},
        {"DOUBLE_SHARP_ONE_ARROW_UP", (int)AccidentalType::DOUBLE_SHARP_ONE_ARROW_UP},
        {"DOUBLE_FLAT_TWO_ARROWS_DOWN", (int)AccidentalType::DOUBLE_FLAT_TWO_ARROWS_DOWN},
        {"FLAT_TWO_ARROWS_DOWN", (int)AccidentalType::FLAT_TWO_ARROWS_DOWN},
        {"NATURAL_TWO_ARROWS_DOWN", (int)AccidentalType::NATURAL_TWO_ARROWS_DOWN},
        {"SHARP_TWO_ARROWS_DOWN", (int)AccidentalType::SHARP_TWO_ARROWS_DOWN},
        {"DOUBLE_SHARP_TWO_ARROWS_DOWN", (int)AccidentalType::DOUBLE_SHARP_TWO_ARROWS_DOWN},
        {"DOUBLE_FLAT_TWO_ARROWS_UP", (int)AccidentalType::DOUBLE_FLAT_TWO_ARROWS_UP},
        {"FLAT_TWO_ARROWS_UP", (int)AccidentalType::FLAT_TWO_ARROWS_UP},
        {"NATURAL_TWO_ARROWS_UP", (int)AccidentalType::NATURAL_TWO_ARROWS_UP},
        {"SHARP_TWO_ARROWS_UP", (int)AccidentalType::SHARP_TWO_ARROWS_UP},
        {"DOUBLE_SHARP_TWO_ARROWS_UP", (int)AccidentalType::DOUBLE_SHARP_TWO_ARROWS_UP},
        {"DOUBLE_FLAT_THREE_ARROWS_DOWN", (int)AccidentalType::DOUBLE_FLAT_THREE_ARROWS_DOWN},
        {"FLAT_THREE_ARROWS_DOWN", (int)AccidentalType::FLAT_THREE_ARROWS_DOWN},
        {"NATURAL_THREE_ARROWS_DOWN", (int)AccidentalType::NATURAL_THREE_ARROWS_DOWN},
        {"SHARP_THREE_ARROWS_DOWN", (int)AccidentalType::SHARP_THREE_ARROWS_DOWN},
        {"DOUBLE_SHARP_THREE_ARROWS_DOWN", (int)AccidentalType::DOUBLE_SHARP_THREE_ARROWS_DOWN},
        {"DOUBLE_FLAT_THREE_ARROWS_UP", (int)AccidentalType::DOUBLE_FLAT_THREE_ARROWS_UP},
        {"FLAT_THREE_ARROWS_UP", (int)AccidentalType::FLAT_THREE_ARROWS_UP},
        {"NATURAL_THREE_ARROWS_UP", (int)AccidentalType::NATURAL_THREE_ARROWS_UP},
        {"SHARP_THREE_ARROWS_UP", (int)AccidentalType::SHARP_THREE_ARROWS_UP},
        {"DOUBLE_SHARP_THREE_ARROWS_UP", (int)AccidentalType::DOUBLE_SHARP_THREE_ARROWS_UP},
        {"LOWER_ONE_SEPTIMAL_COMMA", (int)AccidentalType::LOWER_ONE_SEPTIMAL_COMMA},
        {"RAISE_ONE_SEPTIMAL_COMMA", (int)AccidentalType::RAISE_ONE_SEPTIMAL_COMMA},
        {"LOWER_TWO_SEPTIMAL_COMMAS", (int)AccidentalType::LOWER_TWO_SEPTIMAL_COMMAS},
        {"RAISE_TWO_SEPTIMAL_COMMAS", (int)AccidentalType::RAISE_TWO_SEPTIMAL_COMMAS},
        {"LOWER_ONE_UNDECIMAL_QUARTERTONE", (int)AccidentalType::LOWER_ONE_UNDECIMAL_QUARTERTONE},
        {"RAISE_ONE_UNDECIMAL_QUARTERTONE", (int)AccidentalType::RAISE_ONE_UNDECIMAL_QUARTERTONE},
        {"LOWER_ONE_TRIDECIMAL_QUARTERTONE", (int)AccidentalType::LOWER_ONE_TRIDECIMAL_QUARTERTONE},
        {"RAISE_ONE_TRIDECIMAL_QUARTERTONE", (int)AccidentalType::RAISE_ONE_TRIDECIMAL_QUARTERTONE},
        {"DOUBLE_FLAT_EQUAL_TEMPERED", (int)AccidentalType::DOUBLE_FLAT_EQUAL_TEMPERED},
        {"FLAT_EQUAL_TEMPERED", (int)AccidentalType::FLAT_EQUAL_TEMPERED},
        {"NATURAL_EQUAL_TEMPERED", (int)AccidentalType::NATURAL_EQUAL_TEMPERED},
        {"SHARP_EQUAL_TEMPERED", (int)AccidentalType::SHARP_EQUAL_TEMPERED},
        {"DOUBLE_SHARP_EQUAL_TEMPERED", (int)AccidentalType::DOUBLE_SHARP_EQUAL_TEMPERED},
        {"QUARTER_FLAT_EQUAL_TEMPERED", (int)AccidentalType::QUARTER_FLAT_EQUAL_TEMPERED},
        {"QUARTER_SHARP_EQUAL_TEMPERED", (int)AccidentalType::QUARTER_SHARP_EQUAL_TEMPERED},
        {"FLAT_17", (int)AccidentalType::FLAT_17},
        {"SHARP_17", (int)AccidentalType::SHARP_17},
        {"FLAT_19", (int)AccidentalType::FLAT_19},
        {"SHARP_19", (int)AccidentalType::SHARP_19},
        {"FLAT_23", (int)AccidentalType::FLAT_23},
        {"SHARP_23", (int)AccidentalType::SHARP_23},
        {"FLAT_31", (int)AccidentalType::FLAT_31},
        {"SHARP_31", (int)AccidentalType::SHARP_31},
        {"FLAT_53", (int)AccidentalType::FLAT_53},
        {"SHARP_53", (int)AccidentalType::SHARP_53},
        {"SORI", (int)AccidentalType::SORI},
        {"KORON", (int)AccidentalType::KORON},
        {"TEN_TWELFTH_FLAT", (int)AccidentalType::TEN_TWELFTH_FLAT},
        {"TEN_TWELFTH_SHARP", (int)AccidentalType::TEN_TWELFTH_SHARP},
        {"ELEVEN_TWELFTH_FLAT", (int)AccidentalType::ELEVEN_TWELFTH_FLAT},
        {"ELEVEN_TWELFTH_SHARP", (int)AccidentalType::ELEVEN_TWELFTH_SHARP},
        {"ONE_TWELFTH_FLAT", (int)AccidentalType::ONE_TWELFTH_FLAT},
        {"ONE_TWELFTH_SHARP", (int)AccidentalType::ONE_TWELFTH_SHARP},
        {"TWO_TWELFTH_FLAT", (int)AccidentalType::TWO_TWELFTH_FLAT},
        {"TWO_TWELFTH_SHARP", (int)AccidentalType::TWO_TWELFTH_SHARP},
        {"THREE_TWELFTH_FLAT", (int)AccidentalType::THREE_TWELFTH_FLAT},
        {"THREE_TWELFTH_SHARP", (int)AccidentalType::THREE_TWELFTH_SHARP},
        {"FOUR_TWELFTH_FLAT", (int)AccidentalType::FOUR_TWELFTH_FLAT},
        {"FOUR_TWELFTH_SHARP", (int)AccidentalType::FOUR_TWELFTH_SHARP},
        {"FIVE_TWELFTH_FLAT", (int)AccidentalType::FIVE_TWELFTH_FLAT},
        {"FIVE_TWELFTH_SHARP", (int)AccidentalType::FIVE_TWELFTH_SHARP},
        {"SIX_TWELFTH_FLAT", (int)AccidentalType::SIX_TWELFTH_FLAT},
        {"SIX_TWELFTH_SHARP", (int)AccidentalType::SIX_TWELFTH_SHARP},
        {"SEVEN_TWELFTH_FLAT", (int)AccidentalType::SEVEN_TWELFTH_FLAT},
        {"SEVEN_TWELFTH_SHARP", (int)AccidentalType::SEVEN_TWELFTH_SHARP},
        {"EIGHT_TWELFTH_FLAT", (int)AccidentalType::EIGHT_TWELFTH_FLAT},
        {"EIGHT_TWELFTH_SHARP", (int)AccidentalType::EIGHT_TWELFTH_SHARP},
        {"NINE_TWELFTH_FLAT", (int)AccidentalType::NINE_TWELFTH_FLAT},
        {"NINE_TWELFTH_SHARP", (int)AccidentalType::NINE_TWELFTH_SHARP},
        {"SAGITTAL_5V7KD", (int)AccidentalType::SAGITTAL_5V7KD},
        {"SAGITTAL_5V7KU", (int)AccidentalType::SAGITTAL_5V7KU},
        {"SAGITTAL_5CD", (int)AccidentalType::SAGITTAL_5CD},
        {"SAGITTAL_5CU", (int)AccidentalType::SAGITTAL_5CU},
        {"SAGITTAL_7CD", (int)AccidentalType::SAGITTAL_7CD},
        {"SAGITTAL_7CU", (int)AccidentalType::SAGITTAL_7CU},
        {"SAGITTAL_25SDD", (int)AccidentalType::SAGITTAL_25SDD},
        {"SAGITTAL_25SDU", (int)AccidentalType::SAGITTAL_25SDU},
        {"SAGITTAL_35MDD", (int)AccidentalType::SAGITTAL_35MDD},
        {"SAGITTAL_35MDU", (int)AccidentalType::SAGITTAL_35MDU},
        {"SAGITTAL_11MDD", (int)AccidentalType::SAGITTAL_11MDD},
        {"SAGITTAL_11MDU", (int)AccidentalType::SAGITTAL_11MDU},
        {"SAGITTAL_11LDD", (int)AccidentalType::SAGITTAL_11LDD},
        {"SAGITTAL_11LDU", (int)AccidentalType::SAGITTAL_11LDU},
        {"SAGITTAL_35LDD", (int)AccidentalType::SAGITTAL_35LDD},
        {"SAGITTAL_35LDU", (int)AccidentalType::SAGITTAL_35LDU},
        {"SAGITTAL_FLAT25SU", (int)AccidentalType::SAGITTAL_FLAT25SU},
        {"SAGITTAL_SHARP25SD", (int)AccidentalType::SAGITTAL_SHARP25SD},
        {"SAGITTAL_FLAT7CU", (int)AccidentalType::SAGITTAL_FLAT7CU},
        {"SAGITTAL_SHARP7CD", (int)AccidentalType::SAGITTAL_SHARP7CD},
        {"SAGITTAL_FLAT5CU", (int)AccidentalType::SAGITTAL_FLAT5CU},
        {"SAGITTAL_SHARP5CD", (int)AccidentalType::SAGITTAL_SHARP5CD},
        {"SAGITTAL_FLAT5V7KU", (int)AccidentalType::SAGITTAL_FLAT5V7KU},
        {"SAGITTAL_SHARP5V7KD", (int)AccidentalType::SAGITTAL_SHARP5V7KD},
        {"SAGITTAL_FLAT", (int)AccidentalType::SAGITTAL_FLAT},
        {"SAGITTAL_SHARP", (int)AccidentalType::SAGITTAL_SHARP},
        {"ONE_COMMA_FLAT", (int)AccidentalType::ONE_COMMA_FLAT},
        {"ONE_COMMA_SHARP", (int)AccidentalType::ONE_COMMA_SHARP},
        {"TWO_COMMA_FLAT", (int)AccidentalType::TWO_COMMA_FLAT},
        {"TWO_COMMA_SHARP", (int)AccidentalType::TWO_COMMA_SHARP},
        {"THREE_COMMA_FLAT", (int)AccidentalType::THREE_COMMA_FLAT},
        {"THREE_COMMA_SHARP", (int)AccidentalType::THREE_COMMA_SHARP},
        {"FOUR_COMMA_FLAT", (int)AccidentalType::FOUR_COMMA_FLAT},
        {"FIVE_COMMA_SHARP", (int)AccidentalType::FIVE_COMMA_SHARP}
    };
};
}

#endif

