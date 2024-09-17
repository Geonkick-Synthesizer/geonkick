/**
 * File name: knob.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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

#include <RkPainter.h>
#include <RkEvent.h>

#define GEONKICK_KNOB_MAX_DEGREE 270
#define GEONKICK_KNOB_MIN_DEGREE 0
#define GEONKICK_KNOB_RANGE_DEGREE (GEONKICK_KNOB_MAX_DEGREE - GEONKICK_KNOB_MIN_DEGREE)

Knob::Knob(GeonkickWidget *parent)
          : GeonkickWidget(parent)
          , knobValueDegree{GEONKICK_KNOB_MIN_DEGREE}
          , rangeFrom{0}
          , rangeTo{0}
          , rangeType{RangeType::Linear}
          , isSelected{false}
          , defaultValue{0.0}
{
        show();
}

void Knob::setKnobImage(const RkImage &img)
{
        knobImage = img;
}

void Knob::setKnobBackgroundImage(const RkImage &img)
{
        knobBackground = img;
}

void Knob::paintWidget(RkPaintEvent *event)
{
        RK_UNUSED(event);
        RkImage img(80, 80);
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        if (!knobImage.isNull()) {
                painter.drawImage(knobBackground, 0, 0);
                painter.translate(RkPoint(width() / 2, height() / 2));
                painter.rotate((2 * M_PI / 360) * knobValueDegree);
                int x = (width() - knobImage.width()) / 2 - width() / 2;
                int y = (height() - knobImage.height()) / 2 - height() / 2;
                painter.drawImage(knobImage, x, y);
        }

        RkPainter paint(this);
        paint.drawImage(img, 0, 0);
}

void Knob::rotateKnob(int degree)
{
        knobValueDegree += degree;
        if (knobValueDegree < GEONKICK_KNOB_MIN_DEGREE)
                knobValueDegree = GEONKICK_KNOB_MIN_DEGREE;
        else if (knobValueDegree > GEONKICK_KNOB_MAX_DEGREE)
                knobValueDegree = GEONKICK_KNOB_MAX_DEGREE;

        double k = (knobValueDegree - GEONKICK_KNOB_MIN_DEGREE) / GEONKICK_KNOB_RANGE_DEGREE;
        if (getRangeType() == RangeType::Logarithmic) {
                double logVal = log10(rangeFrom) + k * (log10(rangeTo) - log10(rangeFrom));
                valueUpdated(pow(10, logVal));
        } else {
                valueUpdated(rangeFrom + k * (rangeTo - rangeFrom));
        }
}

void Knob::mouseButtonPressEvent(RkMouseEvent *event)
{
        setFocus(true);
        if (event->button() == RkMouseEvent::ButtonType::WheelUp
            || event->button() == RkMouseEvent::ButtonType::WheelDown) {
                rotateKnob(event->button() == RkMouseEvent::ButtonType::WheelUp ? 2 : -2);
                update();
                return;
        }

        if (!knobImage.isNull()) {
                int xCenter = width() / 2;
                int yCenter = height() / 2;
                int r = knobImage.width() / 2;
                if ((event->x() - xCenter) * (event->x() - xCenter) +
                    (event->y() - yCenter) * (event->y() - yCenter) <=  r * r)
                {
                        isSelected = true;
                        lastPositionPoint.setX(event->x());
                        lastPositionPoint.setY(event->y());
                }
        }
}

void Knob::mouseButtonReleaseEvent(RkMouseEvent *event)
{
        RK_UNUSED(event);
        isSelected = false;
}

void Knob::mouseMoveEvent(RkMouseEvent *event)
{
        if (isSelected) {
                int dy = event->y() - lastPositionPoint.y();
                rotateKnob(-dy);
                lastPositionPoint.setX(event->x());
                lastPositionPoint.setY(event->y());
                update();
        }
}

void Knob::mouseDoubleClickEvent(RkMouseEvent *event)
{
        setCurrentValue(defaultValue);
        action valueUpdated(defaultValue);
}

double Knob::valueToDegree(double val)
{
        val = std::clamp(val, rangeFrom, rangeTo);
        double k = 0.0;
        if (std::fabs(rangeTo - rangeFrom) < std::numeric_limits<double>::epsilon()) {
                return GEONKICK_KNOB_MIN_DEGREE;
        } else {
                if (getRangeType() == RangeType::Logarithmic)
                        k = (log10(val) - log10(rangeFrom)) / (log10(rangeTo) - log10(rangeFrom));
                else
                        k = (val - rangeFrom) / (rangeTo - rangeFrom);
        }
        return GEONKICK_KNOB_MIN_DEGREE + k * GEONKICK_KNOB_RANGE_DEGREE;
}

double Knob::getValue(void) const
{
        double k = (knobValueDegree - GEONKICK_KNOB_MIN_DEGREE) / GEONKICK_KNOB_RANGE_DEGREE;
        double val;
        if (getRangeType() == RangeType::Logarithmic) {
                double logVal = log10(rangeFrom) + k * (log10(rangeTo) - log10(rangeFrom));
                val = pow(10, logVal);
        } else {
                val = rangeFrom + k * (rangeTo - rangeFrom);
        }

	return val;
}

void Knob::setRange(double from, double to)
{
        rangeFrom = from;
        rangeTo = to;
}

void Knob::setRangeType(RangeType type)
{
        rangeType = type;
}

Knob::RangeType Knob::getRangeType() const
{
        return rangeType;
}

void Knob::setDefaultValue(double val)
{
        defaultValue = val;
        setCurrentValue(defaultValue);
}

void Knob::setCurrentValue(double val)
{
        knobValueDegree = valueToDegree(val);
        update();
}
