//-----------------------------------------------------------------------------
// Project     : SDK Core
// Version     : 1.0
//
// Category    : Common Base Classes
// Filename    : public.sdk/source/main/linuxmain.cpp
// Created by  : Steinberg, 03/2017
// Description : Linux Component Entry
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

#include "pluginterfaces/base/fplatform.h"

void* moduleHandle = nullptr;

//------------------------------------------------------------------------
bool InitModule ();		///< must be provided by plug-in: called when the library is loaded
bool DeinitModule ();	///< must be provided by plug-in: called when the library is unloaded

//------------------------------------------------------------------------
extern "C"
{
	/** must be provided by the plug-in! */
	SMTG_EXPORT_SYMBOL bool ModuleEntry (void*);
	SMTG_EXPORT_SYMBOL bool ModuleExit (void);
}

static int moduleCounter {0}; // counting for ModuleEntry/ModuleExit pairs

//------------------------------------------------------------------------
/** must be called from host right after loading dll
Note: this could be called more than one time! */
bool ModuleEntry (void* sharedLibraryHandle)
{
	if (++moduleCounter == 1)
	{
		moduleHandle = sharedLibraryHandle;
		return InitModule ();
	}
	return true;
}

//------------------------------------------------------------------------
/** must be called from host right before unloading dll
Note: this could be called more than one time! */
bool ModuleExit (void)
{
	if (--moduleCounter == 0)
	{
		moduleHandle = nullptr;
		return DeinitModule ();
	}
	else if (moduleCounter < 0)
		return false;
	return true;
}
