/**
 * File name: GKickVstEditor.h
 * Project: Geonkick (A kick synthesizer)
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

#ifndef GKICK_VST_EDITOR_H
#define GKICK_VST_EDITOR_H

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/gui/iplugview.h"

#include <RkEvent.h>

#include <memory>

using namespace Steinberg;

#ifdef GEONKICK_OS_GNU
using namespace Linux;
#endif // GEONKICK_OS_GNU

class MainWindow;
class GeonkickApi;
class RkMain;

#ifdef GEONKICK_OS_GNU
class GKickVstTimer: public ITimerHandler {
 public:
        explicit GKickVstTimer(RkMain *app);
        virtual ~GKickVstTimer() = default;
        tresult PLUGIN_API queryInterface (const TUID _iid, void** obj) override { return 0; }
        uint32 PLUGIN_API addRef() override { return countT++; }
        uint32 PLUGIN_API release() override { return --countT; }

 protected:
        void PLUGIN_API onTimer() override;

 private:
        RkMain *guiApp;
        uint32 countT;
};
#endif // GEONKICK_OS_GNU


class GKickVstEditor : public Vst::EditorView {
 public:
        GKickVstEditor(Vst::EditController *controller, GeonkickApi *api);
        tresult PLUGIN_API isPlatformTypeSupported(Steinberg::FIDString type) override;
        tresult PLUGIN_API attached(void* parent, FIDString type) override;
        tresult PLUGIN_API removed() override;
        tresult PLUGIN_API getSize(ViewRect* newSize) override;

#ifndef GEONKICK_OS_GNU
 protected:
        tresult processKey(RkEvent::Type keyType, char16 key);
        tresult onKeyDown(char16 key, int16 keyCode, int16 modifiers) override;
        tresult onKeyUp(char16 key, int16 keyCode, int16 modifiers) override;
#endif // GEONKICK_OS_GNU

 private:
        std::unique_ptr<RkMain> guiApp;
        MainWindow *mainWindow;
        GeonkickApi *geonkickApi;
#ifdef GEONKICK_OS_GNU
        std::unique_ptr<GKickVstTimer> loopTimer;
#endif // GEONKICK_OS_GNU
};

#endif // GKICK_VST_EDITOR_H
