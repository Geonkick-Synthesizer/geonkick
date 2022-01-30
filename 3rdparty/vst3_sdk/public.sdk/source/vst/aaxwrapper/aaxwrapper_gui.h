//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/aaxwrapper/aaxwrapper_gui.h
// Created by  : Steinberg, 08/2017
// Description : VST 3 -> AAX Wrapper
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
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

/// \cond ignore

#pragma once

#include "AAX_CEffectGUI.h"
#include "pluginterfaces/base/fplatform.h"

//==============================================================================
class AAXWrapper_GUI : public AAX_CEffectGUI
{
public:
	static AAX_IEffectGUI* AAX_CALLBACK Create ();

	AAXWrapper_GUI () = default;
	virtual ~AAXWrapper_GUI () = default;

	void CreateViewContents () SMTG_OVERRIDE;
	void CreateViewContainer () SMTG_OVERRIDE;
	void DeleteViewContainer () SMTG_OVERRIDE;

	AAX_Result GetViewSize (AAX_Point* oEffectViewSize) const SMTG_OVERRIDE;

	AAX_Result SetControlHighlightInfo (AAX_CParamID /* iParameterID */,
	                                    AAX_CBoolean /* iIsHighlighted */,
	                                    AAX_EHighlightColor /* iColor */) SMTG_OVERRIDE;

	AAX_Result TimerWakeup () SMTG_OVERRIDE;

	bool setWindowSize (AAX_Point& size); // calback from AAXWrapper

private:
	bool mInOpen = false;
	bool mRefreshSize = false;
	void* mHWND = nullptr;
};

/// \endcond
