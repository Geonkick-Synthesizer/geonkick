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
#include <QPainter>

class OscillatorWidget;

class OscillatorEnvelope: public QObject
{
  Q_OBJECT

 public:
      enum EnvelopeType {
	      OSC_ENV_FREQUENCY,
	      OSC_ENV_AMPLITUDE
      };
  
	OscillatorEnvelope(OscillatorWidget *parent);
	~OscillatorEnvelope();
	void draw(QPainter &painter);
	void setOrigin(QPointF &point);
	bool hasSelected(void);
	void selectPoint(QPointF point);
	void unselectPoint(void);
	void moveSelectedPoint(double x, double y);
	double getLeftPointLimit(void);
	double getRightPointLimit(void);
	void addPoint(QPointF point);
	void addEnvelopePoints(QPolygonF points);
	void removePoint(QPointF point);
	void setXRatio(double k);
	void setYRatio(double k);
	QPointF scaleUp(QPointF point);
	QPointF scaleDown(QPointF point);
	QPointF getOriginPoint(void);
	double getEnvelopeLenth(void);
	double getEnvelopeHeight(void);
	void removePoints(void);
	double getKickLength(void);
	void setType(OscillatorEnvelope::EnvelopeType type);
	OscillatorEnvelope::EnvelopeType getType(void);
	double getEnvelopeValue(void);
	
 public slots:
	void setEnvelopeValue(double v);

 signals:
	void pointAdded(const QPointF & point);
	void pointRemoved(int index);
	void pointUpdated(int index, const QPointF & point);
	
   
 protected:
	void drawPoints(QPainter &painter);
	void drawLines(QPainter &painter);
     
 private:
	OscillatorWidget *parentWidget;
	QVector<OscillatorEnvelopePoint> envelopePoints;
	OscillatorEnvelopePoint* selectedPoint;
	QPointF originPoint;
	double envelopeW;
	double envelopeH;
	double xRatio;
	double yRatio;
	OscillatorEnvelope::EnvelopeType envelopeType;
	double envelopeValue;
};

#endif
