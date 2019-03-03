/**
 * File name: factory_vst.h
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
#include "processor_vst.h"
#include "ids_vst.h"

#include "public.sdk/source/main/pluginfactoryvst3.h"
#include "pluginterfaces/base/fplatform.h"

#define gkickPluginName		"Geonkick"
#define gkickOriginalFilename	"Geonkick.vst3"
#if SMTG_PLATFORM_64
#define gkickFileDescription	gkickPluginName" Geonkick (64Bit)"
#else
#define gkickFileDescription	gkickPluginName" Geonkick"
#endif
#define gkickCompanyName		"Quamplex"
#define gkickCompanyWeb		"http://quamplex.com"
#define gkickCompanyEmail		"mailto:iurie.nistor@quamplex.com"
#define gkickLegalCopyright	"Copyright (C) 2019 Iurie Nistor"
#define gkickLegalTrademarks	"Licence GPLv3"

BEGIN_FACTORY_DEF(gkickCompanyName, gkickCompanyWeb, gkickCompanyEmail)

DEF_CLASS2(INLINE_UID_FROM_FUID(GKickVstProcessorUID),
			PClassInfo::kManyInstances,
			kVstAudioEffectClass,
			gkickPluginName,
			Vst::kDistributable,
			"Fx",
			"2.0.0",
			kVstVersionString,
			GKickVstProcessor::createInstance)

		DEF_CLASS2(INLINE_UID_FROM_FUID(GKickVstControllerUID),
			PClassInfo::kManyInstances,
			kVstComponentControllerClass,
			gkickPluginName "Controller",
			0,
			"",
			"2.0.0",
			kVstVersionString,
			GKickVstController::createInstance)

		END_FACTORY

		bool InitModule()
	{
		return true;
	}

	bool DeinitModule()
	{
		return true;
	}
