#ifndef GKICK_ENVELOPE_UI_H
#define GKICK_ENVELOPE_UI_H

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

class GKickEnvelope
{
 public:
	enum OutOfRangeType {
		OUT_OF_RANGE_NONE,
		OUT_OF_RANGE_LEFT,
		OUT_OF_RANGE_RIGHT,
		OUT_OF_RANGE_TOP,
		OUT_OF_RANGE_BOTTOM,
	};

	GKickEnvelope(void);
	~GKickEnvelope();
	void draw(QPainter &painter);
	void setOrigin(QPointF &point);
	void setWidth(double width);
	void setHeight(double height);
	bool hasSelected(void);
	void selectPoint(QPointF point);
	void unselectPoint(void);
	void moveSelectedPoint(double dx, double dy);
	void setOutOfRangeX(GKickEnvelope::OutOfRangeType type);
	void setOutOfRangeY(GKickEnvelope::OutOfRangeType type);
	double getLeftPointLimit(void);
	double getRightPointLimit(void);
	void addPoint(QPointF point);
   
 protected:
	void drawPoints(QPainter &painter);
	void drawLines(QPainter &painter);
     
 private:
	QVector<GKickEnvelopePoint> envelopePoints;
	GKickEnvelopePoint* selectedPoint;
	QPointF originPoint;
	double envelopeW;
	double envelopeH;
	GKickEnvelope::OutOfRangeType outOfRangeX;
	GKickEnvelope::OutOfRangeType outOfRangeY;
};

#endif
