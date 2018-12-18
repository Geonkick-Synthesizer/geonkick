/**
 * File name: envelope_draw_area.h
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

#ifndef ENVELOPE_DRAW_AREA_H
#define ENVELOPE_DRAW_AREA_H

#include "globals.h"
#include "geonkick_widget.h"

#include <memory>

class Envelope;
class KickGraph;

class EnvelopeWidgetDrawingArea : public GeonkickWidget
{
   Q_OBJECT

 public:
   EnvelopeWidgetDrawingArea(GeonkickWidget *parent);
   ~EnvelopeWidgetDrawingArea();
   void paintWidget(QPaintEvent *event) override;
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseDoubleClickEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   std::shared_ptr<Envelope> getEnvelope() const;
   const QRect getDrawingArea();
   void setKickGraph(KickGraph *graph);
   KickGraph* getKickGraph();

   public slots:
           void setEnvelope(std::shared_ptr<Envelope> &envelope);
           void envelopeUpdated();

 private:
   std::shared_ptr<Envelope> currentEnvelope;
   KickGraph *kickGraph;
   QRect drawingArea;
   QPointF mousePoint;
};

#endif // ENVELOPE_DRAW_AREA_H
