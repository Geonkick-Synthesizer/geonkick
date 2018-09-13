/**
 * File name: fader.h
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

#ifndef GEONKICK_FADER_H
#define GEONKICK_FADER_H

#include "geonkick_widget.h"

#include <unordered_map>

class GeonkickSlider;
class GeonkickLevel;

class Fader: public GeonkickWidget
{
 Q_OBJECT

 public:
        Fader(GeonkickWidget *parent = nullptr);
        ~Fader();
        int getFaderLevel(void) const;
        int getChannelLevel(int channel) const;

 public slots:
        void setFaderLevel(int level);
        void setChannelLevel(int channel, int level);

 private:
        void drawLevels(QPainter &painter);
        void paintWidget(QPaintEvent *event) final;
        void resizeEvent(QResizeEvent *event) final;
        GeonkickSlider *faderSlider;
        int leftChannelLevel;
        int rightChannelLevel;
        QPixmap levelsImage;
};

#endif // GEONKICK_FADER_H
