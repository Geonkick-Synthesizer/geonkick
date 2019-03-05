/**
 * File name: processor_vst.h
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

#include "processor_vst.h"
#include "ids_vst.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

namespace Steinberg
{

GKickVstProcessor::GKickVstProcessor()
{
        RK_LOG_INFO("called");
        setControllerClass(GKickVstControllerUID);
}

tresult PLUGIN_API GKickVstProcessor::initialize(FUnknown* context)
{
        RK_LOG_INFO("called");
        auto res = AudioEffect::initialize(context);
        if (res != kResultTrue)
                return kResultFalse;

        addAudioInput(STR16("AudioInputXX"), Vst::SpeakerArr::kStereo);
        addAudioOutput(STR16("AudioOutputXX"), Vst::SpeakerArr::kStereo);

        return kResultTrue;
}

tresult PLUGIN_API GKickVstProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs,
                                                         int32 numIns,
                                                         Vst::SpeakerArrangement* outputs,
                                                         int32 numOuts)
{
        RK_LOG_INFO("called");
        if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0])
                return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
        return kResultFalse;
}

tresult PLUGIN_API GKickVstProcessor::setupProcessing(Vst::ProcessSetup& setup)
{
        RK_LOG_INFO("called");
        return AudioEffect::setupProcessing(setup);
}

tresult PLUGIN_API GKickVstProcessor::setActive(TBool state)
{
        RK_LOG_INFO("called");
        return AudioEffect::setActive(state);
}

tresult PLUGIN_API GKickVstProcessor::process(Vst::ProcessData& data)
{
        return kResultOk;
}

tresult PLUGIN_API GKickVstProcessor::setState(IBStream* state)
{
        RK_LOG_INFO("called");
        if (!state)
                return kResultFalse;
        return kResultOk;
}

tresult PLUGIN_API GKickVstProcessor::getState(IBStream* state)
{
        RK_LOG_INFO("called");
		return kResultOk;
}

FUnknown* GKickVstProcessor::createInstance(void*)
{
        RK_LOG_INFO("called");
        return static_cast<Vst::IAudioProcessor*>(new GKickVstProcessor());
}

} // namespace Steinberg
