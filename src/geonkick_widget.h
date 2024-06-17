/**
 * File name: geonkick_widget.h
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

#ifndef GEONKICK_WIDGET_H
#define GEONKICK_WIDGET_H

#include "globals.h"

#include <RkWidget.h>
#include <RkImage.h>

class RkPaintEvent;
class RkNativeWindowInfo;
class ViewState;

class GeonkickWidget: public RkWidget
{
 public:
        explicit GeonkickWidget(RkMain& app,
                                Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        explicit GeonkickWidget(RkMain& app,
                                const RkNativeWindowInfo &info,
                                Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        explicit GeonkickWidget(GeonkickWidget *parent,
                                Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        virtual ~GeonkickWidget() = default;
        void setBackgroundImage(const RkImage &img);
        void setBackgroundImage(const std::string &file);
        void setViewState(ViewState *state);
        ViewState* viewState() const;

 protected:
        void paintEvent(RkPaintEvent *event) override;
        virtual void paintWidget(RkPaintEvent *event);
        void mouseButtonPressEvent(RkMouseEvent *event) override;

private:
        RkImage backgroundImage;
        ViewState* viewingState;
};

#endif // GEONKICK_WIDGET_H
