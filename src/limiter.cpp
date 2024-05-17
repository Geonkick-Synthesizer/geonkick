/**
 * File name: limiter.cpp
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

#include "limiter.h"
#include "geonkick_slider.h"
#include "geonkick_api.h"

#include <RkTimer.h>

RK_DECLARE_IMAGE_RC(meter_scale);

Limiter::Limiter(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickWidget(parent)
        , geonkickApi{api}
        , faderSlider{new GeonkickSlider(this, GeonkickSlider::Orientation::Vertical)}
        , meterValue{0}
        , meterTimer{new RkTimer(this, 30)}
        , levelerValueTimer{new RkTimer(this, 40)}
        , scaleImage{36, 300, RK_IMAGE_RC(meter_scale)}
{
        setFixedSize(65, scaleImage.height());
        faderSlider->setPosition(0, 0);
        faderSlider->setFixedSize(20, height());
        RK_ACT_BIND(faderSlider, valueUpdated, RK_ACT_ARGS(int val), this, onSetLimiterValue(val));
        RK_ACT_BIND(levelerValueTimer, timeout, RK_ACT_ARGS(), this, onUpdateMeter());
        RK_ACT_BIND(meterTimer, timeout, RK_ACT_ARGS(), this, onUpdateMeterTimeout());
        onUpdateLimiter();
        meterTimer->start();
        levelerValueTimer->start();
}

void Limiter::paintWidget(RkPaintEvent *event)
{
        RK_UNUSED(event);
        RkImage img(width(), height());
        RkPainter painter(&img);
        painter.fillRect(rect(), background());
        int x = faderSlider->width() + 10;
        int meterWidth   = 7;
        int meterHeight  = scaleImage.height() - 4;
        int meterPadding = 2;
        int meterInnerW  = meterWidth - 2 * meterPadding;
        int meterInnerH  = meterHeight - 2 * meterPadding;

        int meterPixels = meterInnerH * (static_cast<double>(meterValue) / 100);
        painter.drawImage(scaleImage, 25, 0);
        painter.fillRect(RkRect(x + 2, meterPadding + height() - meterPixels,
                                meterInnerW, meterPixels), RkColor(125, 200, 125));
        RkPainter paint(this);
        paint.drawImage(img, 0, 0);
}

void Limiter::onUpdateMeterTimeout()
{
        int val = getMeterValue() - 1;
        if (val > -1)
                onSetMeterValue(val);
}

int Limiter::getFaderValue(void) const
{
        return faderSlider->getValue();
}

int Limiter::getMeterValue() const
{
        return meterValue;
}

void Limiter::onSetFaderValue(int val)
{
        faderSlider->onSetValue(val, toMeterValue(1.0));
}

void Limiter::onUpdateMeter()
{
        int value = toMeterValue(std::fabs(geonkickApi->getLimiterLevelerValue()));
        if (meterValue < value)
                onSetMeterValue(value);
}

void Limiter::onSetMeterValue(int val)
{
        meterValue = val;
        update();
}

void Limiter::onUpdateLimiter()
{
        double val = geonkickApi->limiterValue();
        if (val < 1e-3)
                onSetFaderValue(0);
        else
                onSetFaderValue(toMeterValue(val));
}

int Limiter::toMeterValue(double val) const
{
        if (val < 1e-3)
                return 0;
        double logVal = 20 * log10(val);
        double k = 70.0 / (1 - 0.07);
        double b = 20.0 - k;
        double c = (b / k);
        double x = logVal / k - c ;
        int value = 100 * x;
        return value;
}

void Limiter::onSetLimiterValue(int val)
{
        double k = 70.0 / (1 - 0.07);
        double b = 20.0 - k;
        double x = static_cast<double>(val) / 100;
        double logVal = k * x + b;
        double value;
        if (logVal < -60)
                value = 0;
        else
                value = pow(10, logVal / 20);
        geonkickApi->setLimiterValue(value);
        action limiterUpdated(val);
}
