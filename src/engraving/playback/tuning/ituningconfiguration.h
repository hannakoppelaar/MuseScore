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
#ifndef MU_ENGRAVING_TUNING_ITUNINGCONFIGURATION_H
#define MU_ENGRAVING_TUNING_ITUNINGCONFIGURATION_H

#include "modularity/imoduleinterface.h"
#include "io/path.h"

namespace mu::engraving::tuning {
class ITuningConfiguration : MODULE_EXPORT_INTERFACE
{
    INTERFACE_ID(ITuningConfiguration)

public:
    virtual ~ITuningConfiguration() = default;

    virtual void init(const io::path_t& tuningFile) = 0;
    virtual bool valid() const = 0;
    virtual std::string getName() const = 0;
    virtual double getOffset (int pitch, int tpc, int accidentalType) const = 0;
};
}

#endif

