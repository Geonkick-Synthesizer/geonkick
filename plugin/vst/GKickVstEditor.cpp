/**
 * File name: GKickVstEditor.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/geonkick)
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
        , guiApp(nullptr)
	    , mainWindow(nullptr)
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
        guiApp = std::make_unique<RkMain>();
        auto info = rk_from_native_win(nullptr, nullptr, reinterpret_cast<HWND>(parent));
        mainWindow = new MainWindow(info);
        if (!guiApp->setTopLevelWindow(mainWindow)) {
                RK_LOG_ERROR("can't create gui");
                guiApp.reset(nullptr);			
        } else {
			mainWindow->show();
			RK_LOG_INFO("show mainWindow");
		}
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

	RK_LOG_INFO("left   : " << newSize->left);
	RK_LOG_INFO("right  : " << newSize->right);
	RK_LOG_INFO("top    : " << newSize->top);
	RK_LOG_INFO("bottom : " << newSize->bottom);
	return kResultOk;
}

