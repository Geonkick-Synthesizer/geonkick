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

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <lv2/lv2plug.in/ns/ext/instance-access/instance-access.h>
#include <lv2/lv2plug.in/ns/ext/state/state.h>

#include "mainwindow.h"
#include "geonkick_api.h"
#include "geonkick_state.h"

#define GEONKICK_URI "http://geontime.com/geonkick"
#define GEONKICK_URI_UI "http://geontime.com/geonkick#ui"
#define GEONKICK_URI_STATE "http://geontime.com/geonkick#state"

class GeonkickLv2Plugin
{
  public:
        enum class PortType: int {
                MidiIn       = 0,
                LeftChannel  = 1,
                RightChannel = 2
        };

        GeonkickLv2Plugin()
                : geonkickApi(new GeonkickApi),
                  midiIn(nullptr),
                  leftChannel(nullptr),
                  rightChannel(nullptr)
        {
        }

        bool init()
        {
                return geonkickApi->init();
        }

        ~GeonkickLv2Plugin()
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

        void setMidiIn(LV2_Atom_Sequence *data)
        {
                midiIn = data;
        }

        void setStateId(LV2_URID id)
        {
                stateId = id;
        }

        LV2_URID getStateId() const
        {
                return stateId;
        }

        void setAtomChunkId(LV2_URID id)
        {
                atomStringId = id;
        }

        LV2_URID getAtomChunkId()
        {
                return atomStringId;
        }

        void setStateData(const QByteArray &data, int flags = 0)
        {
                Q_UNUSED(flags);
                geonkickApi->setState(data);
        }

        const QByteArray getStateData()
        {
                return geonkickApi->getState()->toRawData();
        }

        GeonkickApi* getApi() const
        {
                return geonkickApi;
        }

        void processSamples(int nsamples) const
        {
                if (!midiIn) {
                        return;
                }

                auto it = lv2_atom_sequence_begin(&midiIn->body);
                for (auto i = 0; i < nsamples; i++) {
                        if (it->time.frames == i && !lv2_atom_sequence_is_end(&midiIn->body, midiIn->atom.size, it)) {
                                const uint8_t* const msg = (const uint8_t*)(it + 1);
                                switch (lv2_midi_message_type(msg))
                                {
                                case LV2_MIDI_MSG_NOTE_ON:
                                        GEONKICK_LOG_ERROR("LV2_MIDI_MSG_NOTE_ON");
                                        geonkickApi->setKeyPressed(true, msg[2]);
                                                break;
                                case LV2_MIDI_MSG_NOTE_OFF:
                                        GEONKICK_LOG_ERROR("LV2_MIDI_MSG_NOTE_OFF");
                                        geonkickApi->setKeyPressed(false, msg[2]);
                                                break;
                                default:
                                        break;
                                }
                                it = lv2_atom_sequence_next(it);
                        }
                        auto val = geonkickApi->getAudioFrame();
                        leftChannel[i]  = val;
                        rightChannel[i] = val;
                }
        }

private:
        GeonkickApi *geonkickApi;
        LV2_Atom_Sequence* midiIn;
        float *leftChannel;
        float *rightChannel;
        LV2_URID stateId;
        LV2_URID atomStringId;
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

