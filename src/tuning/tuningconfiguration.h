
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
#ifndef MU_TUNING_TUNINGCONFIGURATION_H
#define MU_TUNING_TUNINGCONFIGURATION_H

#include "ituningconfiguration.h"
#include "io/path.h"
#include "dom/types.h"
#include <QJsonValue>

namespace mu::tuning {
class TuningConfiguration : public ITuningConfiguration
{

public:
    void init(const io::path_t& tuningFile) override;
    bool valid() const override { return m_valid; };
    std::string getName() const override { return m_name; };
    double getOffset (int pitch, int tpc, int accidentalType) const override;
private:
    double parseScalaValue(QJsonValue scalaValueJson);
    int getNominal(int index);

    std::string m_name = "";

    bool m_valid = false;
    std::map<int, double> m_nominalOffsetMap;
    std::map<int, double> m_accidentalOffsetMap;

    std::vector<int> m_standardNominalSteps = {2, 2, 1, 2, 2, 2, 1};

    // maps nominals to a map of accidentals to offsets
    std::map<int, std::map<int, double>> m_offsetMap;

    // Map accidental identifiers from the JSON format to accidentals.
    // There's no way to get the enum symbol as a string in C++17, so
    // we'll need to keep this in sync with types.h manually
    std::map<std::string, int> m_accidentalNameMap {
        {"NONE", (int)engraving::AccidentalType::NONE},
        {"FLAT", (int)engraving::AccidentalType::FLAT},
        {"NATURAL", (int)engraving::AccidentalType::NATURAL},
        {"SHARP", (int)engraving::AccidentalType::SHARP},
        {"SHARP2", (int)engraving::AccidentalType::SHARP2},
        {"FLAT2", (int)engraving::AccidentalType::FLAT2},
        {"SHARP3", (int)engraving::AccidentalType::SHARP3},
        {"FLAT3", (int)engraving::AccidentalType::FLAT3},
        {"NATURAL_FLAT", (int)engraving::AccidentalType::NATURAL_FLAT},
        {"NATURAL_SHARP", (int)engraving::AccidentalType::NATURAL_SHARP},
        {"SHARP_SHARP", (int)engraving::AccidentalType::SHARP_SHARP},
        {"FLAT_ARROW_UP", (int)engraving::AccidentalType::FLAT_ARROW_UP},
        {"FLAT_ARROW_DOWN", (int)engraving::AccidentalType::FLAT_ARROW_DOWN},
        {"NATURAL_ARROW_UP", (int)engraving::AccidentalType::NATURAL_ARROW_UP},
        {"NATURAL_ARROW_DOWN", (int)engraving::AccidentalType::NATURAL_ARROW_DOWN},
        {"SHARP_ARROW_UP", (int)engraving::AccidentalType::SHARP_ARROW_UP},
        {"SHARP_ARROW_DOWN", (int)engraving::AccidentalType::SHARP_ARROW_DOWN},
        {"SHARP2_ARROW_UP", (int)engraving::AccidentalType::SHARP2_ARROW_UP},
        {"SHARP2_ARROW_DOWN", (int)engraving::AccidentalType::SHARP2_ARROW_DOWN},
        {"FLAT2_ARROW_UP", (int)engraving::AccidentalType::FLAT2_ARROW_UP},
        {"FLAT2_ARROW_DOWN", (int)engraving::AccidentalType::FLAT2_ARROW_DOWN},
        {"ARROW_DOWN", (int)engraving::AccidentalType::ARROW_DOWN},
        {"ARROW_UP", (int)engraving::AccidentalType::ARROW_UP},
        {"MIRRORED_FLAT", (int)engraving::AccidentalType::MIRRORED_FLAT},
        {"MIRRORED_FLAT2", (int)engraving::AccidentalType::MIRRORED_FLAT2},
        {"SHARP_SLASH", (int)engraving::AccidentalType::SHARP_SLASH},
        {"SHARP_SLASH4", (int)engraving::AccidentalType::SHARP_SLASH4},
        {"FLAT_SLASH2", (int)engraving::AccidentalType::FLAT_SLASH2},
        {"FLAT_SLASH", (int)engraving::AccidentalType::FLAT_SLASH},
        {"SHARP_SLASH3", (int)engraving::AccidentalType::SHARP_SLASH3},
        {"SHARP_SLASH2", (int)engraving::AccidentalType::SHARP_SLASH2},
        {"DOUBLE_FLAT_ONE_ARROW_DOWN", (int)engraving::AccidentalType::DOUBLE_FLAT_ONE_ARROW_DOWN},
        {"FLAT_ONE_ARROW_DOWN", (int)engraving::AccidentalType::FLAT_ONE_ARROW_DOWN},
        {"NATURAL_ONE_ARROW_DOWN", (int)engraving::AccidentalType::NATURAL_ONE_ARROW_DOWN},
        {"SHARP_ONE_ARROW_DOWN", (int)engraving::AccidentalType::SHARP_ONE_ARROW_DOWN},
        {"DOUBLE_SHARP_ONE_ARROW_DOWN", (int)engraving::AccidentalType::DOUBLE_SHARP_ONE_ARROW_DOWN},
        {"DOUBLE_FLAT_ONE_ARROW_UP", (int)engraving::AccidentalType::DOUBLE_FLAT_ONE_ARROW_UP},
        {"FLAT_ONE_ARROW_UP", (int)engraving::AccidentalType::FLAT_ONE_ARROW_UP},
        {"NATURAL_ONE_ARROW_UP", (int)engraving::AccidentalType::NATURAL_ONE_ARROW_UP},
        {"SHARP_ONE_ARROW_UP", (int)engraving::AccidentalType::SHARP_ONE_ARROW_UP},
        {"DOUBLE_SHARP_ONE_ARROW_UP", (int)engraving::AccidentalType::DOUBLE_SHARP_ONE_ARROW_UP},
        {"DOUBLE_FLAT_TWO_ARROWS_DOWN", (int)engraving::AccidentalType::DOUBLE_FLAT_TWO_ARROWS_DOWN},
        {"FLAT_TWO_ARROWS_DOWN", (int)engraving::AccidentalType::FLAT_TWO_ARROWS_DOWN},
        {"NATURAL_TWO_ARROWS_DOWN", (int)engraving::AccidentalType::NATURAL_TWO_ARROWS_DOWN},
        {"SHARP_TWO_ARROWS_DOWN", (int)engraving::AccidentalType::SHARP_TWO_ARROWS_DOWN},
        {"DOUBLE_SHARP_TWO_ARROWS_DOWN", (int)engraving::AccidentalType::DOUBLE_SHARP_TWO_ARROWS_DOWN},
        {"DOUBLE_FLAT_TWO_ARROWS_UP", (int)engraving::AccidentalType::DOUBLE_FLAT_TWO_ARROWS_UP},
        {"FLAT_TWO_ARROWS_UP", (int)engraving::AccidentalType::FLAT_TWO_ARROWS_UP},
        {"NATURAL_TWO_ARROWS_UP", (int)engraving::AccidentalType::NATURAL_TWO_ARROWS_UP},
        {"SHARP_TWO_ARROWS_UP", (int)engraving::AccidentalType::SHARP_TWO_ARROWS_UP},
        {"DOUBLE_SHARP_TWO_ARROWS_UP", (int)engraving::AccidentalType::DOUBLE_SHARP_TWO_ARROWS_UP},
        {"DOUBLE_FLAT_THREE_ARROWS_DOWN", (int)engraving::AccidentalType::DOUBLE_FLAT_THREE_ARROWS_DOWN},
        {"FLAT_THREE_ARROWS_DOWN", (int)engraving::AccidentalType::FLAT_THREE_ARROWS_DOWN},
        {"NATURAL_THREE_ARROWS_DOWN", (int)engraving::AccidentalType::NATURAL_THREE_ARROWS_DOWN},
        {"SHARP_THREE_ARROWS_DOWN", (int)engraving::AccidentalType::SHARP_THREE_ARROWS_DOWN},
        {"DOUBLE_SHARP_THREE_ARROWS_DOWN", (int)engraving::AccidentalType::DOUBLE_SHARP_THREE_ARROWS_DOWN},
        {"DOUBLE_FLAT_THREE_ARROWS_UP", (int)engraving::AccidentalType::DOUBLE_FLAT_THREE_ARROWS_UP},
        {"FLAT_THREE_ARROWS_UP", (int)engraving::AccidentalType::FLAT_THREE_ARROWS_UP},
        {"NATURAL_THREE_ARROWS_UP", (int)engraving::AccidentalType::NATURAL_THREE_ARROWS_UP},
        {"SHARP_THREE_ARROWS_UP", (int)engraving::AccidentalType::SHARP_THREE_ARROWS_UP},
        {"DOUBLE_SHARP_THREE_ARROWS_UP", (int)engraving::AccidentalType::DOUBLE_SHARP_THREE_ARROWS_UP},
        {"LOWER_ONE_SEPTIMAL_COMMA", (int)engraving::AccidentalType::LOWER_ONE_SEPTIMAL_COMMA},
        {"RAISE_ONE_SEPTIMAL_COMMA", (int)engraving::AccidentalType::RAISE_ONE_SEPTIMAL_COMMA},
        {"LOWER_TWO_SEPTIMAL_COMMAS", (int)engraving::AccidentalType::LOWER_TWO_SEPTIMAL_COMMAS},
        {"RAISE_TWO_SEPTIMAL_COMMAS", (int)engraving::AccidentalType::RAISE_TWO_SEPTIMAL_COMMAS},
        {"LOWER_ONE_UNDECIMAL_QUARTERTONE", (int)engraving::AccidentalType::LOWER_ONE_UNDECIMAL_QUARTERTONE},
        {"RAISE_ONE_UNDECIMAL_QUARTERTONE", (int)engraving::AccidentalType::RAISE_ONE_UNDECIMAL_QUARTERTONE},
        {"LOWER_ONE_TRIDECIMAL_QUARTERTONE", (int)engraving::AccidentalType::LOWER_ONE_TRIDECIMAL_QUARTERTONE},
        {"RAISE_ONE_TRIDECIMAL_QUARTERTONE", (int)engraving::AccidentalType::RAISE_ONE_TRIDECIMAL_QUARTERTONE},
        {"DOUBLE_FLAT_EQUAL_TEMPERED", (int)engraving::AccidentalType::DOUBLE_FLAT_EQUAL_TEMPERED},
        {"FLAT_EQUAL_TEMPERED", (int)engraving::AccidentalType::FLAT_EQUAL_TEMPERED},
        {"NATURAL_EQUAL_TEMPERED", (int)engraving::AccidentalType::NATURAL_EQUAL_TEMPERED},
        {"SHARP_EQUAL_TEMPERED", (int)engraving::AccidentalType::SHARP_EQUAL_TEMPERED},
        {"DOUBLE_SHARP_EQUAL_TEMPERED", (int)engraving::AccidentalType::DOUBLE_SHARP_EQUAL_TEMPERED},
        {"QUARTER_FLAT_EQUAL_TEMPERED", (int)engraving::AccidentalType::QUARTER_FLAT_EQUAL_TEMPERED},
        {"QUARTER_SHARP_EQUAL_TEMPERED", (int)engraving::AccidentalType::QUARTER_SHARP_EQUAL_TEMPERED},
        {"FLAT_17", (int)engraving::AccidentalType::FLAT_17},
        {"SHARP_17", (int)engraving::AccidentalType::SHARP_17},
        {"FLAT_19", (int)engraving::AccidentalType::FLAT_19},
        {"SHARP_19", (int)engraving::AccidentalType::SHARP_19},
        {"FLAT_23", (int)engraving::AccidentalType::FLAT_23},
        {"SHARP_23", (int)engraving::AccidentalType::SHARP_23},
        {"FLAT_31", (int)engraving::AccidentalType::FLAT_31},
        {"SHARP_31", (int)engraving::AccidentalType::SHARP_31},
        {"FLAT_53", (int)engraving::AccidentalType::FLAT_53},
        {"SHARP_53", (int)engraving::AccidentalType::SHARP_53},
        {"SORI", (int)engraving::AccidentalType::SORI},
        {"KORON", (int)engraving::AccidentalType::KORON},
        {"TEN_TWELFTH_FLAT", (int)engraving::AccidentalType::TEN_TWELFTH_FLAT},
        {"TEN_TWELFTH_SHARP", (int)engraving::AccidentalType::TEN_TWELFTH_SHARP},
        {"ELEVEN_TWELFTH_FLAT", (int)engraving::AccidentalType::ELEVEN_TWELFTH_FLAT},
        {"ELEVEN_TWELFTH_SHARP", (int)engraving::AccidentalType::ELEVEN_TWELFTH_SHARP},
        {"ONE_TWELFTH_FLAT", (int)engraving::AccidentalType::ONE_TWELFTH_FLAT},
        {"ONE_TWELFTH_SHARP", (int)engraving::AccidentalType::ONE_TWELFTH_SHARP},
        {"TWO_TWELFTH_FLAT", (int)engraving::AccidentalType::TWO_TWELFTH_FLAT},
        {"TWO_TWELFTH_SHARP", (int)engraving::AccidentalType::TWO_TWELFTH_SHARP},
        {"THREE_TWELFTH_FLAT", (int)engraving::AccidentalType::THREE_TWELFTH_FLAT},
        {"THREE_TWELFTH_SHARP", (int)engraving::AccidentalType::THREE_TWELFTH_SHARP},
        {"FOUR_TWELFTH_FLAT", (int)engraving::AccidentalType::FOUR_TWELFTH_FLAT},
        {"FOUR_TWELFTH_SHARP", (int)engraving::AccidentalType::FOUR_TWELFTH_SHARP},
        {"FIVE_TWELFTH_FLAT", (int)engraving::AccidentalType::FIVE_TWELFTH_FLAT},
        {"FIVE_TWELFTH_SHARP", (int)engraving::AccidentalType::FIVE_TWELFTH_SHARP},
        {"SIX_TWELFTH_FLAT", (int)engraving::AccidentalType::SIX_TWELFTH_FLAT},
        {"SIX_TWELFTH_SHARP", (int)engraving::AccidentalType::SIX_TWELFTH_SHARP},
        {"SEVEN_TWELFTH_FLAT", (int)engraving::AccidentalType::SEVEN_TWELFTH_FLAT},
        {"SEVEN_TWELFTH_SHARP", (int)engraving::AccidentalType::SEVEN_TWELFTH_SHARP},
        {"EIGHT_TWELFTH_FLAT", (int)engraving::AccidentalType::EIGHT_TWELFTH_FLAT},
        {"EIGHT_TWELFTH_SHARP", (int)engraving::AccidentalType::EIGHT_TWELFTH_SHARP},
        {"NINE_TWELFTH_FLAT", (int)engraving::AccidentalType::NINE_TWELFTH_FLAT},
        {"NINE_TWELFTH_SHARP", (int)engraving::AccidentalType::NINE_TWELFTH_SHARP},
        {"SAGITTAL_5V7KD", (int)engraving::AccidentalType::SAGITTAL_5V7KD},
        {"SAGITTAL_5V7KU", (int)engraving::AccidentalType::SAGITTAL_5V7KU},
        {"SAGITTAL_5CD", (int)engraving::AccidentalType::SAGITTAL_5CD},
        {"SAGITTAL_5CU", (int)engraving::AccidentalType::SAGITTAL_5CU},
        {"SAGITTAL_7CD", (int)engraving::AccidentalType::SAGITTAL_7CD},
        {"SAGITTAL_7CU", (int)engraving::AccidentalType::SAGITTAL_7CU},
        {"SAGITTAL_25SDD", (int)engraving::AccidentalType::SAGITTAL_25SDD},
        {"SAGITTAL_25SDU", (int)engraving::AccidentalType::SAGITTAL_25SDU},
        {"SAGITTAL_35MDD", (int)engraving::AccidentalType::SAGITTAL_35MDD},
        {"SAGITTAL_35MDU", (int)engraving::AccidentalType::SAGITTAL_35MDU},
        {"SAGITTAL_11MDD", (int)engraving::AccidentalType::SAGITTAL_11MDD},
        {"SAGITTAL_11MDU", (int)engraving::AccidentalType::SAGITTAL_11MDU},
        {"SAGITTAL_11LDD", (int)engraving::AccidentalType::SAGITTAL_11LDD},
        {"SAGITTAL_11LDU", (int)engraving::AccidentalType::SAGITTAL_11LDU},
        {"SAGITTAL_35LDD", (int)engraving::AccidentalType::SAGITTAL_35LDD},
        {"SAGITTAL_35LDU", (int)engraving::AccidentalType::SAGITTAL_35LDU},
        {"SAGITTAL_FLAT25SU", (int)engraving::AccidentalType::SAGITTAL_FLAT25SU},
        {"SAGITTAL_SHARP25SD", (int)engraving::AccidentalType::SAGITTAL_SHARP25SD},
        {"SAGITTAL_FLAT7CU", (int)engraving::AccidentalType::SAGITTAL_FLAT7CU},
        {"SAGITTAL_SHARP7CD", (int)engraving::AccidentalType::SAGITTAL_SHARP7CD},
        {"SAGITTAL_FLAT5CU", (int)engraving::AccidentalType::SAGITTAL_FLAT5CU},
        {"SAGITTAL_SHARP5CD", (int)engraving::AccidentalType::SAGITTAL_SHARP5CD},
        {"SAGITTAL_FLAT5V7KU", (int)engraving::AccidentalType::SAGITTAL_FLAT5V7KU},
        {"SAGITTAL_SHARP5V7KD", (int)engraving::AccidentalType::SAGITTAL_SHARP5V7KD},
        {"SAGITTAL_FLAT", (int)engraving::AccidentalType::SAGITTAL_FLAT},
        {"SAGITTAL_SHARP", (int)engraving::AccidentalType::SAGITTAL_SHARP},
        {"ONE_COMMA_FLAT", (int)engraving::AccidentalType::ONE_COMMA_FLAT},
        {"ONE_COMMA_SHARP", (int)engraving::AccidentalType::ONE_COMMA_SHARP},
        {"TWO_COMMA_FLAT", (int)engraving::AccidentalType::TWO_COMMA_FLAT},
        {"TWO_COMMA_SHARP", (int)engraving::AccidentalType::TWO_COMMA_SHARP},
        {"THREE_COMMA_FLAT", (int)engraving::AccidentalType::THREE_COMMA_FLAT},
        {"THREE_COMMA_SHARP", (int)engraving::AccidentalType::THREE_COMMA_SHARP},
        {"FOUR_COMMA_FLAT", (int)engraving::AccidentalType::FOUR_COMMA_FLAT},
        {"FIVE_COMMA_SHARP", (int)engraving::AccidentalType::FIVE_COMMA_SHARP}
    };
};
}

#endif

