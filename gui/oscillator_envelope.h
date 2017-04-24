#ifndef GKICK_OSCILLATOR_ENVELOPE_H
#define GKICK_OSCILLATOR_ENVELOPE_H

/**
 * File name: envelope.h
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

#include "envelope_point.h"

#include <QObject>

class OscillatorEnvelope: public QObject
{
  Q_OBJECT

 public:
	enum OutOfRangeType {
		OUT_OF_RANGE_NONE,
		OUT_OF_RANGE_LEFT,
		OUT_OF_RANGE_RIGHT,
		OUT_OF_RANGE_TOP,
		OUT_OF_RANGE_BOTTOM,
	};

	OscillatorEnvelope(void);
	~OscillatorEnvelope();
	void draw(QPainter &painter);
	void setOrigin(QPointF &point);
	void setWidth(double width);
	void setHeight(double height);
	bool hasSelected(void);
	void selectPoint(QPointF point);
	void unselectPoint(void);
	void moveSelectedPoint(double dx, double dy);
	void setOutOfRangeX(OutOfRangeType type);
	void setOutOfRangeY(OutOfRangeType type);
	double getLeftPointLimit(void);
	double getRightPointLimit(void);
	void addPoint(QPointF point);
	void addEnvelopePoints(QPolygonF points);
	void removePoint(QPointF point);
	void setXRatio(double k);
	void setYRatio(double k);
	
 signals:
	void pointAdded(const QPointF & point);
	void pointRemoved(int index);
	void pointUpdated(int index, const QPointF & point);
	
   
 protected:
	void drawPoints(QPainter &painter);
	void drawLines(QPainter &painter);
     
 private:
	QVector<OscillatorEnvelopePoint> envelopePoints;
	OscillatorEnvelopePoint* selectedPoint;
	QPointF originPoint;
	double envelopeW;
	double envelopeH;
	double xRatio;
	double yRatio;
	OutOfRangeType outOfRangeX;
	OutOfRangeType outOfRangeY;
};

#endif
