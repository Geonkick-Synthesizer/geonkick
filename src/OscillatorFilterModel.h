/**
 * File name: OscillatorFilterModel.h
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

#ifndef OSCILLATOR_FILTER_MODEL_H
#define OSCILLATOR_FILTER_MODEL_H

#include "FilterModel.h"

class OscillatorModel;

class OscillatorFilterModel: public FilterModel
{
 public:
        OscillatorFilterModel(OscillatorModel *parent);
        void enable(bool b) override;
        bool isEnabled() const override;
        void setCutOff(double val) override;
        double cutOff() const override;
        void setResonance(double val) override;
        double resonance() const override;
        void setType(FilterType type) override;
        FilterType type() const override;
        void setCutOffRange(const std::pair<double, double>& range) override;
        std::pair<double, double> cutOffRange() const override;
        void setResonanceRange(const std::pair<double, double>& range) override;
        std::pair<double, double> resonanceRange() const override;
        Envelope::Category envelopeCategory() const override;

 private:
        OscillatorModel *oscillatorModel;
};

#endif // OSCILLATOR_DISTORTION_MODEL_H
