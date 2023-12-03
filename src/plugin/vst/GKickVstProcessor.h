/**
 * File name: GKickVstProcessor.h
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

#ifndef GEONKICK_PLUGIN_VST_PROCESSOR_H
#define GEONKICK_PLUGIN_VST_PROCESSOR_H

#include "globals.h"
#include "public.sdk/source/vst/vstsinglecomponenteffect.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef GEONKICK_OS_WINDOWS
__attribute__((visibility("default"))) bool ModuleEntry (void*);
__attribute__((visibility("default"))) bool ModuleExit (void);
#endif // GEONKICK_OS_WINDOWS
        
#ifdef __cplusplus
}
#endif // __cplusplus

class GeonkickApi;

using namespace Steinberg;

class GKickVstProcessor : public Vst::SingleComponentEffect {
  public:
        GKickVstProcessor();
        ~GKickVstProcessor();
        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        static FUnknown* createInstance(void*);
        tresult PLUGIN_API setBusArrangements(Vst::SpeakerArrangement* inputs,
                                              int32 numIns,
                                              Vst::SpeakerArrangement* outputs,
                                              int32 numOuts) SMTG_OVERRIDE;
        tresult PLUGIN_API setupProcessing(Vst::ProcessSetup& setup) SMTG_OVERRIDE;
        tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
        tresult PLUGIN_API process(Vst::ProcessData& data) SMTG_OVERRIDE;
        tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
        IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;
        tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;

  protected:
        std::unique_ptr<GeonkickApi> geonkickApi;
        std::vector<float*> channelsBuffers;
        int sampleRate;
};

#endif // GEONKICK_PLUGIN_VST_PROCESSOR_H
