/**
 * File name: VstFactory.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor 
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

#include "GKickVstProcessor.h"
#include "VstIds.h"

#include "public.sdk/source/main/pluginfactory.h"
#include "pluginterfaces/base/fplatform.h"

#define gkickPluginName		"Geonkick"
#define gkickOriginalFilename	"Geonkick.vst3"
#if SMTG_PLATFORM_64
#define gkickFileDescription	gkickPluginName" Geonkick (64Bit)"
#else
#define gkickFileDescription	gkickPluginName" Geonkick"
#endif
#define gkickCompanyName	"Geonkick"
#define gkickCompanyWeb		"http://iurienistor.wordpress.com"
#define gkickCompanyEmail	""
#define gkickLegalCopyright	"Copyright (C) 2020 Iurie Nistor. Licensed under the GNU General Public License version 3 (GPLv3)."

BEGIN_FACTORY_DEF(gkickCompanyName, gkickCompanyWeb, gkickCompanyEmail)

DEF_CLASS2(INLINE_UID_FROM_FUID(GKickVstProcessorUID),
           PClassInfo::kManyInstances,
           kVstAudioEffectClass,
           gkickPluginName,
           0,
           Vst::PlugType::kInstrumentSynth,
           "3.0.0"/*GEOKICK_VERSION_STRING*/,
           kVstVersionString,
           GKickVstProcessor::createInstance)

END_FACTORY

bool InitModule()
{
        return true;
}

bool DeinitModule()
{
        return true;
}


