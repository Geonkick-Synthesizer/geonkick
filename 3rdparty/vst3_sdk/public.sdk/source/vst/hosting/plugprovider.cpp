//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/plugprovider.cpp
// Created by  : Steinberg, 08/2016
// Description : VST 3 Plug-in Provider class
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

#include "plugprovider.h"

#include "connectionproxy.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstmessage.h"

#include <cstdio>
#include <iostream>

static std::ostream* errorStream = &std::cout;

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// PlugProvider
//------------------------------------------------------------------------
PlugProvider::PlugProvider (const PluginFactory& factory, ClassInfo classInfo, bool plugIsGlobal)
: factory (factory)
, component (nullptr)
, controller (nullptr)
, classInfo (classInfo)
, plugIsGlobal (plugIsGlobal)
{
	if (plugIsGlobal)
	{
		setupPlugin (PluginContextFactory::instance ().getPluginContext ());
	}
}

//------------------------------------------------------------------------
PlugProvider::~PlugProvider ()
{
	terminatePlugin ();
}

//------------------------------------------------------------------------
IComponent* PLUGIN_API PlugProvider::getComponent ()
{
	if (!component)
		setupPlugin (PluginContextFactory::instance ().getPluginContext ());

	if (component)
		component->addRef ();

	return component;
}

//------------------------------------------------------------------------
IEditController* PLUGIN_API PlugProvider::getController ()
{
	if (controller)
		controller->addRef ();

	// 'iController == 0' is allowed! In this case the plug has no controller
	return controller;
}

//------------------------------------------------------------------------
IPluginFactory* PLUGIN_API PlugProvider::getPluginFactory ()
{
	if (auto f = factory.get ())
		return f.get ();
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProvider::getComponentUID (FUID& uid) const
{
	uid = FUID::fromTUID (classInfo.ID ().data ());
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProvider::releasePlugIn (IComponent* iComponent, IEditController* iController)
{
	if (iComponent)
		iComponent->release ();

	if (iController)
		iController->release ();

	if (!plugIsGlobal)
	{
		terminatePlugin ();
	}

	return kResultOk;
}

//------------------------------------------------------------------------
bool PlugProvider::setupPlugin (FUnknown* hostContext)
{
	bool res = false;

	//---create Plug-in here!--------------
	// create its component part
	component = factory.createInstance<IComponent> (classInfo.ID ());
	if (component)
	{
		// initialize the component with our context
		res = (component->initialize (hostContext) == kResultOk);

		// try to create the controller part from the component
		// (for Plug-ins which did not succeed to separate component from controller)
		if (component->queryInterface (IEditController::iid, (void**)&controller) != kResultTrue)
		{
			TUID controllerCID;

			// ask for the associated controller class ID
			if (component->getControllerClassId (controllerCID) == kResultTrue)
			{
				// create its controller part created from the factory
				controller = factory.createInstance<IEditController> (VST3::UID (controllerCID));
				if (controller)
				{
					// initialize the component with our context
					res = (controller->initialize (hostContext) == kResultOk);
				}
			}
		}
	}
	else if (errorStream)
	{
		*errorStream << "Failed to create instance of " << classInfo.name () << "!\n";
	}

	if (res)
		connectComponents ();

	return res;
}

//------------------------------------------------------------------------
bool PlugProvider::connectComponents ()
{
	if (!component || !controller)
		return false;

	FUnknownPtr<IConnectionPoint> compICP (component);
	FUnknownPtr<IConnectionPoint> contrICP (controller);
	if (!compICP || !contrICP)
		return false;

	bool res = false;

	componentCP = NEW ConnectionProxy (compICP);
	controllerCP = NEW ConnectionProxy (contrICP);

	if (componentCP->connect (contrICP) != kResultTrue)
	{
		// TODO: Alert or what for non conformant plugin ?
	}
	else
	{
		if (controllerCP->connect (compICP) != kResultTrue)
		{
			// TODO: Alert or what for non conformant plugin ?
		}
		else
			res = true;
	}
	return res;
}

//------------------------------------------------------------------------
bool PlugProvider::disconnectComponents ()
{
	if (!componentCP || !controllerCP)
		return false;

	bool res = componentCP->disconnect ();
	res &= controllerCP->disconnect ();

	componentCP = nullptr;
	controllerCP = nullptr;

	return res;
}

//------------------------------------------------------------------------
void PlugProvider::terminatePlugin ()
{
	disconnectComponents ();

	bool controllerIsComponent = false;
	if (component)
	{
		controllerIsComponent = FUnknownPtr<IEditController> (component).getInterface () != nullptr;
		component->terminate ();
	}

	if (controller && controllerIsComponent == false)
		controller->terminate ();

	component = nullptr;
	controller = nullptr;
}
}
} // namespaces
