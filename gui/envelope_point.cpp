/**
 * File name: envelope_point.cpp
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

#include "envelope_point.h"
#include "gkick_envelope.h"

#include <QDebug>

GKickEnvelopePoint::GKickEnvelopePoint(void)
	: QPointF(),
	  is_selected(false),
	  pointRadius(7),
	  dotRadius(3),
	  parentEnvelope(NULL)

{
}

GKickEnvelopePoint::GKickEnvelopePoint(GKickEnvelope *parent,
						 const QPointF &point)
	: QPointF(point),
	  is_selected(false),
	  pointRadius(7),
	  dotRadius(3),
	  parentEnvelope(parent)
{
}

GKickEnvelopePoint::GKickEnvelopePoint(GKickEnvelope *parent,
						 double x, double y)
	: QPointF(x, y),
	  is_selected(false),
	  pointRadius(7),
	  dotRadius(3),
	  parentEnvelope(parent)

{
}

GKickEnvelopePoint::~GKickEnvelopePoint()
{
}

void GKickEnvelopePoint::draw(QPainter &painter)
{
	QPen pen;
	if (isSelected()) {
		pen.setWidth(2);
	} else {
		pen.setWidth(1);
	}

	painter.setPen(pen);
	painter.setBrush(Qt::white);
	QPointF point = scaleUp(QPointF(x(), y()));
	QPointF origin = parentEnvelope->getOriginPoint();
	QRectF rect;
	rect.setCoords(origin.x() + (point.x() - radius()),
		       origin.y() - (point.y() - radius()),
		       origin.x() + (point.x() + radius()),
		       origin.y() - (point.y() + radius()));
	painter.drawEllipse(rect);
	painter.drawText(origin.x() + (point.x() + 3 * radius() / 3),
			 origin.y() - (point.y() + 3 * radius() / 3), pointText());

	painter.setBrush(Qt::black);
	rect.setCoords(origin.x() + (point.x() - radius() / 3),
		       origin.y() - (point.y() - radius() / 3),
		       origin.x() + (point.x() + radius() / 3),
		       origin.y() - (point.y() + radius() / 3));
	painter.drawEllipse(rect);
}

QPointF GKickEnvelopePoint::scaleUp(QPointF point)
{
  return parentEnvelope->scaleUp(point);
}

double GKickEnvelopePoint::radius(void)
{
	return pointRadius;
}

double GKickEnvelopePoint::getDotRadius(void)
{
	return dotRadius;
}

bool GKickEnvelopePoint::isSelected(void)
{
	return is_selected;
}

void GKickEnvelopePoint::selectPoint(void)
{
	is_selected = true;
}

void GKickEnvelopePoint::unselectPoint(void)
{
	is_selected = false;
}

bool GKickEnvelopePoint::hasPoint(const QPointF &point)
{
	double px = point.x();
	double py = point.y();
	QPointF p = scaleUp(QPointF(x(), y()));

	if ((px > p.x() - pointRadius) && (px < p.x() + pointRadius)
	    && (py > y() - pointRadius) && (py < p.y() + pointRadius)
	    && ((p.x() - px) * (p.x() - px) + (p.y() - py) * (p.y() - py) < pointRadius * pointRadius))	{
                return true;
        }

	return false;
}

QString GKickEnvelopePoint::pointText(void)
{
	if (parentEnvelope->type() == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
		return pointAmplitudeText();
	} else if (parentEnvelope->type() == GKickEnvelope::ENV_TYPE_FREQUENCY) {
		return pointFrequencyText();
	}

	return "";
}

QString GKickEnvelopePoint::pointAmplitudeText(void)
{
	QString text = QString::number(parentEnvelope->getEnvelopeValue() * y(), 'f', 3);
	return text;
}

QString GKickEnvelopePoint::pointFrequencyText(void)
{
	double v = parentEnvelope->getEnvelopeValue() * y();

	QString textVal;
	QString str;
	if (v > 1000.0) {
		textVal = QString::number(v / 1000.0, 'f', 3) + " kHz";
	} else {
		textVal = QString::number(v, 'f', 0) + " Hz";
	}

	return textVal;
}

QString GKickEnvelopePoint::getTimeTextValue(void)
{
	return QString::number((parentEnvelope->getKickLength() * x()) * 1000, 'f', 0) + " ms";
}

