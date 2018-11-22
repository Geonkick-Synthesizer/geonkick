/**
 * File name: knob.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#include "knob.h"
#include "geonkick_theme.h"

#include <QPainter>
#include <QDebug>
#include <QStyleOption>
#include <math.h>

#define GEONKICK_KNOB_MAX_DEGREE 270
#define GEONKICK_KNOB_MIN_DEGREE 0
#define GEONKICK_KNOB_RANGE_DEGREE (GEONKICK_KNOB_MAX_DEGREE - GEONKICK_KNOB_MIN_DEGREE)

Knob::Knob(GeonkickWidget *parent)
          : GeonkickWidget(parent),
	  knobValueDegree(GEONKICK_KNOB_MIN_DEGREE),
          rangeFrom(0),
          rangeTo(0),
          isSelected(false)
{
        QPalette pal;
        pal.setColor(QPalette::Background, QColor(68, 68, 70, 0));
        pal.setColor(QPalette::WindowText, Qt::white);
        setAutoFillBackground(true);
        setPalette(pal);
}

Knob::~Knob()
{
}

void Knob::setKnobImage(const QPixmap &pixmap)
{
        knobPixmap = pixmap;
}

void
Knob::paintWidget(QPaintEvent *event)
{
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
Knob::mousePressEvent(QMouseEvent *event)
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
Knob::mouseReleaseEvent(QMouseEvent *event)
{
        isSelected = false;
}

void
Knob::mouseMoveEvent(QMouseEvent *event)
{
        if (isSelected) {
                int dy = event->y() - lastPositionPoint.y();
                knobValueDegree -= 0.5 * dy;
                if (knobValueDegree < GEONKICK_KNOB_MIN_DEGREE) {
                        knobValueDegree = GEONKICK_KNOB_MIN_DEGREE;
                } else if (knobValueDegree > GEONKICK_KNOB_MAX_DEGREE) {
                        knobValueDegree = GEONKICK_KNOB_MAX_DEGREE;
                }
                lastPositionPoint.setX(event->x());
                lastPositionPoint.setY(event->y());
                double k = (knobValueDegree - GEONKICK_KNOB_MIN_DEGREE) / GEONKICK_KNOB_RANGE_DEGREE;
                emit valueUpdated(rangeFrom + k * (rangeTo - rangeFrom));
                update();
        }
}

double Knob::getValue(void) const
{
        double k = (knobValueDegree - GEONKICK_KNOB_MIN_DEGREE) / GEONKICK_KNOB_RANGE_DEGREE;
	return rangeFrom + k * (rangeTo - rangeFrom);
}

void Knob::setRange(double from, double to)
{
        rangeFrom = from;
        rangeTo = to;
}

void Knob::setCurrentValue(double val)
{
        if (val > rangeTo) {
                val = rangeTo;
        } else if (val < rangeFrom) {
                val = rangeFrom;
        }

        double k = 0;
        if (fabs(rangeTo - rangeFrom) < std::numeric_limits<double>::epsilon()) {
                knobValueDegree = GEONKICK_KNOB_MIN_DEGREE;
        } else {
                k = (val - rangeFrom) / (rangeTo - rangeFrom);
        }
        knobValueDegree = GEONKICK_KNOB_MIN_DEGREE + k * GEONKICK_KNOB_RANGE_DEGREE;
}

