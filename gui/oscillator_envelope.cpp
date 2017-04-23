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

#include "oscillator_envelope.h"

OscillatorEnvelope::OscillatorEnvelope(void) :
	envelopePoints(),
	selectedPoint(NULL),
	originPoint(0.0, 0.0),
	envelopeW(0.0),
	envelopeH(0.0),
	outOfRangeX(OscillatorEnvelope::OUT_OF_RANGE_NONE),
	outOfRangeY(OscillatorEnvelope::OUT_OF_RANGE_NONE)
{
}

OscillatorEnvelope::~OscillatorEnvelope()
{
}

void OscillatorEnvelope::draw(QPainter &painter)
{
	drawPoints(painter);
	drawLines(painter);
}

void OscillatorEnvelope::drawPoints(QPainter &painter)
{
	for (int i = 0; i < envelopePoints.size(); i++) {
		envelopePoints[i].draw(painter, originPoint);
	}
}

void OscillatorEnvelope::drawLines(QPainter &painter)
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

void OscillatorEnvelope::setOrigin(QPointF &point)
{
	originPoint = point;
}

void OscillatorEnvelope::setOutOfRangeX(OscillatorEnvelope::OutOfRangeType type)
{
	outOfRangeX = type;
}

void OscillatorEnvelope::setOutOfRangeY(OscillatorEnvelope::OutOfRangeType type)
{
	outOfRangeY = type;
}

void OscillatorEnvelope::setWidth(double width)
{
	envelopeW = width;
}

void OscillatorEnvelope::setHeight(double height)
{
	envelopeH = height;
}

bool OscillatorEnvelope::hasSelected(void)
{
	return selectedPoint != NULL;
}

void OscillatorEnvelope::selectPoint(QPointF point)
{
	outOfRangeX = OscillatorEnvelope::OUT_OF_RANGE_NONE;
	outOfRangeY = OscillatorEnvelope::OUT_OF_RANGE_NONE;

	for(int i = 0; i < envelopePoints.size(); i++) {
		if (envelopePoints[i].hasPoint(point)) {
			selectedPoint = &envelopePoints[i];
			selectedPoint->selectPoint();
			break;
		}
		
	}
}

void OscillatorEnvelope::unselectPoint(void)
{
	if (selectedPoint) {
		selectedPoint->unselectPoint();
		selectedPoint = NULL;
	}
	
	outOfRangeX = outOfRangeY = OscillatorEnvelope::OUT_OF_RANGE_NONE;
}

double OscillatorEnvelope::getLeftPointLimit(void)
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

double OscillatorEnvelope::getRightPointLimit(void)
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

void OscillatorEnvelope::moveSelectedPoint(double dx, double dy)
{
	if (selectedPoint == NULL) {
		return;
	}

	if (outOfRangeX == OscillatorEnvelope::OUT_OF_RANGE_LEFT) {
		selectedPoint->setX(getLeftPointLimit());
	} else if (outOfRangeX == OscillatorEnvelope::OUT_OF_RANGE_RIGHT) {
		selectedPoint->setX(getRightPointLimit());
	} else {
		selectedPoint->setX(selectedPoint->x() + dx);
	}

	if (outOfRangeY == OscillatorEnvelope::OUT_OF_RANGE_BOTTOM) {
		selectedPoint->setY(0.0);
	} else if (outOfRangeY == OscillatorEnvelope::OUT_OF_RANGE_TOP) {
		selectedPoint->setY(envelopeH);
	} else {
		selectedPoint->setY(selectedPoint->y() + dy);
	}

	int index = envelopePoints.indexOf(*selectedPoint);
	if (index > -1 && index < envelopePoints.size()) {
	  emit pointUpdated(index, QPointF(selectedPoint->x(), selectedPoint->y()));
	}
}

void OscillatorEnvelope::addEnvelopePoints(const QPolygonF &points)
{
  for (int i = 0; i < 10/*points.size()*/; i++) {
    envelopePoints << OscillatorEnvelopePoint(QPointF(100.0,100.0)/*points[i]*/);
  }
}

void OscillatorEnvelope::addPoint(QPointF point)
{
  bool added = false;
  
	if (point.x() > envelopeW) {
		envelopePoints.append(OscillatorEnvelopePoint(envelopeW, point.y()));
		added = true;
	} else if (point.x() < 0.0) {
		envelopePoints.push_front(OscillatorEnvelopePoint(0.0, point.y()));
		added = true;
	} else if (point.x() < envelopePoints[0].x()) {
		envelopePoints.push_front(OscillatorEnvelopePoint(point));
		added = true;
	} else if (point.x() > envelopePoints.last().x()) {
		envelopePoints.push_back(OscillatorEnvelopePoint(point));
		added = true;
	} else {		
		OscillatorEnvelopePoint p;
		for(int i = 0; i < envelopePoints.size(); i++) {
			if (point.x() < envelopePoints[i].x()) {
				envelopePoints.insert(i, OscillatorEnvelopePoint(point));
				added = true;
				break;
			}
		}
	}

	if (added) {
	  emit pointAdded(point);
	}
}

void OscillatorEnvelope::removePoint(QPointF point)
{
	for(int i = 0; i < envelopePoints.size(); i++) {
		if (envelopePoints[i].hasPoint(point)) {
			if (envelopePoints[i] != envelopePoints.first()
			    && envelopePoints[i] != envelopePoints.last()) {
				envelopePoints.remove(i);
				emit pointRemoved(i);
			}
			break;
		}
		
	}	
}
