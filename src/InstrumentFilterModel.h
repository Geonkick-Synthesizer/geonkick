/**
 * File name: InstrumentFilterModel.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef INSTRUEMNT_FILTER_MODEL_H
#define INSTRUEMNT_FILTER_MODEL_H

#include "FilterModel.h"

class PercussionModel;

class InstrumentFilterModel: public FilterModel
{
 public:
        InstrumentFilterModel(PercussionModel *parent);
         void enable(bool b);
         bool isEnabled() const;
         void setCutOff(double val);
         double cutOff() const;
         void setResonance(double val);
         double resonance() const;
         void setType(FilterType type);
         FilterType type() const;
         void setCutOffRange(const std::pair<double, double>& range);
         std::pair<double, double> cutOffRange() const;
         void setResonanceRange(const std::pair<double, double>& range);
         std::pair<double, double> resonanceRange() const;

 private:
        PercussionModel *instrumentModel;
};

#endif // INSTRUEMNT_FILTER_MODEL_H
