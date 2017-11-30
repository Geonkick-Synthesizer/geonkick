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

GKickKnob::GKickKnob(QWidget *parent, const QString &name)
	: QWidget(parent),
          knobName(name),
	  knobRadius(GKICK_UI_DEFAULT_KNOB_RADIOUS),
	  lastPositionPoint(),
	  knobValueDegree(0),
	  realValue(0.0),
          isSelected(false)

{
        setFixedSize(2 * knobRadius, 2 * knobRadius);
        setContentsMargins(2, 2, 2, 2);
}

GKickKnob::~GKickKnob()
{

}

void
GKickKnob::paintEvent(QPaintEvent *event)
{
        Q_UNUSED(event);
        QPainter painter(this);

        QRect rect;
        painter.setBrush(Qt::white);
        rect.setCoords(2, 2, width() - 2, height() - 2);
        painter.drawEllipse(rect);

        painter.setBrush(Qt::gray);

        rect.setCoords(2, 2, width() - 2, height() - 2);
        painter.drawPie(rect, (90) * 16, - knobValueDegree * 16);

        painter.setBrush(Qt::white);
        rect.setCoords(8, 8, width() - 8, height() - 8);
        painter.drawEllipse(rect);
}

int
GKickKnob::getRadius(void)
{
        return knobRadius;
}

int
GKickKnob::getWidth(void)
{
        return 2 * knobRadius;
}

int
GKickKnob::getHeight(void)
{
        return 2 * knobRadius;
}

void
GKickKnob::mousePressEvent(QMouseEvent *event)
{
        isSelected = true;
        lastPositionPoint.setX(event->x());
        lastPositionPoint.setY(event->y());
}

void
GKickKnob::mouseReleaseEvent(QMouseEvent *event)
{
        isSelected = false;
}

void
GKickKnob::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void
GKickKnob::mouseMoveEvent(QMouseEvent *event)
{
        if (isSelected) {
                int dy = event->y() - lastPositionPoint.y();
                knobValueDegree += dy;
                if (knobValueDegree < 0) {
                        knobValueDegree = 0;
                } else if (knobValueDegree > 360) {
                        knobValueDegree = 360;
                }
                lastPositionPoint.setX(event->x());
                lastPositionPoint.setY(event->y());
                emit valueUpdated(realValue * (double)knobValueDegree / 360.0);
                update();
        }
}

void GKickKnob::setPosition(double v)
{
        knobValueDegree = v * 360.0;
        if (knobValueDegree > 360.0) {
                knobValueDegree = 360.0;
        } else if(knobValueDegree < 0.0) {
                knobValueDegree = 0.0;
        }
        emit valueUpdated(realValue * (double)knobValueDegree / 360.0);
}

double GKickKnob::getPosition(void)
{
	return (double)knobValueDegree / 360.0;
}

void GKickKnob::setValue(double v)
{
	realValue = v;
}

double GKickKnob::getValue(void)
{
	return realValue * ((double)knobValueDegree / 360.0);
}

void GKickKnob::resizeEvent(QResizeEvent *event)
{
        Q_UNUSED(event);
}

void GKickKnob::setMaxValue(double val)
{
}

void GKickKnob::setCurrentValue(double val)
{
}

