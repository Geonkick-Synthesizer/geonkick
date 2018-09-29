/**
 * File name: envelope_point.h
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

#ifndef GKICK_ENVELOPE_POINT_UI_H
#define GKICK_ENVELOPE_POINT_UI_H

#include <QPoint>
#include <QPainter>

#include "globals.h"

class Envelope;

class EnvelopePoint : public QPoint
{
 public:
	EnvelopePoint(void);
	EnvelopePoint(Envelope *parent, const QPoint &point);
	EnvelopePoint(Envelope *parent, int x, int y);
	~EnvelopePoint();
	void draw(QPainter &painter);
	bool isSelected(void);
	bool hasPoint(const QPoint &point);
	void selectPoint(void);
	void unselectPoint(void);
	int radius(void);
	int getDotRadius(void);
	QString valueType(void);

 protected:
        void drawPointValue(QPainter &painter);

 private:
        QString frequencyToNote(double f);
	bool is_selected;
	int pointRadius;
	int dotRadius;
	Envelope *parentEnvelope;
};

#endif
