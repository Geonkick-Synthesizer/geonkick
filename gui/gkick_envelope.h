#ifndef GKICK_ENVELOPE_H
#define GKICK_ENVELOPE_H

/**
 * File name: gkick_envelope.h
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

#include <memory>

class GKickEnvelope: public QObject
{
  Q_OBJECT

 public:

        enum EnvelopeType {
                ENV_TYPE_AMPLITUDE = 0,
                ENV_TYPE_FREQUENCY = 1
        };


	GKickEnvelope();
	virtual ~GKickEnvelope();
        void update();
        bool hasSelected();
        void selectPoint(QPointF point);
        void unselectPoint(void);
        void moveSelectedPoint(double x, double y);
        double getLeftPointLimit(void);
        double getRightPointLimit(void);
        void addPoint(QPointF point);
        void removePoint(QPointF point);
        void setXRatio(double k);
        void setYRatio(double k);
        QPointF scaleUp(QPointF point);
        QPointF scaleDown(QPointF point);
        QPointF getOriginPoint(void);
        double getEnvelopeLenth(void);
        double getEnvelopeHeight(void);
        double getKickLength(void);
        void setType(EnvelopeType type);
        EnvelopeType getType();
        double getEnvelopeValue(void);

 protected:
        virtual void pointAddedEvent(double x, double y) {}
        virtual void pointUpdatedEvent(unsigned int index, double x, double y) {}
        virtual void pointRemovedEvent(unsigned int index) {}
        void addEnvelopePoints(QPolygonF points);
        void removePoints(void);
        void drawPoints(QPainter &painter);
        void drawLines(QPainter &painter);

 private:

        bool isActive;
        QVector<std::shared_ptr<GKickEnvelopePoint>> envelopePoints;
        std::shared_ptr<GKickEnvelopePoint> selectedPoint;
        EnvelopeType envelopeType;
};

#endif
