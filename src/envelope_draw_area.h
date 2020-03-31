/**
 * File name: envelope_draw_area.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://geontime.com>
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

#include "RkRect.h"
#include "RkRealPoint.h"
#include "RkImage.h"

class Envelope;
class KickGraph;
class RkMouseEvent;
class KickGraph;
class GeonkickApi;

class EnvelopeWidgetDrawingArea : public GeonkickWidget
{
 public:
   EnvelopeWidgetDrawingArea(GeonkickWidget *parent, GeonkickApi *api);
   ~EnvelopeWidgetDrawingArea();
   void paintWidget(const std::shared_ptr<RkPaintEvent> &event) final;
   Envelope* getEnvelope() const;
   const RkRect getDrawingArea();
   void updateKickGraph(const std::shared_ptr<RkImage> &graphImage);
   bool isHideEnvelope() const;
   void setEnvelope(Envelope* envelope);
   void envelopeUpdated();
   void setHideEnvelope(bool b);

 protected:
   void mouseMoveEvent(const std::shared_ptr<RkMouseEvent> &event) final;
   void mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event) final;
   void mouseButtonReleaseEvent(const std::shared_ptr<RkMouseEvent> &event) final;
   void mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event) final;
   std::string getEnvStateText() const;

 private:
   GeonkickApi* geonkickApi;
   Envelope* currentEnvelope;
   RkRect drawingArea;
   RkRealPoint mousePoint;
   bool hideEnvelope;
   std::shared_ptr<RkImage> kickGraphImage;
   RkImage envelopeImage;
   std::unique_ptr<KickGraph> kickGraphics;
};

#endif // ENVELOPE_DRAW_AREA_H
