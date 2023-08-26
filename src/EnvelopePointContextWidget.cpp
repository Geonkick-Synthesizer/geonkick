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
                                                       Rk::WindowFlags flag)
        : GeonkickWidget(parent, flag)
        , pointEnvelope{envelope}
        , lineEdit{new RkLineEdit(this)}
{
        setFixedSize(150, 20);
        setBackgroundColor({68, 68, 70, 240});
        lineEdit->setSize(width(), height());
        lineEdit->moveCursorToEnd();
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
        GEONKICK_LOG_INFO("EnvelopePointContextWidget::setFocus()");
        lineEdit->setFocus();
}

void EnvelopePointContextWidget::setValue(rk_real val)
{
        lineEdit->setText(std::to_string(val));
}

void EnvelopePointContextWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Left) {
	}
}

void EnvelopePointContextWidget::mouseButtonReleaseEvent(RkMouseEvent *event)
{
}

void EnvelopePointContextWidget::mouseMoveEvent(RkMouseEvent *event)
{
}

void EnvelopePointContextWidget::closeEvent(RkCloseEvent *event)
{
        GEONKICK_LOG_INFO("void EnvelopePointContextWidget::closeEvent(RkCloseEvent *event)");
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
