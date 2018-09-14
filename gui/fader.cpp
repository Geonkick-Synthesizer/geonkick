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

Fader::Fader(GeonkickWidget *parent)
        : GeonkickWidget(parent),
          faderSlider(new GeonkickSlider(this, GeonkickSlider::Orientation::Vertical)),
          leftChannelLevel(40),
          rightChannelLevel(50),
          levelsImage("./themes/geontime/fader.png")
{
        faderSlider->move(0, 3);
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

void Fader::resizeEvent(QResizeEvent *event)
{
        faderSlider->setFixedSize(20, levelsImage.size().height() - 3);
        update();
}

int Fader::getFaderLevel(void) const
{
        faderSlider->getValue();
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

void Fader::setChannelLevel(int channel, int level)
{
        if (channel == 0) {
                leftChannelLevel = level;
        } else {
                rightChannelLevel = level;
        }
}
