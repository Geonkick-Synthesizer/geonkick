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
          rightChannelLevel(30)
{
        faderSlider->move(0, 5);
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

        drawLevels(painter);
        drawScale(painter);
}

void Fader::drawLevels(QPainter &painter)
{
        int x = faderSlider->width() + 10;
        int levelWidth   = 7;
        int levelHeight  = faderSlider->height();
        int levelPadding = 2;
        int levelInnerW  = levelWidth - 2 * levelPadding;
        int levelInnerH  = levelHeight - 2 * levelPadding;

        // Left channel level rectangles.
        int levelPixels = levelInnerH * ((double)leftChannelLevel / 100);
        painter.fillRect(x, faderSlider->y(), levelWidth, levelHeight,
                         QBrush(QColor(40, 40, 40)));
        painter.fillRect(x + 2, faderSlider->y() + levelPadding + levelInnerH - levelPixels,
                         levelInnerW, levelPixels,
                         QBrush(QColor(125, 200, 125)));

        // Right channel level rectangles.
        levelPixels = (faderSlider->height() - 4) * ((double)rightChannelLevel / 100);
        x += levelWidth + 3;
        painter.fillRect(x, faderSlider->y(), levelWidth, levelHeight,
                         QBrush(QColor(40, 40, 40)));
        painter.fillRect(x + 2, faderSlider->y() + levelPadding + levelInnerH - levelPixels,
                         levelInnerW, levelPixels,
                         QBrush(QColor(125, 200, 125)));
}

void Fader::drawScale(QPainter &painter)
{
        int i = 20;
        double y = faderSlider->y();
        double dy =  ((double)faderSlider->height()) / 80;
        auto font = painter.font();
        font.setPixelSize(11);
        painter.setFont(font);
        painter.setPen(QPen(QColor(200, 200, 200)));
        while (i >= -50) {
                int x = faderSlider->width() + 7;
                if (abs(i) % 10 == 0 || abs(i) == 6 || abs(i) == 3 || abs(i) == 15 ) {
                        if (abs(i) == 6 || abs(i) == 3 || abs(i) == 15) {
                                painter.drawLine(x, y, x + 2, y);
                        } else {
                                painter.drawLine(x - 3, y, x + 2, y);
                        }
                        x += 2 * (7 + 3);

                        if( abs(i) == 6 || abs(i) == 3 || abs(i) == 15) {
                                painter.drawLine(x , y, x + 3, y);
                        } else {
                                painter.drawLine(x , y, x + 5, y);
                        }
                        painter.drawText(QPoint(x + 10, y + 3), QString::number(i));
                }

                i--;
                y += dy;
        }
}

void Fader::resizeEvent(QResizeEvent *event)
{
        faderSlider->setFixedSize(20, height() - 10);
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
