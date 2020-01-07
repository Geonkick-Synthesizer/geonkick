/**
 * File name: channels_widget.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://geontime.com>
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

#ifndef CHANNELS_WIDGET_H
#define CHANNELS_WIDGET_H

#include "geonkick_widget.h"
#include "RkPainter.h"

class GeonkickApi;

class ChannelsWidget: public GeonkickWidget
{
 public:
        ChannelsWidget(GeonkickWidget *parent, GeonkickApi* api);
        ~ChannelsWidget() = default;
        RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());

 protected:
	void paintWidget(const std::shared_ptr<RkPaintEvent> &event) override;
	void mouseMoveEvent(const std::shared_ptr<RkMouseEvent> &event) override;
	void mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event) override;
	void mouseButtonReleaseEvent(const std::shared_ptr<RkMouseEvent> &event) override;
	void mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event) override;
        void drawKeys(RkPainter &painter);
 private:
	GeonkickApi* geonkickApi;
};

#endif // CHANNELS_WIDGET_H
