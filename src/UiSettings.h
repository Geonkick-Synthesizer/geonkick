/**
 * File name: UiSettings.h
 * Project: Geonkick (A percussion synthesizer)
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

#ifndef GEONGKICK_SETTINGS_H
#define GEONGKICK_SETTINGS_H

#include "globals.h"
#include "ViewState.h"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class UiSettings {
 public:
        using View = ViewState::View;
        using Oscillator = GeonkickApi::OscillatorType;

        UiSettings();
        void fromJsonObject(const rapidjson::Value &obj);
        std::string toJson() const;
        View getMainView() const;
        void setMainView(View view);
        void setSamplesBrowserPath(const std::string &path);
        std::string samplesBrowserPath() const;
        void setSamplesBrowserPreviewFile(const std::string &file);
        std::string samplesBrowserPreviewFile() const;
        void setSamplesPreviewLimiter(double &value);
        double samplesPreviewLimiter() const;
        void setSamplesBrowserOscillator(UiSettings::Oscillator osc);
        UiSettings::Oscillator samplesBrowserOscillator() const;
        void setSettings(const std::string &key, const std::string &value);
        std::string getSettings(const std::string &key) const;
        bool showSidebar() const;

 protected:
        void parserSamplesBrowser(const rapidjson::Value &obj);

 private:
        struct SamplesBrowser {
                std::string currentDirectory;
                std::string previewFile;
                Oscillator oscillator;
                double previewLimiter;
        };

        View mainView;
        SamplesBrowser samplesBrowser;
        std::unordered_map<std::string, std::string> uiSettings;
};

#endif // GEONGKICK_UI_SETTINGS_H