        const LV2_Feature *feature;
        while (feature = *features) {
                if (QByteArray(feature->URI) == QByteArray(LV2_INSTANCE_ACCESS_URI)) {
                        auto geonkickLv2PLugin = static_cast<GeonkickLv2Plugin*>(feature->data);
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
}

static const LV2UI_Descriptor gkick_descriptor_ui = {
	GEONKICK_URI_UI,
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
        auto geonkickLv2PLugin = new GeonkickLv2Plugin;
        if (!geonkickLv2PLugin->init()) {
                GEONKICK_LOG_DEBUG("can't init plugin");
                delete geonkickLv2PLugin;
                return NULL;
        }

        const LV2_Feature *feature;
        while (feature = *features) {
                if (QByteArray(feature->URI) == QByteArray(LV2_URID__map)) {
                        auto uridMap = static_cast<LV2_URID_Map*>(feature->data);
                        if (uridMap && uridMap->map && uridMap->handle) {
                                geonkickLv2PLugin->setStateId(uridMap->map(uridMap->handle, GEONKICK_URI_STATE));
                                geonkickLv2PLugin->setAtomChunkId(uridMap->map(uridMap->handle, LV2_ATOM__Chunk));
                        }
                        break;
                }
                features++;
        }


        return static_cast<LV2_Handle>(geonkickLv2PLugin);
}

static void gkick_connect_port(LV2_Handle instance,
                               uint32_t   port,
                               void*      data)
{
        auto geonkickLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
        auto portType = static_cast<GeonkickLv2Plugin::PortType>(port);
        switch (portType)
        {
        case GeonkickLv2Plugin::PortType::LeftChannel:
        case GeonkickLv2Plugin::PortType::RightChannel:
                geonkickLv2PLugin->setAudioChannel(static_cast<float*>(data), portType);
                break;
        case GeonkickLv2Plugin::PortType::MidiIn:
                geonkickLv2PLugin->setMidiIn(static_cast<LV2_Atom_Sequence*>(data));
                break;
        }
}

static void gkick_activate(LV2_Handle instance)
{
        qDebug() << __PRETTY_FUNCTION__ << "instance: " << instance;
}

static void gkick_run(LV2_Handle instance, uint32_t n_samples)
{
       auto geonkickLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
       geonkickLv2PLugin->processSamples(n_samples);
}

static void gkick_deactivate(LV2_Handle instance)
{
        qDebug() << __PRETTY_FUNCTION__ << "instance: " << instance;
}

static void gkick_cleaup(LV2_Handle instance)
{
        qDebug() << __PRETTY_FUNCTION__ << "instance: " << instance;
        delete static_cast<GeonkickLv2Plugin*>(instance);
}

static LV2_State_Status
gkick_state_save(LV2_Handle                instance,
                 LV2_State_Store_Function  store,
                 LV2_State_Handle          handle,
                 uint32_t                  flags,
                 const LV2_Feature* const* features)
{
        qDebug() << __PRETTY_FUNCTION__;
        auto geonkickLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
        if (geonkickLv2PLugin){
                QByteArray stateData = geonkickLv2PLugin->getStateData();
                store(handle, geonkickLv2PLugin->getStateId(), stateData.data(),
                      stateData.size(), geonkickLv2PLugin->getAtomChunkId(),
                      LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE);
                GEONKICK_LOG_DEBUG(QString("id:") << geonkickLv2PLugin->getStateId());
                GEONKICK_LOG_DEBUG(QString("size:") << stateData.size());
                GEONKICK_LOG_DEBUG(QString("geonkickLv2PLugin->getAtomStringId():") << geonkickLv2PLugin->getAtomChunkId());
        }

        return LV2_STATE_SUCCESS;
}

static LV2_State_Status
gkick_state_restore(LV2_Handle                  instance,
                    LV2_State_Retrieve_Function retrieve,
                    LV2_State_Handle            handle,
                    uint32_t                    flags,
                    const LV2_Feature* const*   features)
{
        qDebug() << __PRETTY_FUNCTION__;
        auto geonkickLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
        if (geonkickLv2PLugin){
                size_t size   = 0;
                LV2_URID type = 0;
                const char *data = (const char*)retrieve(handle, geonkickLv2PLugin->getStateId(),
                                            &size, &type, &flags);
                GEONKICK_LOG_DEBUG(QString("id:") << geonkickLv2PLugin->getStateId());
                GEONKICK_LOG_DEBUG(QString("size:") << size << ", type : " << type);
                GEONKICK_LOG_DEBUG(QString("geonkickLv2PLugin->getAtomStringId():") << geonkickLv2PLugin->getAtomChunkId());
                geonkickLv2PLugin->setStateData(QByteArray(data, size), flags);
        }
}

static const void* gkick_extention_data(const char* uri)
{
        static const LV2_State_Interface state = {gkick_state_save, gkick_state_restore};
        if (QByteArray(uri) == QByteArray(LV2_STATE__interface)) {
                GEONKICK_LOG_INFO("called");
                return &state;
        }
        return NULL;
}

static const LV2_Descriptor gkick_descriptor = {
	GEONKICK_URI,
	gkick_instantiate,
	gkick_connect_port,
	gkick_activate,
	gkick_run,
	gkick_deactivate,
	gkick_cleaup,
	gkick_extention_data,
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
