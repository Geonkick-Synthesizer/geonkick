/**
 * File name: EnvelopePointContextWidget.cpp
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

#include "EnvelopePointContextWidget.h"
#include "envelope.h"

#include <RkEvent.h>
#include <RkLineEdit.h>

EnvelopePointContextWidget::EnvelopePointContextWidget(Envelope* envelope,
                                                       GeonkickWidget *parent,
                                                       Rk::WidgetFlags flag)
        : GeonkickWidget(parent, flag)
        , pointEnvelope{envelope}
        , lineEdit{new RkLineEdit(this)}
{
        setFixedSize(110, 30);
        setBackgroundColor({68, 68, 70});
        setBorderColor(40, 40, 40);
        setBorderWidth(1);
        lineEdit->setSize(100, 20);
        lineEdit->setPosition(5, 5);
        lineEdit->show();
        RK_ACT_BIND(lineEdit,
                    editingFinished,
                    RK_ACT_ARGS(),
                    this,
                    onUpdateValue());
        RK_ACT_BIND(lineEdit,
                    escapePressed,
                    RK_ACT_ARGS(),
                    this,
                    close());
        pointEnvelope->setEditCurrentPoint();
        setValue(pointEnvelope->getSelectedPointValue());
}

void EnvelopePointContextWidget::setFocus()
{
        lineEdit->setFocus();
}

void EnvelopePointContextWidget::setValue(rk_real val)
{
        double roundedValue = std::round(val * 10000.0) / 10000.0;
        lineEdit->setText(Geonkick::doubleToStr(roundedValue, 4));
        lineEdit->moveCursorToEnd();
}

void EnvelopePointContextWidget::closeEvent(RkCloseEvent *event)
{
        RkWidget::closeEvent(event);
}

void EnvelopePointContextWidget::onUpdateValue()
{
        double value = 0.0;
        try {
                value = std::stod(lineEdit->text());
        } catch (...) {
        }
        pointEnvelope->updateSelectedPointValue(value);
        close();
}
