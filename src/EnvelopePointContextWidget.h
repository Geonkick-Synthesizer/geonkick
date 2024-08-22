/**
 * File name: EnvelopePointContextWidget.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2023 Iurie Nistor
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

#ifndef GEONKICK_ENVELOPE_POINT_CONTEXT_WIDGET_H
#define GEONKICK_ENVELOPE_POINT_CONTEXT_WIDGET_H

#include "geonkick_widget.h"

class Envelope;
class RkLineEdit;

class EnvelopePointContextWidget: public GeonkickWidget
{
 public:
        EnvelopePointContextWidget(Envelope *envelope,
                                   GeonkickWidget *parent,
                                   Rk::WidgetFlags flag = Rk::WidgetFlags::Popup);
        void setValue(rk_real val);
        RK_DECL_ACT(valueUpdated,
                    valueUpdated(rk_real val),
                    RK_ARG_TYPE(rk_real),
                    RK_ARG_VAL(val));
        void setFocus();
 protected:
        void closeEvent(RkCloseEvent *event) override;
        void onUpdateValue();

 private:
        Envelope *pointEnvelope;
        RkLineEdit *lineEdit;
};

#endif // GEONKICK_ENVELOPE_POINT_CONTEXT_WIDGET_H
