/**
 * File name: ViewState.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor 
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

#ifndef GEONGKICK_VIEW_STATE_H
#define GEONGKICK_VIEW_STATE_H

#include "globals.h"
#include "geonkick_api.h"
#include "envelope.h"

#include <RkObject.h>

class ViewState: public RkObject {
 public:
        enum class View : int {
                Controls = 0,
                Kit      = 1,
                Presets  = 2,
                Samples  = 3,
        };

        using Oscillator = GeonkickApi::OscillatorType;

        ViewState(RkObject *parent);
        RK_DECL_ACT(mainViewChanged,
                    mainViewChanged(ViewState::View view),
                    RK_ARG_TYPE(ViewState::View),
                    RK_ARG_VAL(view));
        RK_DECL_ACT(samplesBrowserPathChanged,
                    samplesBrowserPathChanged(const std::string &path),
                    RK_ARG_TYPE(const std::string&),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(samplesBrowserOscChanged,
                    samplesBrowserOscChanged(ViewState::Oscillator osc),
                    RK_ARG_TYPE(ViewState::Oscillator),
                    RK_ARG_VAL(osc));
        RK_DECL_ACT(envelopeCategoryChanged,
                    envelopeCategoryChanged(Envelope::Category category),
                    RK_ARG_TYPE(Envelope::Category),
                    RK_ARG_VAL(category));
        RK_DECL_ACT(envelopeTypeChanged,
                    envelopeTypeChanged(Envelope::Type envelope),
                    RK_ARG_TYPE(Envelope::Type),
                    RK_ARG_VAL(envelope));
        RK_DECL_ACT(envelopeChanged,
                    envelopeChanged(Envelope::Category category, Envelope::Type envelope),
                    RK_ARG_TYPE(Envelope::Category, Envelope::Type),
                    RK_ARG_VAL(category, envelope));


        ViewState::View getMainView() const;
        void setMainView(ViewState::View view);
        void setSamplesBrowserPath(const std::string &path);
        void setSamplesBrowserPreviewFile(const std::string &file);
        std::string samplesBrowserPath() const;
        std::string samplesBrowserPreviewFile() const;
        void setSamplesBrowserOscillator(Oscillator osc);
        Oscillator samplesBrowserOscillator() const;
        void setEnvelope(Envelope::Category category, Envelope::Type envelope);
        void setEnvelopeCategory(Envelope::Category category);
        Envelope::Category getEnvelopeCategory() const;
        void setEnvelopeType(Envelope::Type envelope);
        Envelope::Type getEnvelopeType() const;

 private:
        struct SamplesBrowser {
                std::string currentDirectory;
                std::string previewFile;
                Oscillator oscillator;
        };

        ViewState::View mainView;
        SamplesBrowser samplesBrowser;
        Envelope::Type envelopeType;
        Envelope::Category envelopeCategory;
};

#endif // GEONGKICK_VIEW_STATE_H
