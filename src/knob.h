/**
 * File name: knob.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
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

#ifndef GKICK_KNOB_H
#define GKICK_KNOB_H

#include "geonkick_widget.h"

#include <RkWidget.h>
#include <RkImage.h>

class RkMouseEvent;

class Knob : public GeonkickWidget
{
public:

   enum RangeType: int {
           Linear,
           Logarithmic
   };

   explicit Knob(GeonkickWidget *parent);
   virtual ~Knob() = default;
   double getValue(void) const;
   void setRange(double from, double to);
   void setRangeType(RangeType type);
   RangeType getRangeType() const;
   void setDefaultValue(double val);
   void setCurrentValue(double val);
   void setKnobImage(const RkImage &img);
   void setKnobBackgroundImage(const RkImage &img);
   void rotateKnob(int degree);
   RK_DECL_ACT(valueUpdated,
               valueUpdated(double v),
               RK_ARG_TYPE(double),
               RK_ARG_VAL(v));
 protected:
   void paintWidget(RkPaintEvent *event) override;
   void mouseButtonPressEvent(RkMouseEvent *event) override;
   void mouseButtonReleaseEvent(RkMouseEvent *event) override;
   void mouseMoveEvent(RkMouseEvent *event) override;
   void mouseDoubleClickEvent(RkMouseEvent *event) override;
   double valueToDegree(double val);

 private:
   RkImage knobImage;
   RkImage knobBackground;
   RkPoint lastPositionPoint;
   double knobValueDegree;
   double rangeFrom;
   double rangeTo;
   RangeType rangeType;
   bool isSelected;
   double defaultValue;
};

#endif
