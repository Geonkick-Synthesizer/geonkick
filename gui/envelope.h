/**
 * File name: envelope.h
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

#ifndef GEONKICK_ENVELOPE_H
#define GEONKICK_ENVELOPE_H

#include "envelope_point.h"

#include <QObject>
#include <QPainter>

#include <memory>
#include <unordered_set>

class Envelope: public QObject
{
        Q_OBJECT

 public:

        enum class EnvelopeType:int {
                Aplitude  = 0,
                Frequency = 1
        };

        Envelope();
        virtual ~Envelope();
        int W(void) const;
        int H(void) const;
        virtual double envelopeLengh(void) const { return 0;}
        QPointF origin(void) const;
        void draw(QPainter &painter, const QRectF &rect);
        bool hasSelected() const;
        void selectPoint(const QPointF &point);
        void unselectPoint(void);
        void moveSelectedPoint(double x, double y);
        double getLeftPointLimit(void) const;
        double getRightPointLimit(void) const;
        void addPoint(const QPointF &point);
        virtual void updatePoints() {};
        void removePoint(QPointF point);
        EnvelopeType type() const;
        double getEnvelopeValue(void) const;
        bool isSupportedType(Envelope::EnvelopeType type) const;

 public slots:
         virtual void setEnvelopeLengh(double len) {};
         bool setType(EnvelopeType type);
         void addSupportedType(Envelope::EnvelopeType type);
         void removeSupportedType(Envelope::EnvelopeType type);
         void setPoints(const QPolygonF  &points);
         void removePoints();
 signals:
         void envelopeLengthUpdated(double len);

 protected:
        virtual void pointAddedEvent(double x, double y) {};
        virtual void pointUpdatedEvent(unsigned int index, double x, double y) {};
        virtual void pointRemovedEvent(unsigned int index) {};
        void drawScale(QPainter &painter);
        void drawPoints(QPainter &painter);
        void drawLines(QPainter &painter);

 private:
        std::vector<std::shared_ptr<EnvelopePoint>> envelopePoints;
        std::vector<std::shared_ptr<EnvelopePoint>>::size_type selectedPointIndex;
        std::unordered_set<EnvelopeType> supportedTypes;
        bool pointSelected;
        EnvelopeType envelopeType;
        QPointF envelopeOrigin;
        double envelopeW;
        double envelopeH;
};

#endif // GEONKICK_ENVELOPE_H
