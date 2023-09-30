/**
 * File name: limiter.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
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

#ifndef GEONKICK_LIMITER_H
#define GEONKICK_LIMITER_H

#include "geonkick_widget.h"

#include <RkPainter.h>

class GeonkickSlider;
class GeonkickLevel;
class GeonkickApi;
class RkTimer;

class Limiter: public GeonkickWidget
{
 public:
        Limiter(GeonkickApi *api, GeonkickWidget *parent);
        int getFaderValue(void) const;
        int getMeterValue() const;

        RK_DECL_ACT(limiterUpdated,
                    limiterUpdated(int val),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(val));
        void onUpdateLimiter();
        void onSetLimiterValue(int val);

 protected:
        int toMeterValue(double val) const;
        void onUpdateMeter();
        void onUpdateMeterTimeout();
        void onSetFaderValue(int val);
        void onSetMeterValue(int val);

 private:
        void paintWidget(RkPaintEvent *event) final;
        GeonkickApi *geonkickApi;
        GeonkickSlider *faderSlider;
        int meterValue;
        RkTimer *meterTimer;
        RkTimer *levelerValueTimer;
        RkImage scaleImage;
};

#endif // GEONKICK_LIMITER_H
