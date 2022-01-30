//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostcheck.cpp
// Created by  : Steinberg, 04/2012
// Description :
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2021, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "hostcheck.h"
#include "logevents.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

//------------------------------------------------------------------------
HostCheck::HostCheck ()
{
	mProcessSetupCheck.setEventLogger (&mEventLogger);
	mProcessContextCheck.setEventLogger (&mEventLogger);
	mEventListCheck.setEventLogger (&mEventLogger);
	mParamChangesCheck.setEventLogger (&mEventLogger);
	mParamChangesCheck.setParamIDs (&mParameterIds);
}

//------------------------------------------------------------------------
void HostCheck::addParameter (Steinberg::Vst::ParamID paramId)
{
	mParameterIds.insert (paramId);
	mParamChangesCheck.updateParameterIDs ();
}

//------------------------------------------------------------------------
void HostCheck::addLogEvent (Steinberg::int32 logId)
{
	mEventLogger.addLogEvent (logId);
}

//------------------------------------------------------------------------
bool HostCheck::validate (Steinberg::Vst::ProcessData& data, Steinberg::int32 minInputBufferCount,
                          Steinberg::int32 minOutputBufferCount)
{
	mProcessSetupCheck.check (data);
	mProcessContextCheck.check (data.processContext);
	mEventListCheck.check (data.inputEvents);
	mParamChangesCheck.checkParameterChanges (data.inputParameterChanges);

	checkAudioBuffers (data.inputs, data.numInputs, Steinberg::Vst::kInput, data.symbolicSampleSize,
	                   minInputBufferCount);
	checkAudioBuffers (data.outputs, data.numOutputs, Steinberg::Vst::kOutput,
	                   data.symbolicSampleSize, minOutputBufferCount);

	return mEventLogger.empty ();
}

//------------------------------------------------------------------------
void HostCheck::checkAudioBuffers (Steinberg::Vst::AudioBusBuffers* buffers,
                                   Steinberg::int32 numBuffers, Steinberg::Vst::BusDirection dir,
                                   Steinberg::int32 symbolicSampleSize,
                                   Steinberg::int32 minBufferCount)
{
	if (mComponent)
	{
		if (numBuffers > 0)
		{
			// Steinberg::int32 audioBusCount = mComponent->getBusCount (Steinberg::Vst::kAudio,
			// dir);
			bool isValid = minBufferCount <= numBuffers;
			if (!isValid)
			{
				addLogEvent (kLogIdAudioBufNotMatchComponentBusCount);
			}
		}
	}

	if (numBuffers > 0)
	{
		if (!buffers)
		{
			addLogEvent (kLogIdNullPointerToAudioBusBuffer);
			return;
		}

		for (Steinberg::int32 bufferIdx = 0; bufferIdx < numBuffers; ++bufferIdx)
		{
			Steinberg::Vst::BusInfo busInfo = {};
			mComponent->getBusInfo (Steinberg::Vst::kAudio, dir, bufferIdx, busInfo);
			Steinberg::Vst::AudioBusBuffers& tmpBuffers = buffers[bufferIdx];
			if (tmpBuffers.numChannels != busInfo.channelCount)
			{
				addLogEvent (kLogIdInvalidAudioBufNumOfChannels);
			}

			if (symbolicSampleSize == Steinberg::Vst::kSample32)
			{
				for (Steinberg::int32 chIdx = 0; chIdx < tmpBuffers.numChannels; ++chIdx)
				{
					if (!tmpBuffers.channelBuffers32 || !tmpBuffers.channelBuffers32[chIdx])
					{
						if (busInfo.busType == Steinberg::Vst::kAux)
							addLogEvent (kLogIdNullPointerToAuxChannelBuf);
						else
							addLogEvent (kLogIdNullPointerToChannelBuf);
					}
				}
			}
			else
			{
				for (Steinberg::int32 chIdx = 0; chIdx < tmpBuffers.numChannels; ++chIdx)
				{
					if (!tmpBuffers.channelBuffers64 || !tmpBuffers.channelBuffers64[chIdx])
					{
						if (busInfo.busType == Steinberg::Vst::kAux)
							addLogEvent (kLogIdNullPointerToAuxChannelBuf);
						else
							addLogEvent (kLogIdNullPointerToChannelBuf);
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void HostCheck::setComponent (Steinberg::Vst::IComponent* component)
{
	mEventListCheck.setComponent (component);
	mComponent = component;
}

//------------------------------------------------------------------------
void HostCheck::setProcessSetup (Steinberg::Vst::ProcessSetup& setup)
{
	mProcessSetupCheck.setProcessSetup (setup);
	mEventListCheck.setProcessSetup (setup);
	mProcessContextCheck.setSampleRate (setup.sampleRate);
}
