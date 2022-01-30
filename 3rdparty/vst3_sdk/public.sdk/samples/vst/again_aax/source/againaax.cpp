//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/again/source/againaax.cpp
// Created by  : Steinberg, 03/2016
// Description : AGain AAX Example for VST SDK 3
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

#include "public.sdk/source/vst/aaxwrapper/aaxwrapper_description.h"

#include "againcids.h"
#include "againparamids.h"
#include "pluginterfaces/base/futils.h"

//------------------------------------------------------------------------
#if 0 // for additional outputs
AAX_Aux_Desc effAux_stereo[] = 
{
	// name, channel count
	{ "AGain AUX2",  2 }, 
	{ nullptr }
};
#endif

//------------------------------------------------------------------------
#if 1 // MIDI inputs for instruments or Fx with MIDI input
AAX_MIDI_Desc effMIDI[] = 
{
	// port name, channel mask
	{ "AGain", 0xffff }, 
	{ nullptr }
};
#endif

//------------------------------------------------------------------------
// Input/Output meters
AAX_Meter_Desc effMeters[] = 
{
	// not used { "Input", CCONST ('A', 'G', 'I', 'n'),  0 /*AAX_eMeterOrientation_Default*/, 0 /*AAX_eMeterType_Input*/ },
	{ "Output", kVuPPMId, 0 /*AAX_eMeterOrientation_Default*/, 1 /*AAX_eMeterType_Output*/ },
	{ nullptr }
};

//------------------------------------------------------------------------
AAX_Plugin_Desc effPlugins[] = {
	// effect-ID, name,	
	// Native ID, AudioSuite ID,
	// InChannels, OutChannels, InSideChain channels,
	// MIDI, Aux,
	// Meters
	// Latency
	// note: IDs must be unique across plugins

	// Mono variant
	{"com.steinberg.again.mono",
	 "AGain", 
	 CCONST ('A', 'G', 'N', '1'),
	 CCONST ('A', 'G', 'A', '1'),
	 1, /*mInputChannels*/
	 1, /*mOutputChannels*/
	 0, /*mSideChainInputChannels*/
	 effMIDI, /*effMIDI*/
	 nullptr, /*effAux*/
	 effMeters, /*effMeters*/
	 0 /*Latency*/ 
	},

	// Stereo variant
	{"com.steinberg.again.stereo",
	 "AGain", 
	 CCONST ('A', 'G', 'N', '2'),
	 CCONST ('A', 'G', 'A', '2'), 
	 2, /*mInputChannels*/
	 2, /*mOutputChannels*/
	 0, /*mSideChainInputChannels*/
	effMIDI, /*effMIDI*/
	 nullptr, /*effAux*/
	 effMeters, /*effMeters*/
	 0 /*Latency*/
	},

	{nullptr}
};

//------------------------------------------------------------------------
AAX_Effect_Desc effDesc = {
	"Steinberg",	// manufacturer
	"AGain",		// product
	CCONST ('S', 'M', 'T', 'G'), // manufacturer ID
	CCONST ('A', 'G', 'S', 'B'), // product ID
	AGainVST3Category, // VST category (define againcids.h)
	{0},			// VST3 class ID (set later)
	1,				// version
	nullptr,		// no pagetable file "again.xml",
	effPlugins,
};

//------------------------------------------------------------------------
// this drag's in all the craft from the AAX library
int* forceLinkAAXWrapper = &AAXWrapper_linkAnchor;

//------------------------------------------------------------------------
AAX_Effect_Desc* AAXWrapper_GetDescription ()
{
	// cannot initialize in global descriptor, and it might be link order dependent
	memcpy (effDesc.mVST3PluginID, (const char*)Steinberg::Vst::AGainProcessorUID,
	        sizeof (effDesc.mVST3PluginID));
	return &effDesc;
}
