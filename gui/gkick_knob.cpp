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

GKickKnob::GKickKnob(GeonkickWidget *parent, const QString &name)
          : GeonkickWidget(parent),
          knobName(name),
	  knobRadius(GKICK_UI_DEFAULT_KNOB_DIAMETER),
	  lastPositionPoint(),
	  knobValueDegree(0),
	  realValue(0.0),
          isSelected(false)

{
        setObjectName("GKickKnob");
        setFixedSize(GKICK_UI_DEFAULT_KNOB_DIAMETER, GKICK_UI_DEFAULT_KNOB_DIAMETER);
}

GKickKnob::~GKickKnob()
{

}

void
GKickKnob::paintEvent(QPaintEvent *event)
{
        Q_UNUSED(event);
        QPainter painter(this);
        QStyleOption opt;
        opt.init(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	QPixmap p;
	p.load("./themes/geontime/knob.png");
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
        painter.translate(GKICK_UI_DEFAULT_KNOB_DIAMETER/2, GKICK_UI_DEFAULT_KNOB_DIAMETER/2);
	painter.rotate(knobValueDegree);
	int x = (GKICK_UI_DEFAULT_KNOB_DIAMETER - p.size().width()) / 2 - GKICK_UI_DEFAULT_KNOB_DIAMETER / 2;
	int y = (GKICK_UI_DEFAULT_KNOB_DIAMETER - p.size().height()) / 2 - GKICK_UI_DEFAULT_KNOB_DIAMETER / 2;
	painter.drawPixmap(x, y, p.size().width(), p.size().height(), p);
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

