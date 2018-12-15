/**
 * File name: kick_graph.h
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

#ifndef GEONKICK_GRAPTH_H
#define GEONKICK_GRAPTH_H

#include "geonkick_api.h"

#include <QObject>
#include <QPainter>

class QPixmap;
class QImage;

class KickGraph : public QObject {
   Q_OBJECT

public:

     KickGraph(QObject *parent, GeonkickApi *api);
     ~KickGraph();
     void draw(QPainter &painter);

public slots:
     void setDrawingArea(const QRect &rect);

 signals:
     void graphUpdated();

protected:
     void drawKickGraph();

protected slots:
     void updateGraphBuffer();

private:
     GeonkickApi *geonkickApi;
     std::vector<gkick_real> kickBuffer;
     QRect drawingArea;
     QImage cacheGraphImage;
};

#endif // GEONKICK_GRAPH
