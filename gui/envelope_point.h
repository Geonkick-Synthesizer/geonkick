#ifndef GKICK_ENVELOPE_POINT_UI_H
#define GKICK_ENVELOPE_POINT_UI_H

/**
 * File name: envelope_point.h
 * Project: GeonKick
 *
 * Copyright (C) 2015 Iurie Nistor
 * Email: nistor@iurie.org
 *
 * This file is part of GeonKick.
 *
 * waveToMp3 is free software; you can redistribute it and/or modify
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

#include <QPointF>
#include <QPainter>

class OscillatorEnvelope;

class OscillatorEnvelopePoint : public QPointF
{
 public:
	OscillatorEnvelopePoint(void);
	OscillatorEnvelopePoint(OscillatorEnvelope *parent, const QPointF &point);
	OscillatorEnvelopePoint(OscillatorEnvelope *parent, double x, double y);
	virtual ~OscillatorEnvelopePoint();
	void draw(QPainter &painter);
	bool isSelected(void);
	bool hasPoint(const QPointF &point);
	void selectPoint(void);
	void unselectPoint(void);
	double radius(void);
	double getDotRadius(void);
	
 protected:
	QPointF scaleUp(QPointF point);
	
 private:
	bool is_selected;
	double pointRadius;
	double dotRadius;
	OscillatorEnvelope *parentEnvelope;
};

#endif
