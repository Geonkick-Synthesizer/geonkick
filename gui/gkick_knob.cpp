/**
 * File name: gkick_knob.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#include "gkick_knob.h"

#include <QPainter>
#include <QDebug>
#include <QStyleOption>

#include "geonkick_theme.h"

#define GEONKICK_KNOB_MAX_DEGREE 270
#define GEONKICK_KNOB_MIN_DEGREE 0
#define GEONKICK_KNOB_RANGE_DEGREE (GEONKICK_KNOB_MAX_DEGREE - GEONKICK_KNOB_MIN_DEGREE)

GKickKnob::GKickKnob(GeonkickWidget *parent)
          : GeonkickWidget(parent),
	  knobValueDegree(GEONKICK_KNOB_MIN_DEGREE),
	  rangeValue(0),
          isSelected(false)
{
}

GKickKnob::~GKickKnob()
{
}

void GKickKnob::setKnobImage(const QPixmap &pixmap)
{
        knobPixmap = pixmap;
}

void
GKickKnob::paintEvent(QPaintEvent *event)
{
        GeonkickWidget::paintEvent(event);
        QPainter painter(this);
        if (!knobPixmap.isNull()) {
                painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
                painter.translate(width() / 2, height() / 2);
                painter.rotate(knobValueDegree);
                int x = (width() - knobPixmap.size().width()) / 2 - width() / 2;
                int y = (height() - knobPixmap.size().height()) / 2 - height() / 2;
                painter.drawPixmap(x, y, knobPixmap.size().width(), knobPixmap.size().height(), knobPixmap);
        }
}

void
GKickKnob::mousePressEvent(QMouseEvent *event)
{
        if (!knobPixmap.isNull()) {
                int xCenter = width() / 2;
                int yCenter = height() / 2;
                int r = knobPixmap.size().width() / 2;
                if ((event->x() - xCenter) * (event->x() - xCenter) +
                    (event->y() - yCenter) * (event->y() - yCenter) <=  r * r)
                {
                        isSelected = true;
                        lastPositionPoint.setX(event->x());
                        lastPositionPoint.setY(event->y());
                }
        }
}

void
GKickKnob::mouseReleaseEvent(QMouseEvent *event)
{
        isSelected = false;
}

void
GKickKnob::mouseMoveEvent(QMouseEvent *event)
{
        if (isSelected) {
                int dy = event->y() - lastPositionPoint.y();
                knobValueDegree -= dy;
                if (knobValueDegree < GEONKICK_KNOB_MIN_DEGREE) {
                        knobValueDegree = GEONKICK_KNOB_MIN_DEGREE;
                } else if (knobValueDegree > GEONKICK_KNOB_MAX_DEGREE) {
                        knobValueDegree = GEONKICK_KNOB_MAX_DEGREE;
                }
                lastPositionPoint.setX(event->x());
                lastPositionPoint.setY(event->y());
                double k = (knobValueDegree - GEONKICK_KNOB_MIN_DEGREE) / GEONKICK_KNOB_RANGE_DEGREE;
                emit valueUpdated(k * rangeValue);
                update();
        }
}

double GKickKnob::getValue(void)
{
        double k = (knobValueDegree - GEONKICK_KNOB_MIN_DEGREE) / GEONKICK_KNOB_RANGE_DEGREE;
	return k * rangeValue;
}

void GKickKnob::setMaxValue(double val)
{
}

void GKickKnob::setCurrentValue(double val)
{
}

