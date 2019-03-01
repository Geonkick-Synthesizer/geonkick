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

#include <fstreamer.h>
#include <ibstream.h>
#include <ivstparameterchanges.h>

GKickVstProcessor::GKickVstProcessor()
{
	setControllerClass(GKickVstControllerUID);
}

tresult PLUGIN_API GKickVstProcessor::initialize(FUnknown* context)
{
	auto res = AudioEffect::initialize (context);
	if (res != kResultTrue)
		return kResultFalse;

	addAudioInput(STR16("AudioInput"), Vst::SpeakerArr::kStereo);
	addAudioOutput(STR16("AudioOutput"), Vst::SpeakerArr::kStereo);

	return kResultTrue;
}

tresult PLUGIN_API GKickVstProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs,
                                                         int32 numIns,
                                                         Vst::SpeakerArrangement* outputs,
                                                         int32 numOuts)
{
	if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0])
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	return kResultFalse;
}

tresult PLUGIN_API GKickVstProcessor::setupProcessing(Vst::ProcessSetup& setup)
{
	return AudioEffect::setupProcessing(setup);
}

tresult PLUGIN_API GKickVstProcessor::setActive(TBool state)
{
	return AudioEffect::setActive(state);
}

tresult PLUGIN_API GKickVstProcessor::process(Vst::ProcessData& data)
{
	return kResultOk;
}

tresult PLUGIN_API GKickVstProcessor::setState(IBStream* state)
{
	if (!state)
		return kResultFalse;

	return kResultOk;
}

tresult PLUGIN_API GKickVstProcessor::getState(IBStream* state)
{
	return kResultOk;
}

static FUnknown* GKickVstProcessor::createInstance(void*)
{
        return static_cast<Vst::IAudioProcessor*>(new GKickVstProcessor());
}
