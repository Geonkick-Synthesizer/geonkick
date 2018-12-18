/**
 * File name: fader.cpp
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

#include "fader.h"
#include "geonkick_slider.h"
#include "geonkick_api.h"

Fader::Fader(GeonkickApi *api, GeonkickWidget *parent)
        : GeonkickWidget(parent),
          geonkickApi(api),
          faderSlider(new GeonkickSlider(this, GeonkickSlider::Orientation::Vertical)),
          leftChannelLevel(0),
          rightChannelLevel(0),
          levelsImage(":/fader.png")
{
        faderSlider->move(0, 3);
        connect(faderSlider, SIGNAL(valueUpdated(int)), this, SIGNAL(levelUpdated(int)));
        connect(geonkickApi, SIGNAL(currentPlayingFrameVal(double)), this, SLOT(updateLeveler(double)));
        connect(&faderTimer, SIGNAL(timeout()), this, SLOT(updateLevelerTimeout()));
        updateFader();
        faderTimer.start(50);
}

Fader::~Fader()
{
}

/**
 *  Draws two concentric filled rectangles for every channel.
 */
void Fader::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event)
        QPainter painter(this);
        painter.drawPixmap(faderSlider->x() + faderSlider->width() + 5, 0, levelsImage);
        drawLevels(painter);
}

void Fader::drawLevels(QPainter &painter)
{
        int x = faderSlider->width() + 10;
        int levelWidth   = 7;
        int levelHeight  = levelsImage.size().height() - 4;
        int levelPadding = 2;
        int levelInnerW  = levelWidth - 2 * levelPadding;
        int levelInnerH  = levelHeight - 2 * levelPadding;

        int levelPixels = levelInnerH * ((double)leftChannelLevel / 100);
        painter.fillRect(x + 2, levelPadding + 325 - levelPixels,
                         levelInnerW, levelPixels,
                         QBrush(QColor(125, 200, 125)));
}

void Fader::updateLevelerTimeout()
{
        for (auto i = 0; i < 2; i++) {
                int val = getChannelLevel(i) - 1;
                if (val > -1)
                        setChannelLevel(i, val);
        }
}

void Fader::resizeEvent(QResizeEvent *event)
{
        faderSlider->setFixedSize(20, levelsImage.size().height() - 3);
        update();
}

int Fader::getFaderLevel(void) const
{
        return faderSlider->getValue();
}

int Fader::getChannelLevel(int channel) const
{
        int level;
        if (channel == 0) {
                level = leftChannelLevel;
        } else {
                level = rightChannelLevel;
        }

        return level;
}

void Fader::setFaderLevel(int level)
{
        faderSlider->setValue(level);
}

void Fader::updateLeveler(double val)
{
        val = fabs(val);
        int  level = logValToLevel(val);
        if (getChannelLevel(0) < level)
                setChannelLevel(0, level);
        if (getChannelLevel(0) < level)
                setChannelLevel(1, level);
}

void Fader::setChannelLevel(int channel, int level)
{
        if (channel == 0) {
                leftChannelLevel = level;
        } else {
                rightChannelLevel = level;
        }
        update();
}

void Fader::updateFader()
{
        double val = geonkickApi->limiterValue();
        int level = logValToLevel(20 * log10(val));
        setFaderLevel(level);
}

int Fader::logValToLevel(double val)
{
        int level;
        if (20 * log10(val) < -60) {
                level = 0;
        } else {
                double logVal = 20 * log10(geonkickApi->limiterValue());
                double k = 70.0 / (1 - 0.07);
                double b = 20.0 - k;
                double x = (logVal - b) / k;
                level = 100 * x;
        }
        return level;
}
