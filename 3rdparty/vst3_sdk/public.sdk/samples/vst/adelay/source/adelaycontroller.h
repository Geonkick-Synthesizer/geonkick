//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/adelay/source/adelaycontroller.h
// Created by  : Steinberg, 06/2009
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

#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

#if SMTG_OS_MACOS
#include <TargetConditionals.h>
#endif

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
class IDelayTestController : public FUnknown
{
public:
	virtual bool PLUGIN_API doTest () = 0;
//------------------------------------------------------------------------
	static const FUID iid;
};

DECLARE_CLASS_IID (IDelayTestController, 0x9FC98F39, 0x27234512, 0x84FBC4AD, 0x618A14FD)

//-----------------------------------------------------------------------------
class ADelayController : public EditController, public IDelayTestController
{
public:
//------------------------------------------------------------------------
	// create function required for plug-in factory,
	// it will be called to create new instances of this controller
//------------------------------------------------------------------------
	static FUnknown* createInstance (void*) { return (IEditController*)new ADelayController (); }

	//---from IPluginBase--------
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

//---from EditController-----
#if TARGET_OS_IPHONE
	IPlugView* PLUGIN_API createView (FIDString name) SMTG_OVERRIDE;
#endif
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;

	bool PLUGIN_API doTest () SMTG_OVERRIDE;

	//---Interface---------
	OBJ_METHODS (ADelayController, EditController)
	DEFINE_INTERFACES
		DEF_INTERFACE (IDelayTestController)
	END_DEFINE_INTERFACES (EditController)
	REFCOUNT_METHODS (EditController)
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
