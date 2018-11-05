/**
 * File name: geonkick_lv2.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include "mainwindow.h"

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include <QApplication>

#define APP_URI "http://geontime.com/geonkick"
#define APP_URI_UI "http://geontime.com/geonkick#ui"

class GeonkickLv2Plugin
{
public:
        enum class PortType: int {
                MidiIn       = 0
                LeftChannel  = 1;
                RightChannel = 2
        };

        GeonkickLv2Plugin()
                : geonkickApi(nullptr),
                  geonkickGui(nullptr),
                  leftChannel(nullptr),
                  rightChannel(nullptr),
                  midiIn(nullptr)
        {}
        ~GeonkickLv2Plugin()
        {
                if (!geonkickGui) {
                        delete geonkickGui;
                }

                if (!geonkickApi) {
                }
        }

        MainWindow* createGui()
        {
                if (!geonkickApi) {
                        geonkickGui = nullptr;
                } else {
                        geonkickGui = new MainWindow(geonkickApi);
                }

                return geonkickGui;
        }

        GeonkickApi* createApi()
        {
                geonkickApi = new GeonkickApi;
                if (!geonkickApi.init()) {
                        GEONKICK_LOG_ERROR("can't create api");
                        geonkickApi = nullptr;
                }

                return geonkickApi;
        }

        GeonkickApi* getApi()
        {
                return geonkickApi;
        }

        MainWindow* getGui()
        {
                return geonkickGui;
        }

private:
        GeonkickApi *geonkickApi;
        MainWindow  *geonkickGui;
}

/**
 * Funcitons for LV2 UI plugin.
 *
 * Creates and shows an instance of Geonkick GUI that takes
 * the geonkick API instance as a pointer.
 */

static LV2UI_Handle gkick_instantiate_ui(const LV2UI_Descriptor*   descriptor,
                                         const char*               plugin_uri,
                                         const char*               bundle_path,
                                         LV2UI_Write_Function      write_function,
                                         LV2UI_Controller          controller,
                                         LV2UI_Widget*             widget,
                                         const LV2_Feature* const* features)
{
        if (!QCoreApplication::instance()) {
                GEONKICK_LOG_ERROR("host doesn't provide a Qt application instance.");
                return NULL;
        }

        return static_cast<LV2UI_Handle>(GeonkickLv2PLugin::getGui());
}

static void gkick_cleanup_ui(LV2UI_Handle handle)
{
        GeonkickLv2PLugin::destroyGui();
}

static void gkick_port_event_ui(LV2UI_Handle ui,
                                uint32_t port_index,
                                uint32_t buffer_size,
                                uint32_t format,
                                const void *buffer )
{
}

static const LV2UI_Descriptor gkick_descriptor_ui = {
	APP_URI_UI,
	gkick_instantiate_ui,
	gkick_cleanup_ui,
	gkick_port_event_ui,
	NULL
};

const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &gkick_descriptor_ui;
	default:
		return NULL;
        }
}

/**
 * Functions for LV2 plugin.
 *
 * The Geonkick API instance is loaded alone.
 */

static LV2_Handle gkick_instantiate(const LV2_Descriptor*     descriptor,
                                    double                    rate,
                                    const char*               bundle_path,
                                    const LV2_Feature* const* features)
{
        auto api = new GeonkickApi;
	return static_cast<LV2_Handle>(api);
}

static void gkick_connect_port(LV2_Handle instance,
                               uint32_t   port,
                               void*      data)
{
        auto portType = static_cast<GeonKickLv2Plugin::PortType>(port);
        switch (static_cast<PortType>(port))
        {
        case GeonKickLv2Plugin::PortType::LeftChannel:
        case GeonKickLv2Plugin::PortType::RightChannel:
                geonkickLv2Plugin->setChannel(static_cast<const float*>(data), portType);
                break;
        case GeonKickLv2Plugin::PortType::MidiIn:
                geonkickLv2Plugin->setMidiIn(static_cast<const LV2_Atom_Sequence*>(data));
                break;
        }
}

static void gkick_activate(LV2_Handle instance)
{
}

static void gkick_run(LV2_Handle instance, uint32_t n_samples)
{
        for (auto i = 0; i < n_samples; i++) {
                geonkickLv2Plugin->setOutputValue(geonkickApi->getNexBufferValue());
        }
}

static void gkick_deactivate(LV2_Handle instance)
{
}

static void gkick_cleaup(LV2_Handle instance)
{
        delete static_cast<GeonkickApi*>(instance);
}

static const LV2_Descriptor gkick_descriptor = {
	APP_URI,
	gkick_instantiate,
	gkick_connect_port,
	gkick_activate,
	gkick_run,
	gkick_deactivate,
	gkick_cleaup,
	NULL
};

const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:  return &gkick_descriptor;
	default: return NULL;
	}
}
