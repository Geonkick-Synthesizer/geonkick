/**
 * File name: GkickVstController.cpp
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

#include "GKickVstController.h"
#include "GKickVstEditor.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"

FUnknown* GKickVstController::createInstance(void*)
{
        GEONKICK_LOG_INFO("called");
        return static_cast<Vst::IEditController*>(new GKickVstController());
}

tresult PLUGIN_API GKickVstController::initialize(FUnknown* context)
{
        GEONKICK_LOG_INFO("called");
        EditController::initialize(context);
        return kResultTrue;
}

IPlugView* PLUGIN_API GKickVstController::createView(const char* name)
{
        if (name && std::string(name) == std::string("editor"))
                return static_cast<IPlugView*>(new GKickVstEditor(this));
        return nullptr;
}

tresult PLUGIN_API GKickVstController::setComponentState(IBStream* state)
{
        GEONKICK_LOG_INFO("called");
        return kResultOk;
}
