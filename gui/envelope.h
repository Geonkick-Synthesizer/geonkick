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

#include "geonkick_api.h"

#include <QObject>
#include <QPainter>

#include <memory>
#include <unordered_set>

class Envelope: public QObject
{
        Q_OBJECT

 public:

        using Type = GeonkickApi::EnvelopeType;

        Envelope(QObject *parent = nullptr, const QRect &area = QRect());
        virtual ~Envelope();
        int W(void) const;
        int H(void) const;
        virtual double envelopeLengh(void) const { return 0;}
        virtual double envelopeAmplitude(void) const { return 0;}
        QPoint getOrigin(void) const;
        void draw(QPainter &painter);
        bool hasSelected() const;
        void selectPoint(const QPoint &point);
        void unselectPoint(void);
        void moveSelectedPoint(int x, int y);
        void addPoint(const QPoint &point);
        virtual void updatePoints() {};
        void removePoint(const QPoint &point);
        Type type() const;
        bool isSupportedType(Type type) const;
        const QRect& getDrawingArea();

 public slots:
         virtual void setEnvelopeLengh(double len) {}
         bool setType(Type type);
         void addSupportedType(Type type);
         void removeSupportedType(Type type);
         void setPoints(const QPolygonF  &points);
         void removePoints();
         void setDrawingArea(const QRect &rect);
 signals:
         void envelopeLengthUpdated(double len);
         void amplitudeUpdated(double amplitude);
         void envelopeUpdated();

 protected:
        virtual void pointAddedEvent(double x, double y) = 0;
        virtual void pointUpdatedEvent(unsigned int index, double x, double y) = 0;
        virtual void pointRemovedEvent(unsigned int index)  = 0;
        void drawAxies(QPainter &painter);
        void drawScale(QPainter &painter);
        void drawTimeScale(QPainter &painter);
        void drawValueScale(QPainter &painter);
        void drawPoints(QPainter &painter);
        void drawPoint(QPainter &painter, const QPoint &point);
        void drawPointValue(QPainter &painter, const QPoint &point, double value);
        void drawLines(QPainter &painter);
        QPointF scaleDown(const QPoint &point);
        QPoint scaleUp(const QPointF &point);
        bool hasPoint(const QPointF &point, const QPoint &p);
        int getPointRadius() const;
        void setPointRadius(int radius);
        int getDotRadius()  const;
        void setDotRadius(int radius);
        double getLeftPointLimit(void) const;
        double getRightPointLimit(void) const;
        QString frequencyToNote(double f);

 private:
        QRect drawingArea;
        std::vector<QPointF> envelopePoints;
        int pointRadius;
        int dotRadius;
        std::vector<QPointF>::size_type selectedPointIndex;
        std::unordered_set<Type> supportedTypes;
        bool pointSelected;
        Type envelopeType;
};

#endif // GEONKICK_ENVELOPE_H
