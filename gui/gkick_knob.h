/**
 * File name: gkick_knob.h
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

#ifndef GKICK_KNOB_H
#define GKICK_KNOB_H

#include "geonkick_widget.h"

#include <QWidget>
#include <QMouseEvent>

class GKickKnob : public GeonkickWidget
{
   Q_OBJECT
public:
   GKickKnob(GeonkickWidget *parent);
   virtual ~GKickKnob();
   double getValue(void);
   void setMaxValue(double val);
   void setCurrentValue(double val);
   void setKnobImage(const QPixmap &pixmap);

 signals:
     void valueUpdated(double v);

 protected:
   void paintEvent(QPaintEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);

 private:
   QPixmap knobPixmap;
   QPoint lastPositionPoint;
   double knobValueDegree;
   double rangeValue;
   bool isSelected;
};

#endif
