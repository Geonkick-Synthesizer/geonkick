/**
 * File name: limiter.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

Limiter::Limiter(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickWidget(parent),
          geonkickApi(api),
          faderSlider(new GeonkickSlider(this, GeonkickSlider::Orientation::Vertical)),
          meterValue(0),
          meterImage(":/meter_scale.png")
{
        faderSlider->move(0, 3);
        connect(faderSlider, SIGNAL(valueUpdated(int)), this, SLOT(setLimiterValue(int)));
        connect(geonkickApi, SIGNAL(currentPlayingFrameVal(double)), this, SLOT(updateMeter(double)));
        connect(&meterTimer, SIGNAL(timeout()), this, SLOT(updateMeterTimeout()));
        updateLimiter();
        meterTimer.start(50);
}

Limiter::~Limiter()
{
}

/**
 *  Draws two concentric filled rectangles for every channel.
 */
void Limiter::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event)
        QPainter painter(this);
        painter.drawPixmap(faderSlider->x() + faderSlider->width() + 5, 0, meterImage);
        drawMeter(painter);
}

void Limiter::drawMeter(QPainter &painter)
{
        int x = faderSlider->width() + 10;
        int meterWidth   = 7;
        int meterHeight  = meterImage.size().height() - 4;
        int meterPadding = 2;
        int meterInnerW  = meterWidth - 2 * meterPadding;
        int meterInnerH  = meterHeight - 2 * meterPadding;

        int meterPixels = meterInnerH * ((double)meterValue / 100);
        painter.fillRect(x + 2, meterPadding + 325 - meterPixels,
                         meterInnerW, meterPixels,
                         QBrush(RkColor(125, 200, 125)));
}

void Limiter::updateMeterTimeout()
{
        int val = getMeterValue() - 1;
        if (val > -1)
                setMeterValue(val);
}

void Limiter::resizeEvent(QResizeEvent *event)
{
        Q_UNUSED(event);
        faderSlider->setFixedSize(20, meterImage.size().height() - 3);
        update();
}

int Limiter::getFaderValue(void) const
{
        return faderSlider->getValue();
}

int Limiter::getMeterValue() const
{
        return meterValue;
}

void Limiter::setFaderValue(int val)
{
        faderSlider->setValue(val);
}

void Limiter::updateMeter(double val)
{
        int value = toMeterValue(fabs(val));
        if (meterValue < value)
                setMeterValue(value);
}

void Limiter::setMeterValue(int val)
{
        meterValue = val;
        update();
}

void Limiter::updateLimiter()
{
        double val = geonkickApi->limiterValue();
        if (val < 1e-3)
                setFaderValue(0);
        else
                setFaderValue(toMeterValue(val));
}

int Limiter::toMeterValue(double val) const
{
        if (val < 1e-3)
                return 0;

        double logVal = 20 * log10(val);
        const double k = 70.0 / (1 - 0.07);
        const double b = 20.0 - k;
        const double c = (b / k);
        double x = logVal / k - c ;
        int value = 100 * x;
        return value;
}

void Limiter::setLimiterValue(int val)
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
}
