/**
 * File name: filter.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
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

#ifndef GEONKICK_FILTER_H
#define GEONKICK_FILTER_H

#include "geonkick_widget.h"
#include "geonkick_api.h"

class GeonkickButton;
class Knob;

class Filter: public GeonkickWidget
{
 public:
        Filter(GeonkickWidget *parent);

        void enable(bool b);
        bool isEnabled() const;
        void setCutOff(double val);
        double cutOff() const;
        void setResonance(double val);
        double resonance() const;
        void setType(GeonkickApi::FilterType type);
        GeonkickApi::FilterType type() const;
        RK_DECL_ACT(enabled, enabled(bool b), RK_ARG_TYPE(bool), RK_ARG_VAL(b));
        RK_DECL_ACT(cutoffChanged, cutoffChanged(double val), RK_ARG_TYPE(double), RK_ARG_VAL(val));
        RK_DECL_ACT(resonanceChanged, resonanceChanged(double val), RK_ARG_TYPE(double), RK_ARG_VAL(val));
        RK_DECL_ACT(typeChanged, typeChanged(GeonkickApi::FilterType type), RK_ARG_TYPE(FilterType), RK_ARG_VAL(val));

 private:
        GeonkickButton *filterCheckbox;
        Knob *cutOffKnob;
        Knob *resonanceKnob;
};

#endif // GEONKICK_FILTER_H
