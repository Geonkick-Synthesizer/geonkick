/**
 * File name: GKickVstProcessor.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor <http://geontime.com>
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
#include "GKickVstEditor.h"
#include "VstIds.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "geonkick_api.h"
#include "kit_state.h"

GKickVstProcessor::GKickVstProcessor()
        : geonkickApi{nullptr}
{
}

FUnknown* GKickVstProcessor::createInstance(void*)
{
        return static_cast<Vst::IAudioProcessor*>(new GKickVstProcessor());
}


tresult PLUGIN_API
GKickVstProcessor::initialize(FUnknown* context)
{
        auto res = Vst::SingleComponentEffect::initialize(context);
        if (res != kResultTrue)
                return kResultFalse;

        geonkickApi = std::make_unique<GeonkickApi>();
        if (!geonkickApi->init()) {
                geonkickApi = nullptr;
                GEONKICK_LOG_ERROR("can't init Geonkick API");
                return kResultFalse;
        }

        auto nChannels = geonkickApi->numberOfChannels();
        for (decltype(nChannels) i = 0; i < nChannels; i++) {
                std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> convert;
                std::u16string str16 = convert.from_bytes(std::string("Out" + std::to_string(i)));
                addAudioOutput(str16.c_str(), Vst::SpeakerArr::kStereo);
        }
        addEventInput(STR16("MIDI in"), 1);
        return kResultTrue;
}

tresult PLUGIN_API
GKickVstProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs,
                                                         int32 numIns,
                                                         Vst::SpeakerArrangement* outputs,
                                                         int32 numOuts)
{
        auto n = geonkickApi->numberOfChannels();
        if (numIns == 0 && numOuts == static_cast<decltype(numOuts)>(n))
                return Vst::SingleComponentEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
        return kResultFalse;
}

tresult PLUGIN_API
GKickVstProcessor::setupProcessing(Vst::ProcessSetup& setup)
{
        return Vst::SingleComponentEffect::setupProcessing(setup);
}

tresult PLUGIN_API
GKickVstProcessor::setActive(TBool state)
{
        return Vst::SingleComponentEffect::setActive(state);
}

tresult PLUGIN_API
GKickVstProcessor::process(Vst::ProcessData& data)
{
        if (data.numSamples > 0) {
                auto events = data.inputEvents;
                auto nEvents = events->getEventCount();
                auto eventIndex = 0;
                Vst::Event event;
                auto res = events->getEvent(eventIndex, event);
                for (decltype(data.numSamples) i = 0; i < data.numSamples; i++) {
                        if (res != kResultOk)
                                res = events->getEvent(eventIndex, event);
                        while (res == kResultOk && event.sampleOffset == i && eventIndex < nEvents) {
                                switch (event.type) {
                                case Vst::Event::kNoteOnEvent:
                                        geonkickApi->setKeyPressed(true,
                                                                   event.noteOn.pitch,
                                                                   127 * event.noteOn.velocity);
                                        break;

                                case Vst::Event::kNoteOffEvent:
                                        geonkickApi->setKeyPressed(false,
                                                                   event.noteOff.pitch,
                                                                   127 * event.noteOff.velocity);
                                        break;
                                default:
                                        break;
                                }

                                eventIndex++;
                                if (eventIndex < nEvents)
                                        res = events->getEvent(eventIndex, event);
                        }

                        auto nChannels = geonkickApi->numberOfChannels();
                        nChannels = std::min(nChannels, static_cast<decltype(nChannels)>(data.numOutputs));
                        for (decltype(nChannels) ch = 0; ch < nChannels; ch++) {
                                if (data.outputs[ch].channelBuffers32[0] && data.outputs[ch].channelBuffers32[1]) {
                                        auto val = geonkickApi->getAudioFrame(ch);
                                        data.outputs[ch].channelBuffers32[0][i] = val;
                                        data.outputs[ch].channelBuffers32[1][i] = val;
                                }
                        }
                }
	}

        return kResultOk;
}

tresult PLUGIN_API
GKickVstProcessor::setState(IBStream* state)
{
        if (state == nullptr || geonkickApi == nullptr) {
                GEONKICK_LOG_ERROR("wrong arguments or DSP is not ready");
                return kResultFalse;
        }

        if (state->seek(0, IBStream::kIBSeekEnd, 0) == kResultFalse) {
                GEONKICK_LOG_ERROR("can't seek in stream");
                return kResultFalse;
        }

        int64 endStream = 0;
        if (state->tell(&endStream) == kResultFalse) {
                GEONKICK_LOG_ERROR("can't get current position in stream");
                return kResultFalse;
        } else if (endStream < 1) {
                GEONKICK_LOG_ERROR("stream is empty");
                return kResultFalse;
        }

        if (state->seek(0, IBStream::kIBSeekSet, 0) == kResultFalse) {
                GEONKICK_LOG_ERROR("can't seek in stream");
                return kResultFalse;
        }

        std::string data(endStream, '\0');
        int32 nBytes = 0;
        if (state->read(data.data(), data.size(), &nBytes) == kResultFalse) {
                GEONKICK_LOG_ERROR("error on reading the state");
                return kResultFalse;
        }

        if (static_cast<decltype(nBytes)>(data.size()) != nBytes) {
                GEONKICK_LOG_ERROR("error on reading the state");
                return kResultFalse;
        }
        geonkickApi->setKitState(data);
        geonkickApi->notifyUpdateGui();
        return kResultOk;
}

tresult PLUGIN_API
GKickVstProcessor::getState(IBStream* state)
{
        if (state == nullptr || geonkickApi == nullptr) {
                GEONKICK_LOG_ERROR("wrong arguments or DSP is not ready");
                return kResultFalse;
        }

        int32 nBytes = 0;
        auto data = geonkickApi->getKitState()->toJson();
        if (state->write(data.data(), data.size(), &nBytes) == kResultFalse) {
                GEONKICK_LOG_ERROR("error on saving the state");
                return kResultFalse;
        }

        if (static_cast<decltype(nBytes)>(data.size()) != nBytes) {
                GEONKICK_LOG_ERROR("error on saving the state");
                return kResultFalse;
        }
        return kResultOk;
}

IPlugView* PLUGIN_API
GKickVstProcessor::createView(FIDString name)
{
        if (name && std::string(name) == std::string("editor"))
                return static_cast<IPlugView*>(new GKickVstEditor(this, geonkickApi.get()));
        return nullptr;
}

tresult PLUGIN_API
GKickVstProcessor::setComponentState(IBStream* state)
{
        return kResultOk;
}

