/**
 * File name: GKickVstEditor.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor
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
#include "MainWindow.h"
#include "GKickVstEditor.h"

#include <RkPlatform.h>
#include <RkMain.h>

#ifdef GEONKICK_OS_GNU
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

#endif // GEONKICK_OS_GNU

GKickVstEditor::GKickVstEditor(Vst::EditController *controller, GeonkickApi *api)
        : Vst::EditorView(controller)
        , guiApp{nullptr}
        , mainWindow{nullptr}
        , geonkickApi{api}
#ifdef GEONKICK_OS_GNU
        , loopTimer{nullptr}
#endif // GEONKICK_OS_GNU
{
}

tresult PLUGIN_API
GKickVstEditor::isPlatformTypeSupported([[maybe_unused]]Steinberg::FIDString type)
{
        return kResultTrue;
}

tresult PLUGIN_API
GKickVstEditor::attached(void* parent, FIDString type)
{
        guiApp = std::make_unique<RkMain>();
#ifdef GEONKICK_OS_GNU
        loopTimer = std::make_unique<GKickVstTimer>(guiApp.get());
#endif // GEONKICK_OS_GNU
        
        geonkickApi->setEventQueue(guiApp->eventQueue());

#ifdef GEONKICK_OS_WINDOWS
        auto info = rk_from_native_win(reinterpret_cast<HWND>(parent),
                                       rk_win_api_instance(),
                                       rk_win_api_class_name());
#else // GEONKICK_OS_GNU
        auto xDisplay = XOpenDisplay(nullptr);
        if (!xDisplay) {
                GEONKICK_LOG_ERROR("can't open display");
                return kResultFalse;
        }
        auto screenNumber = DefaultScreen(xDisplay);
        auto info = rk_from_native_x11(xDisplay, screenNumber, reinterpret_cast<Window>(parent));
#endif // GEONKICK_OS_GNU

        mainWindow = new MainWindow(guiApp.get(), geonkickApi, info);
        mainWindow->show();
        if (!mainWindow->init()) {
                GEONKICK_LOG_ERROR("can't init main window");
                return kResultFalse;
        }

#ifdef GEONKICK_OS_GNU
        IRunLoop* loop = nullptr;
        if (plugFrame->queryInterface(IRunLoop::iid, (void**)&loop) == Steinberg::kResultOk) {
                loop->registerTimer(loopTimer.get(), 1);
        } else {
                GEONKICK_LOG_ERROR("can't get loop");
                return kResultFalse;
        }
#endif // GEONKICK_OS_GNU
        return Vst::EditorView::attached(parent, type);
}

tresult PLUGIN_API
GKickVstEditor::removed()
{
#ifdef GEONKICK_OS_GNU        
        IRunLoop* loop = nullptr;
        if (plugFrame->queryInterface(IRunLoop::iid, (void**)&loop) == Steinberg::kResultOk) {
                loop->unregisterTimer(loopTimer.get());
        } else {
                GEONKICK_LOG_ERROR("can't get loop");
                return kResultFalse;
        }
#endif // GEONKICK_OS_GNU
        
        if (guiApp)
                guiApp = nullptr;
        return kResultOk;
}

tresult PLUGIN_API
GKickVstEditor::getSize(ViewRect* newSize)
{
        if (newSize == nullptr)
		return kResultFalse;

        auto winRect = MainWindow::getWindowSize();
	newSize->left   = winRect.left();
	newSize->right  = winRect.right();
	newSize->top    = winRect.top();
	newSize->bottom = winRect.bottom();
	return kResultOk;
}
