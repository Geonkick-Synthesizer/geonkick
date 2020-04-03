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

#include "GKickVstEditor.h"
#include "mainwindow.h"

#include <RkMain.h>
#include <RkPlatform.h>

GKickVstEditor::GKickVstEditor(Vst::EditController *controller)
        : Vst::EditorView(controller)
        , guiApp{nullptr}
        , mainWindow{nullptr}
{
}

GKickVstEditor::~GKickVstEditor()
{
}

tresult PLUGIN_API GKickVstEditor::isPlatformTypeSupported(Steinberg::FIDString type)
{
        RK_UNUSED(type);
        return kResultTrue;
}

tresult PLUGIN_API GKickVstEditor::attached(void* parent, FIDString type)
{
        GEONKICK_LOG_INFO("called");
        guiApp = std::make_unique<RkMain>();
#ifdef RK_OS_WIN
        auto info = rk_from_native_win(nullptr, nullptr, reinterpret_cast<HWND>(parent));
#elif RK_OS_GNU
        //        auto info = rk_from_native_x11(nullptr, nullptr, reinterpret_cast<Window>(*parent));
#else
#error operating system not defined
#endif // RK_OS_GNU
        //        mainWindow = new MainWindow(guiApp->get(), info);
        //        mainWindow->show();
        return Vst::EditorView::attached(parent, type);
}

tresult PLUGIN_API GKickVstEditor::removed()
{
        if (guiApp)
                guiApp.reset(nullptr);
        return kResultOk;
}

tresult PLUGIN_API GKickVstEditor::getSize(ViewRect* newSize)
{
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

