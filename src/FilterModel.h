/**
 * File name: DistortionModel.h
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

#ifndef FILTER_MODEL_H
#define FILTER_MODEL_H

#include "AbstractModel.h"
#include "geonkick_api.h"
#include "envelope.h"

class FilterModel: public AbstractModel
{
 public:
        using FilterType = GeonkickApi::FilterType;
        explicit FilterModel(RkObject *parent);
        virtual ~FilterModel() = default;
        virtual void enable(bool b) = 0;
        virtual bool isEnabled() const = 0;
        virtual void setCutOff(double val) = 0;
        virtual double cutOff() const = 0;
        virtual void setResonance(double val) = 0;
        virtual double resonance() const = 0;
        virtual void setType(FilterType type) = 0;
        virtual FilterType type() const = 0;
        virtual void setCutOffRange(const std::pair<double, double>& range) = 0;
        virtual std::pair<double, double> cutOffRange() const = 0;
        virtual void setResonanceRange(const std::pair<double, double>& range) = 0;
        virtual std::pair<double, double> resonanceRange() const = 0;
        virtual Envelope::Category envelopeCategory() const = 0;

        RK_DECL_ACT(enabled, enabled(bool b), RK_ARG_TYPE(bool), RK_ARG_VAL(b));
        RK_DECL_ACT(cutOffChanged, cutOffChanged(double val),
                    RK_ARG_TYPE(double), RK_ARG_VAL(val));
        RK_DECL_ACT(resonanceChanged, resonanceChanged(double val),
                    RK_ARG_TYPE(double), RK_ARG_VAL(val));
        RK_DECL_ACT(resonanceRangeChanged,
                    resonanceRangeChanged(const std::pair<double, double>& range),
                    RK_ARG_TYPE(const std::pair<double, double>&),
                    RK_ARG_VAL(range));
        RK_DECL_ACT(cutOffRangeChanged,
                    cutOffRangeChanged(const std::pair<double, double>& range),
                    RK_ARG_TYPE(const std::pair<double, double>&),
                    RK_ARG_VAL(range));
        RK_DECL_ACT(typeChanged, typeChanged(FilterType type),
                    RK_ARG_TYPE(FilterType),
                    RK_ARG_VAL(type));
};

#endif // FILTER_MODEL_H
