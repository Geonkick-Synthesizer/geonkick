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
        GEONKICK_LOG_INFO("called");
}

tresult PLUGIN_API GKickVstEditor::isPlatformTypeSupported(Steinberg::FIDString type)
{
        GEONKICK_LOG_INFO("called");
        RK_UNUSED(type);
        return kResultTrue;
}

tresult PLUGIN_API GKickVstEditor::attached(void* parent, FIDString type)
{
        guiApp = new RkMain();
        loopTimer = std::move(std::make_unique<GKickVstTimer>(guiApp));
        geonkickApi->setEventQueue(guiApp->eventQueue().get());
#ifdef RK_OS_WIN
        //        auto info = rk_from_native_win(nullptr, nullptr, reinterpret_cast<HWND>(parent));
#elif RK_OS_GNU
        //        auto info = rk_from_native_x11(nullptr, nullptr, reinterpret_cast<Window>(*parent));
#else
#error operating system not defined
#endif // RK_OS_GNU
        Display* xDisplay = XOpenDisplay(nullptr);
        int screenNumber = DefaultScreen(xDisplay);
        auto info = rk_from_native_x11(xDisplay, screenNumber, reinterpret_cast<Window>(parent));
        mainWindow = new MainWindow(guiApp, geonkickApi, info);
        mainWindow->show();
        if (!mainWindow->init()) {
                GEONKICK_LOG_ERROR("can't init main window");
        }

        IRunLoop* loop = nullptr;
        if (plugFrame->queryInterface(IRunLoop::iid, (void**)&loop) == Steinberg::kResultOk) {
                loop->registerTimer(loopTimer.get(), 30);
        } else {
                GEONKICK_LOG_INFO("can't get loop");
        }
        return Vst::EditorView::attached(parent, type);
}

tresult PLUGIN_API GKickVstEditor::removed()
{
        GEONKICK_LOG_INFO("called");
        IRunLoop* loop = nullptr;
        if (plugFrame->queryInterface(IRunLoop::iid, (void**)&loop) == Steinberg::kResultOk) {
                loop->unregisterTimer(loopTimer.get());
        } else {
                GEONKICK_LOG_INFO("can't get loop");
        }
        if (guiApp)
                delete guiApp;
        return kResultOk;
}

tresult PLUGIN_API GKickVstEditor::getSize(ViewRect* newSize)
{
        GEONKICK_LOG_INFO("called");
	if (newSize == nullptr || mainWindow == nullptr)
		return kResultFalse;

	newSize->left   = mainWindow->x();
	newSize->right  = mainWindow->x() + mainWindow->width();
	newSize->top    = mainWindow->y();
	newSize->bottom = mainWindow->y() + mainWindow->height();

	GEONKICK_LOG_INFO("left   : " << newSize->left);
	GEONKICK_LOG_INFO("right  : " << newSize->right);
	GEONKICK_LOG_INFO("top    : " << newSize->top);
	GEONKICK_LOG_INFO("bottom : " << newSize->bottom);
	return kResultOk;
}

