/**
 * File name: gkick_envelope.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#ifndef GKICK_ENVELOPE_H
#define GKICK_ENVELOPE_H

#include "envelope_point.h"

#include <QObject>
#include <QPainter>

#include <memory>

class GKickEnvelope: public QObject
{
        Q_OBJECT

 public:

        enum EnvelopeType {
                ENV_TYPE_AMPLITUDE  = 0,
                ENV_TYPE_FREQUENCY  = 1,
        };

        enum  EnvelopeCategory {
                ENV_CATEGORY_OSC_1   = 0,
                ENV_CATEGORY_OSC_2   = 1,
                ENV_CATEGORY_NOISE   = 2,
                ENV_CATEGORY_GENERAL = 3
        };

        GKickEnvelope();
        virtual ~GKickEnvelope();
        int W(void) const;
        int H(void) const;
        virtual double envelopeLengh(void) const { return 0.0; }
        QPointF origin(void) const;
        void draw(QPainter &painter);
        void drawAxes(QPainter &painter);
        bool hasSelected() const;
        void selectPoint(const QPointF &point);
        void unselectPoint(void);
        void moveSelectedPoint(double x, double y);
        double getLeftPointLimit(void) const;
        double getRightPointLimit(void) const;
        void addPoint(QPointF point);
        void removePoint(QPointF point);
        void setType(EnvelopeType type);
        EnvelopeType type() const;
        void setCategory(EnvelopeCategory cat);
        EnvelopeCategory category() const;
        double getEnvelopeValue(void) const;
        QString name() const;

 protected:
        virtual void pointAddedEvent(double x, double y) {}
        virtual void pointUpdatedEvent(unsigned int index, double x, double y) {}
        virtual void pointRemovedEvent(unsigned int index) {}
        void drawPoints(QPainter &painter);
        void drawLines(QPainter &painter);

 private:
        std::vector<std::shared_ptr<GKickEnvelopePoint>> envelopePoints;
        int selectedPointIndex;
        EnvelopeType envelopeType;
        EnvelopeCategory envelopeCategory;
        QString envelopeName;
        double envelopeW;
        double envelopeH;
};

#endif
