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

//#include "mainwindow.h"

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include "lv2/lv2plug.in/ns/ext/instance-access/instance-access.h"

#include "mainwindow.h"
#include "geonkick_api.h"

#include <QDebug>

#define APP_URI "http://geontime.com/geonkick"
#define APP_URI_UI "http://geontime.com/geonkick#ui"

class GeonkickLv2PLugin
{
  public:
        enum class PortType: int {
                MidiIn       = 0,
                LeftChannel  = 1,
                RightChannel = 2
        };

        GeonkickLv2PLugin()
                : geonkickApi(new GeonkickApi),
                  midiIn(nullptr),
                  leftChannel(nullptr),
                  rightChannel(nullptr)
        {
        }

        ~GeonkickLv2PLugin()
        {
                if (geonkickApi) {
                        delete geonkickApi;
                }
        }

        void setAudioChannel(float *data, PortType port)
        {
                if (port == PortType::LeftChannel)
                        leftChannel = data;
                else
                        rightChannel = data;
        }

        void setMidiIn(const LV2_Atom_Sequence *data)
        {
                midiIn = data;
        }

        GeonkickApi* getApi()
        {
                return geonkickApi;
        }

        void processSamples(int nsamples)
        {
                if (!midiIn) {
                        return;
                }

                auto it = lv2_atom_sequence_begin(&midiIn->body);
                for (auto i = 0; i < nsamples; i++) {
                        if (it->time.frame == i && !lv2_atom_sequence_is_end(&midiIn->body, midiIn->atom.size, it)) {
                                const uint8_t* const msg = (const uint8_t*)(ev + 1);
                                switch (lv2_midi_message_type(msg))
                                {
                                case LV2_MIDI_MSG_NOTE_ON:
                                                geonkickApi->setKeyPressed(true);
                                case LV2_MIDI_MSG_NOTE_OFF:
                                                geonkickApi->setKeyPressed(false);
                                }
                                it = lv2_atom_sequence_next(it);
                        }
                        auto val = geonkickApi->getAudioFrame();
                        leftChannel[i]  = val;
                        rightChannel[i] = val;
                }
        }

pivate:
        GeonkickApi *geonkickApi;
        LV2_Atom_Sequence* midiIn;
        float *leftChannel;
        float *rightChannel;
};

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
        qDebug() << __PRETTY_FUNCTION__;
        MainWindow *mainWindow = nullptr;
        if (!features) {
                return NULL;
        }

        const LV2_Feature *feature = *features;
        while (feature != nullptr) {
                if (QByteArray(feature->URI) == QByteArray(LV2_INSTANCE_ACCESS_URI)) {
                        auto geonkickLv2PLugin = static_cast<GeonkickLv2PLugin*>(feature->data);
                        mainWindow = new MainWindow(geonkickLv2PLugin->getApi());
                        if (!mainWindow->init()) {
                                GEONKICK_LOG_ERROR("can't init MainWindow");
                                delete mainWindow;
                                return nullptr;
                        } else {
                                mainWindow->show();
                        }
                        break;
                }
                features++;
        }

        *widget = mainWindow;
        return mainWindow;
}

static void gkick_cleanup_ui(LV2UI_Handle handle)
{
        qDebug() << __PRETTY_FUNCTION__;
        if (handle) {
                delete static_cast<MainWindow*>(handle);
        }
}

static void gkick_port_event_ui(LV2UI_Handle ui,
                                uint32_t port_index,
                                uint32_t buffer_size,
                                uint32_t format,
                                const void *buffer )
{
        qDebug() << __PRETTY_FUNCTION__;
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
        qDebug() << __PRETTY_FUNCTION__;
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
        qDebug() << __PRETTY_FUNCTION__ ;;
        geonkickLv2PLugin = new GeonkickLv2Plugin;
        if (!geonkickLv2PLugin->init()) {
                GEONKICK_LOG_DEBUG("can't init plugin");
                delete geonkickLv2PLugin;
                return NULL;
        }
        return static_cast<LV2_Handle>(geonkickLv2PLugin);
}

static void gkick_connect_port(LV2_Handle instance,
                               uint32_t   port,
                               void*      data)
{
        qDebug() << __PRETTY_FUNCTION__ << "instance: " << instance;
        auto geonkickLv2PLugin = static_cast<GeonkickLv2PLugin*>(instance);
        switch (static_cast<PortType>(port))
        {
        case GeonkickLv2PLugin::PortType::LeftChannel:
        case GeonkickLv2PLugin::PortType::RightChannel:
                geonkickLv2PLugin->setAudioChannel(static_cast<float*>(data), portType);
                break;
        case GeonKickApi::PortType::MidiIn:
                geonkickLv2PLugin->setMidiIn(static_cast<const LV2_Atom_Sequence*>(data));
                break;
        }
}

static void gkick_activate(LV2_Handle instance)
{
        qDebug() << __PRETTY_FUNCTION__ << "instance: " << instance;
}

static void gkick_run(LV2_Handle instance, uint32_t n_samples)
{
       auto geonkickLv2PLugin = static_cast<GeonkickLv2PLugin*>(instance);
       geonkickLv2PLugin->processSamples(n_samples);
}

static void gkick_deactivate(LV2_Handle instance)
{
        qDebug() << __PRETTY_FUNCTION__ << "instance: " << instance;
}

static void gkick_cleaup(LV2_Handle instance)
{
        qDebug() << __PRETTY_FUNCTION__ << "instance: " << instance;
        delete static_cast<GeonkickLv2PLugin*>(instance);
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
        qDebug() << __PRETTY_FUNCTION__ << "index: " << index;
	switch (index)
        {
	case 0:  return &gkick_descriptor;
	default: return NULL;
	}
}
