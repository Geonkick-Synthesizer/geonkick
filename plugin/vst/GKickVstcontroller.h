/**
 * File name: controller_vst.h
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

#ifndef GKICK_VST_CONTROLLER_H
#define GKICK_VST_CONTROLLER_H

#include "VstIds.h"

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;

class GKickVstController : public Vst::EditController {
 public:
        static FUnknown* createInstance(void*);
        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        IPlugView* PLUGIN_API createView(const char* name) SMTG_OVERRIDE;
        tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
};

#endif // GKICK_VST_CONTROLLER_H
