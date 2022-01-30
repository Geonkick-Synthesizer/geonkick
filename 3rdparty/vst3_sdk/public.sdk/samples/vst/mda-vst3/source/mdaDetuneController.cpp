/*
 *  mdaDetuneController.cpp
 *  mda-vst3
 *
 *  Created by Arne Scheffler on 6/14/08.
 *
 *  mda VST Plug-ins
 *
 *  Copyright (c) 2008 Paul Kellett
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "mdaDetuneController.h"

#include "mdaDetuneController.h"
#include "pluginterfaces/base/ibstream.h"

namespace Steinberg {
namespace Vst {
namespace mda {

//-----------------------------------------------------------------------------
DetuneController::DetuneController ()
{
}

//-----------------------------------------------------------------------------
DetuneController::~DetuneController ()
{
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API DetuneController::initialize (FUnknown* context)
{
	tresult res = BaseController::initialize (context);
	if (res == kResultTrue)
	{
		parameters.addParameter (USTRING("Detune"), USTRING("cents"), 0, 0.4, ParameterInfo::kCanAutomate, kParam0);
		parameters.addParameter (new ScaledParameter (USTRING("Mix"), USTRING("%"), 0, 0.4, ParameterInfo::kCanAutomate, kParam1, 0, 99));
		parameters.addParameter (new ScaledParameter (USTRING("Output"), USTRING("dB"), 0, 0.5, ParameterInfo::kCanAutomate, kParam2, -20, 20));
		parameters.addParameter (USTRING("Latency"), USTRING("ms"), 0, 0.5, ParameterInfo::kCanAutomate, kParam3);
	}
	return res;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API DetuneController::terminate ()
{
	return BaseController::terminate ();
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API DetuneController::getParamStringByValue (ParamID tag, ParamValue valueNormalized, String128 string)
{
	return BaseController::getParamStringByValue (tag, valueNormalized, string);
	/*
	UString128 result;
		switch (tag)
		{
			#if 0
			case kParam3:
			{
				result.printFloat (bufres);
				break;
			}
			case kParam0:
			{
				result.printFloat (100*semi);
				break;
			}
			#endif
			default:
				return BaseController::getParamStringByValue (tag, valueNormalized, string);
		}
		result.copyTo (string, 128);
		return kResultTrue;*/
	
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API DetuneController::getParamValueByString (ParamID tag, TChar* string, ParamValue& valueNormalized)
{
	return BaseController::getParamValueByString (tag, string, valueNormalized);
/*
		switch (tag)
		{
			default:
				return BaseController::getParamValueByString (tag, string, valueNormalized);
		}
		return kResultFalse;*/
	
}

}}} // namespaces
