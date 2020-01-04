/**
 * File name: control_area.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#include "oscillator.h"
#include "control_area.h"
#include "oscillator_group_box.h"
#include "general_group_box.h"
#include "effects_group_box.h"
#include "geonkick_api.h"

ControlArea::ControlArea(GeonkickWidget *parent,
                         GeonkickApi* api,
                         const std::vector<std::unique_ptr<Oscillator>> &oscillators)
        : GeonkickWidget(parent),
          controlsWidget{new ControlsWidget(this)}
        , channelsSettingsWidget{new ChannelsWidget(this)}
{
        setFixedSize(920, 360);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), controlsWidget, updateGui());
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), channelWidget, updateGui());
        showControls();
}

void ControlArea::showControls()
{
        controlsWidget->show();
        channelsWidget->hide();
}

void ControlArea::showChannelsSettings()
{
        controlsWidget->hide();
        channelsWidget->show();
}

