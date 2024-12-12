/**
 * File name: FilterView.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor
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

#ifndef GEONKICK_FILTER_VIEW_H
#define GEONKICK_FILTER_VIEW_H

#include "AbstractView.h"
#include "FilterModel.h"
#include "geonkick_api.h"
#include "envelope.h"

class GeonkickButton;
class Knob;

class FilterView: public AbstractView
{
        using FilterType = FilterModel::FilterType;
 public:
        explicit FilterView(GeonkickWidget *parent, FilterModel* model);
        void createView() override;
        void updateView() override;

 protected:
        void bindModel() override;
        void unbindModel() override;
        void onEnabled(bool b);
        void onCutOffChanged(double val);
        void onResonanceChanged(double val);
        void onTypeChanged(FilterType type);
        void mouseButtonPressEvent(RkMouseEvent *event) override;

 private:
        GeonkickButton *filterCheckbox;
        Knob *cutOffKnob;
        Knob *resonanceKnob;
        GeonkickButton *lpFilterButton;
        GeonkickButton *hpFilterButton;
        GeonkickButton *bpFilterButton;
        Envelope::Category envelopeCategory;
};

#endif // GEONKICK_FILTER_VIEW_H
