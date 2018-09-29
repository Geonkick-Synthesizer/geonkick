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
	: QPoint(0, 0),
	  is_selected(false),
	  pointRadius(10),
	  dotRadius(3),
	  parentEnvelope(NULL)

{
}

EnvelopePoint::EnvelopePoint(Envelope *parent, const QPoint &point)
	: QPoint(point),
	  is_selected(false),
	  pointRadius(10),
	  dotRadius(3),
	  parentEnvelope(parent)
{
}

EnvelopePoint::EnvelopePoint(Envelope *parent, int x, int y)
	: QPoint(x, y),
	  is_selected(false),
	  pointRadius(10),
	  dotRadius(3),
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

        QPoint origin = parentEnvelope->origin();
        double d = 2 * radius();
	QRect rect(origin.x() + x() - d / 2, origin.y() - y() - d /2, d, d);
	painter.drawEllipse(rect);

        QBrush brush = painter.brush();
        painter.setBrush(QColor(200, 200, 200, 200));
        d = 2 * getDotRadius();
        rect = QRect(origin.x() + x() - d / 2, origin.y() - y() - d / 2, d, d);
        painter.drawEllipse(rect);
        painter.setBrush(brush);
        drawPointValue(painter);
}

void EnvelopePoint::drawPointValue(QPainter &painter)
{
        if (parentEnvelope->type() == Envelope::Type::Amplitude) {
                double d = 2 * radius();
                QPoint origin = parentEnvelope->origin();
                QPoint point(origin.x() + x(), origin.y() - y() - 0.7 * d);
                painter.drawText(point, QString::number((double)y() / parentEnvelope->H(), 'f', 1));
        } else if (parentEnvelope->type() == Envelope::Type::Frequency) {
                QPoint origin = parentEnvelope->origin();
                double logVal = ((double)y() / parentEnvelope->H()) * (log10(parentEnvelope->envelopeAmplitude()) - log10(20)) + log10(20);
                double linearVal = pow(10, logVal);
                double d = 2 * radius();
                QPoint point(origin.x() + x(), origin.y() - y() - 0.7 *  d);
                if (linearVal < 1000) {
                        painter.drawText(point, QString::number(linearVal, 'f', 0)
                                         + "Hz " + frequencyToNote(linearVal));
                } else if (linearVal >= 1000 && linearVal <= 20000) {
                        painter.drawText(point, QString::number(linearVal / 1000, 'f', 1) + "kHz "
                                         + frequencyToNote(linearVal));
                }
        }
}

int EnvelopePoint::radius(void)
{
	return pointRadius;
}

int EnvelopePoint::getDotRadius(void)
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

bool EnvelopePoint::hasPoint(const QPoint &point)
{
	if ((point.x() > x() - pointRadius) && (point.x() < x() + pointRadius)
	    && (point.y() > y() - pointRadius) && (point.y() < y() + pointRadius)
	    && pow(x() - point.x(), 2) + pow(y() - point.y(), 2) < pow(pointRadius, 2)) {
                return true;
        }

	return false;
}

// TODO: more precise funtion... move this function out of here
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
