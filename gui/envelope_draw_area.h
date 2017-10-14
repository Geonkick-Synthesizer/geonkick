#ifndef ENVELOPE_DRAW_AREA_H
#define ENVELOPE_DRAW_AREA_H

/**
 * File name: envelope_draw_area.h
 * Project: GeonKick
 *
 * Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>
 *
 * This file is part of Geonkick.
 *
 * MashaFile is free software; you can redistribute it and/or modify
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

class EnvelopeDrawingArea : public QWidget
{
   Q_OBJECT

 public:
   EnvelopeDrawingArea(QWidget *parent, std::shared_ptr<GKickEnvelope> &envelope);
   ~EnvelopeDrawingArea();

 private:
   std::shared_ptr<GKickEnvelope> currentEnvelope;
};

#endif
