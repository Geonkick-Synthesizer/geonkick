/**
 * File name: controller_vst.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/geonkick)
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

#include "controller_vst.h"
#include "ids_vst.h"

#include <fstreamer.h>
#include <ibstream.h>

tresult PLUGIN_API GKickVstController::initialize(FUnknown* context)
{
        GKICK_LOG_INFO("called");
	auto res = EditController::initialize(context);
        GKICK_LOG_INFO("init: " << res);
	return kResultTrue;
}

IPlugView* PLUGIN_API GKickVstController::createView(const char* name)
{
        GKICK_LOG_INFO("called");
	if (name && std::string(name) == std::string("editor")) {
		return new VST3Editor(this, "view", "plug.uidesc");
	}

	return nullptr;
}

tresult PLUGIN_API GKickVstController::setComponentState(IBStream* state)
{
        GKICK_LOG_INFO("called");
	return kResultOk;
}
