/**
 * File name: envelope.cpp
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

#include "envelope.h"

GKickEnvelope::GKickEnvelope(void) :
	envelopePoints(),
	selectedPoint(NULL),
	originPoint(0.0, 0.0),
	envelopeW(0.0),
	envelopeH(0.0),
	outOfRangeX(GKickEnvelope::OUT_OF_RANGE_NONE),
	outOfRangeY(GKickEnvelope::OUT_OF_RANGE_NONE)
{
	envelopePoints << GKickEnvelopePoint(QPointF(0.0, 10.0));
	envelopePoints << GKickEnvelopePoint(QPointF(50.0, 50.0));
	envelopePoints << GKickEnvelopePoint(QPointF(100.0, 120.0));
	envelopePoints << GKickEnvelopePoint(QPointF(150.0, 200.0));
	envelopePoints << GKickEnvelopePoint(QPointF(250.0, 20.0));
	envelopePoints << GKickEnvelopePoint(QPointF(270.0, 10.0));
	envelopePoints << GKickEnvelopePoint(QPointF(300.0, 50.0));
	envelopePoints << GKickEnvelopePoint(QPointF(320.0, 120.0));
	envelopePoints << GKickEnvelopePoint(QPointF(350.0, 200.0));
	envelopePoints << GKickEnvelopePoint(QPointF(380.0, 20.0));

}

GKickEnvelope::~GKickEnvelope()
{
}

void GKickEnvelope::draw(QPainter &painter)
{
	drawPoints(painter);
	drawLines(painter);
}

void GKickEnvelope::drawPoints(QPainter &painter)
{
	for (int i = 0; i < envelopePoints.size(); i++) {
		envelopePoints[i].draw(painter, originPoint);
	}
}

void GKickEnvelope::drawLines(QPainter &painter)
{
	QPolygonF points;
	for (int i = 0; i < envelopePoints.size(); i++) {
		points << QPointF(originPoint.x() + envelopePoints[i].x(),
				  originPoint.y() - envelopePoints[i].y());
	}

	QPen pen = painter.pen();
	pen.setWidth(1);
	painter.setPen(pen);
	painter.drawPolyline(points);
}

void GKickEnvelope::setOrigin(QPointF &point)
{
	originPoint = point;
}

void GKickEnvelope::setOutOfRangeX(GKickEnvelope::OutOfRangeType type)
{
	outOfRangeX = type;
}

void GKickEnvelope::setOutOfRangeY(GKickEnvelope::OutOfRangeType type)
{
	outOfRangeY = type;
}

void GKickEnvelope::setWidth(double width)
{
	envelopeW = width;
}

void GKickEnvelope::setHeight(double height)
{
	envelopeH = height;
}

bool GKickEnvelope::hasSelected(void)
{
	return selectedPoint != NULL;
}

void GKickEnvelope::selectPoint(QPointF point)
{
	outOfRangeX = GKickEnvelope::OUT_OF_RANGE_NONE;
	outOfRangeY = GKickEnvelope::OUT_OF_RANGE_NONE;

	for(int i = 0; i < envelopePoints.size(); i++) {
		if (envelopePoints[i].hasPoint(point)) {
			selectedPoint = &envelopePoints[i];
			selectedPoint->selectPoint();
			break;
		}
		
	}
}

void GKickEnvelope::unselectPoint(void)
{
	if (selectedPoint) {
		selectedPoint->unselectPoint();
		selectedPoint = NULL;
	}
	
	outOfRangeX = outOfRangeY = GKickEnvelope::OUT_OF_RANGE_NONE;
}

double GKickEnvelope::getLeftPointLimit(void)
{
	if (selectedPoint == NULL) {
		return 0.0;
	}

	double x = 0.0;
	int index = envelopePoints.indexOf(*selectedPoint);
	if (index == -1) {
		x = 0.0;
	} else if (envelopePoints[index] == envelopePoints.first()) {
		x = 0.0;
	} else {
		x = envelopePoints[index - 1].x();
	}

	return x;
}

double GKickEnvelope::getRightPointLimit(void)
{
	if (selectedPoint == NULL) {
		return 0.0;
	}

	double x = 0.0;
	int index = envelopePoints.indexOf(*selectedPoint);
	if (index == -1) {
		x = 0.0;
	} else if (envelopePoints[index] == envelopePoints.last()) {
		x = envelopeW;
	} else {
		x = envelopePoints[index + 1].x();
	}

	return x;
}

void GKickEnvelope::moveSelectedPoint(double dx, double dy)
{
	if (selectedPoint == NULL) {
		return;
	}

	if (outOfRangeX == GKickEnvelope::OUT_OF_RANGE_LEFT) {
		selectedPoint->setX(getLeftPointLimit());
	} else if (outOfRangeX == GKickEnvelope::OUT_OF_RANGE_RIGHT) {
		selectedPoint->setX(getRightPointLimit());
	} else {
		selectedPoint->setX(selectedPoint->x() + dx);
	}

	if (outOfRangeY == GKickEnvelope::OUT_OF_RANGE_BOTTOM) {
		selectedPoint->setY(0.0);
	} else if (outOfRangeY == GKickEnvelope::OUT_OF_RANGE_TOP) {
		selectedPoint->setY(envelopeH);
	} else {
		selectedPoint->setY(selectedPoint->y() + dy);
	}
}

void GKickEnvelope::addPoint(QPointF point)
{
	if (point.x() > envelopeW) {
		envelopePoints.append(GKickEnvelopePoint(envelopeW, point.y()));
	} else if (point.x() < 0.0) {
		envelopePoints.push_front(GKickEnvelopePoint(0.0, point.y()));
	} else if (point.x() < envelopePoints[0].x()) {
		envelopePoints.push_front(GKickEnvelopePoint(point));
	} else if (point.x() > envelopePoints.last().x()) {
		envelopePoints.push_back(GKickEnvelopePoint(point));
	} else {		
		GKickEnvelopePoint p;
		for(int i = 0; i < envelopePoints.size(); i++) {
			if (point.x() < envelopePoints[i].x()) {
				envelopePoints.insert(i, GKickEnvelopePoint(point));
				break;
			}
		}
	}
}
