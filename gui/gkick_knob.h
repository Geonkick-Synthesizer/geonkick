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

#include <QWidget>
#include <QMouseEvent>

#define GKICK_UI_DEFAULT_KNOB_DIAMETER 140

class GKickKnob : public QWidget
{
   Q_OBJECT
public:
   GKickKnob(QWidget *parent, const QString &name);
   virtual ~GKickKnob();
   int getRadius(void);
   int getWidth(void);
   int getHeight(void);
   double getPosition(void);
   double getValue(void);
   void setMaxValue(double val);
   void setCurrentValue(double val);

   public slots:
     void setPosition(double v);
     void setValue(double v);

 signals:
     void valueUpdated(double v);

 protected:
   void paintEvent(QPaintEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseDoubleClickEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void resizeEvent(QResizeEvent *event);

 private:
   QString knobName;
   int knobRadius;
   QPoint lastPositionPoint;
   int knobValueDegree;
   double realValue;
   bool isSelected;
};

#endif
