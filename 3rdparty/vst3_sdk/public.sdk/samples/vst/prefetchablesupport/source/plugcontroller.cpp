//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/prefetchablesupport/source/plugcontroller.cpp
// Created by  : Steinberg, 04/2015
// Description : Plug Controller Example for VST 3
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


#include "plugcontroller.h"
#include "plugparamids.h"

#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstprefetchablesupport.h"
#include "base/source/fstreamer.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// PlugController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::initialize (FUnknown* context)
{
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	//---Create Parameters------------

	//---Bypass parameter---
	int32 stepCount = 1;
	ParamValue defaultVal = 0;
	int32 flags = ParameterInfo::kCanAutomate|ParameterInfo::kIsBypass;
	int32 tag = kBypassId;
	parameters.addParameter (STR16 ("Bypass"), nullptr, stepCount, defaultVal, flags, tag);

	//---PrefetchMode parameter
	tag = kPrefetchableMode;
	auto* prefetchList = new StringListParameter (STR16 ("Prefetch Mode"), tag);
	parameters.addParameter (prefetchList);

	prefetchList->appendString (STR16 ("Is Never"));
	prefetchList->appendString (STR16 ("Is Yet"));
	prefetchList->appendString (STR16 ("Is Not Yet"));
	prefetchList->setNormalized (kIsYetPrefetchable / (kNumPrefetchableSupport - 1));

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::setComponentState (IBStream* state)
{
	// we receive the current state of the component (processor part)
	// we read only the gain and bypass value...

	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	// read the bypass
	int32 bypassState = 0;
	if (streamer.readInt32 (bypassState) == false)
		return kResultFalse;
	setParamNormalized (kBypassId, bypassState ? 1 : 0);

	int32 prefetchableMode;
	if (streamer.readInt32 (prefetchableMode) == false)
		return kResultFalse;
	setParamNormalized (kPrefetchableMode, prefetchableMode / (kNumPrefetchableSupport - 1));

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::setParamNormalized (ParamID tag, ParamValue value)
{
	tresult res = kResultFalse;

	if (tag == kPrefetchableMode)
	{
		if (value != getParamNormalized (tag))
		{
			res = EditControllerEx1::setParamNormalized (tag, value);
			getComponentHandler ()->restartComponent (kPrefetchableSupportChanged);
		}
	}
	else
		res = EditControllerEx1::setParamNormalized (tag, value);

	return res;
}

}} // namespaces
