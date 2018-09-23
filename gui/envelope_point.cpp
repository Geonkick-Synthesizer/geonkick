/**
 * File name: envelope_point.cpp
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

#include "envelope_point.h"
#include "envelope.h"

#include <QDebug>

EnvelopePoint::EnvelopePoint(void)
	: QPointF(),
	  is_selected(false),
	  pointRadius(20),
	  dotRadius(5),
	  parentEnvelope(NULL)

{
}

EnvelopePoint::EnvelopePoint(Envelope *parent, const QPointF &point)
	: QPointF(point),
	  is_selected(false),
	  pointRadius(20),
	  dotRadius(6),
	  parentEnvelope(parent)
{
}

EnvelopePoint::EnvelopePoint(Envelope *parent, double x, double y)
	: QPointF(x, y),
	  is_selected(false),
	  pointRadius(20),
	  dotRadius(6),
	  parentEnvelope(parent)

{
}

EnvelopePoint::~EnvelopePoint()
{
}

void EnvelopePoint::draw(QPainter &painter)
{
	QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(200, 200, 200, 200));
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
	painter.setPen(pen);

        QPointF origin = parentEnvelope->origin();
	QPointF point  = QPointF(origin.x() + x() * (parentEnvelope->W()),
                                 origin.y() - y() * (parentEnvelope->H()));
        double r = radius();
	QRectF rect(point.x() - r / 2, point.y() - r / 2, r, r);
	painter.drawEllipse(rect);

        QBrush brush = painter.brush();
        painter.setBrush(QColor(200, 200, 200, 200));
        r = getDotRadius();
        rect = QRectF(point.x() - r / 2, point.y() - r / 2, r, r);
        painter.drawEllipse(rect);
        painter.setBrush(brush);

        double value = 0;
        if (parentEnvelope->type() == Envelope::Type::Amplitude) {
                value = y() * parentEnvelope->envelopeAmplitude();
                painter.drawText(point.x() + 1.5 * r, point.y() - 1.5 * r,
                                 QString::number(value, 'f', 0) + "dB");
        } else if (parentEnvelope->type() == Envelope::Type::Frequency) {
                value = y() * parentEnvelope->envelopeAmplitude();
                if (value < 1000) {
                        painter.drawText(point.x() + 1.5 * r, point.y() - 1.5 * r,
                                         QString::number(value, 'f', 0)
                                         + "Hz " + frequencyToNote(value));
                } else if (value >= 1000 && value <= 20000) {
                        painter.drawText(point.x() +  1.5 * r, point.y() - 1.5 * r,
                                         QString::number(value / 1000, 'f', 1) + "kHz "
                                         + frequencyToNote(value));
                }
        }
}

double EnvelopePoint::radius(void)
{
	return pointRadius;
}

double EnvelopePoint::getDotRadius(void)
{
	return dotRadius;
}

bool EnvelopePoint::isSelected(void)
{
	return is_selected;
}

void EnvelopePoint::selectPoint(void)
{
	is_selected = true;
}

void EnvelopePoint::unselectPoint(void)
{
	is_selected = false;
}

bool EnvelopePoint::hasPoint(const QPointF &point)
{
	double px = point.x();
	double py = point.y();
        double X = x() * parentEnvelope->W();
        double Y = y() * parentEnvelope->H();

	if ((px > X - pointRadius) && (px < X + pointRadius)
	    && (py > Y - pointRadius) && (py < Y + pointRadius)
	    && ((X - px) * (X - px) + (Y - py) * (Y - py) < pointRadius * pointRadius))	{
                return true;
        }

	return false;
}

// TODO: more precise funtion...
QString EnvelopePoint::frequencyToNote(double f)
{
        if (f < 16.35160 || f > 7902.133) {
                return QString();
        }

        int n = 0;
        while (f > 32.70320) {
                f /=2;
                n++;
        }

        int octave = n;
        std::vector<double> pitches {
                        16.35160,
                        17.32391,
                        18.35405,
                        19.44544,
                        20.60172,
                        21.82676,
                        23.12465,
                        24.49971,
                        25.95654,
                        27.50000,
                        29.13524,
                        30.86771};

        std::vector<QString> notes{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

        n = 12;
        while (--n && pitches[n] > f);
        if (n < 11 && f > (pitches[n + 1] - pitches[n]) / 2) {
                n++;
        }

        return "(" + notes[n] + QString::number(octave) + ")";
}
