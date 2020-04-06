/**
 * File name: GKickVstEditor.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor <http://goentime.com>
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

#include "geonkick_api.h"
#include "mainwindow.h"
#include "GKickVstEditor.h"

#include <RkPlatform.h>
#include <RkMain.h>

GKickVstTimer::GKickVstTimer(RkMain *app)
        : guiApp{app}
        , countT{1}
{
}

void PLUGIN_API
GKickVstTimer::onTimer()
{
        if (guiApp)
                guiApp->exec(false);
}

GKickVstEditor::GKickVstEditor(Vst::EditController *controller, GeonkickApi *api)
        : Vst::EditorView(controller)
        , guiApp{nullptr}
        , mainWindow{nullptr}
        , geonkickApi{api}
        , loopTimer{nullptr}
{
}

tresult PLUGIN_API
GKickVstEditor::isPlatformTypeSupported(Steinberg::FIDString type)
{
        RK_UNUSED(type);
        return kResultTrue;
}

tresult PLUGIN_API
GKickVstEditor::attached(void* parent, FIDString type)
{
        guiApp = std::make_unique<RkMain>();
        loopTimer = std::make_unique<GKickVstTimer>(guiApp.get());
        geonkickApi->setEventQueue(guiApp->eventQueue().get());
        Display* xDisplay = XOpenDisplay(nullptr);
        int screenNumber = DefaultScreen(xDisplay);
        auto info = rk_from_native_x11(xDisplay, screenNumber, reinterpret_cast<Window>(parent));
        mainWindow = new MainWindow(guiApp.get(), geonkickApi, info);
        mainWindow->show();
        if (!mainWindow->init()) {
                GEONKICK_LOG_ERROR("can't init main window");
                return kResultFalse;
        }

        IRunLoop* loop = nullptr;
        if (plugFrame->queryInterface(IRunLoop::iid, (void**)&loop) == Steinberg::kResultOk) {
                loop->registerTimer(loopTimer.get(), 30);
        } else {
                GEONKICK_LOG_INFO("can't get loop");
                return kResultFalse;
        }
        return Vst::EditorView::attached(parent, type);
}

tresult PLUGIN_API
GKickVstEditor::removed()
{
        IRunLoop* loop = nullptr;
        if (plugFrame->queryInterface(IRunLoop::iid, (void**)&loop) == Steinberg::kResultOk) {
                loop->unregisterTimer(loopTimer.get());
        } else {
                GEONKICK_LOG_INFO("can't get loop");
                return kResultFalse;
        }
        if (guiApp)
                guiApp = nullptr;
        return kResultOk;
}

tresult PLUGIN_API
GKickVstEditor::getSize(ViewRect* newSize)
{
	if (newSize == nullptr || mainWindow == nullptr)
		return kResultFalse;

	newSize->left   = mainWindow->x();
	newSize->right  = mainWindow->x() + mainWindow->width();
	newSize->top    = mainWindow->y();
	newSize->bottom = mainWindow->y() + mainWindow->height();
	return kResultOk;
}
