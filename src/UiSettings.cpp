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

#include "UiSettings.h"

UiSettings::UiSettings()
        : mainView{UiSettings::View::Controls}
        , samplesBrowser{"", "", UiSettings::Oscillator::Oscillator1}
{
}

void UiSettings::fromJsonObject(const rapidjson::Value &obj)
{
        for (const auto &m: obj.GetObject()) {
                if (m.name == "MainView" && m.value.IsInt())
                        mainView = static_cast<View>(m.value.GetInt());
                if (m.name == "SamplesBrowser" && m.value.IsObject())
                        parserSamplesBrowser(m.value);
        }
}

void UiSettings::parserSamplesBrowser(const rapidjson::Value &obj)
{
        for (const auto &m: obj.GetObject()) {
                if (m.name == "currentDirectory" && m.value.IsString())
                        samplesBrowser.currentDirectory = m.value.GetString();
                if (m.name == "previewFile" && m.value.IsString())
                        samplesBrowser.previewFile = m.value.GetString();
                if (m.name == "limiterValue" && m.value.IsDouble())
                        samplesBrowser.previewLimiter = m.value.GetDouble();
                if (m.name == "oscillator" && m.value.IsInt())
                        samplesBrowser.oscillator = static_cast<Oscillator>(m.value.GetInt());
                if (m.name == "settings" && m.value.IsArray()) {
                        for (const auto &el: m.value.GetArray()) {
                                if (el.IsArray() && el.GetArray().Size() == 2)
                                        uiSettings.insert({el.GetArray()[0].GetString(),
                                                           el.GetArray()[1].GetString()});
                        }
                }
        }
}

std::string UiSettings::toJson() const
{
        std::ostringstream jsonStream;
        jsonStream << std::fixed << std::setprecision(7);
        jsonStream << "{" << std::endl;
        jsonStream << "\"MainView\": " << static_cast<int>(mainView) << ", " << std::endl;
        jsonStream << "\"SamplesBrowser\": { " << std::endl;
        jsonStream << "    \"currentDirectory\": \"" << samplesBrowser.currentDirectory << "\"," << std::endl;
        jsonStream << "    \"previewFile\": \"" << samplesBrowser.previewFile << "\"," << std::endl;
        jsonStream << "    \"previewLimiter\": " << samplesBrowser.previewLimiter
                   << "," << std::endl;
        jsonStream << "    \"oscillator\": " << static_cast<int>(samplesBrowser.oscillator) << std::endl;
        jsonStream << "}," << std::endl;
        jsonStream << "\"settings\": [" << std::endl;
        bool first = true;
        for (const auto &e: uiSettings) {
                if (!first)
                        jsonStream << ", ";
                jsonStream << "[\"" << e.first << "\", \"" << e.second << "\"]";
                first = false;
        }
        jsonStream << "]" << std::endl;
        jsonStream << "}" << std::endl;
        return jsonStream.str();
}

UiSettings::View UiSettings::getMainView() const
{
        return mainView;
}

void UiSettings::setMainView(View view)
{
        mainView = view;
}

void UiSettings::setSamplesBrowserPath(const std::string &path)
{
        samplesBrowser.currentDirectory = path;
}

std::string UiSettings::samplesBrowserPath() const
{
        return samplesBrowser.currentDirectory;
}

void UiSettings::setSamplesBrowserPreviewFile(const std::string &file)
{
        samplesBrowser.previewFile = file;
}

std::string UiSettings::samplesBrowserPreviewFile() const
{
        return samplesBrowser.previewFile;
}

void UiSettings::setSamplesPreviewLimiter(double &value)
{
        samplesBrowser.previewLimiter = value;
}

double UiSettings::samplesPreviewLimiter() const
{
        return samplesBrowser.previewLimiter;
}

void UiSettings::setSamplesBrowserOscillator(UiSettings::Oscillator osc)
{
        samplesBrowser.oscillator = osc;
}

UiSettings::Oscillator UiSettings::samplesBrowserOscillator() const
{
        return samplesBrowser.oscillator;
}

void UiSettings::setSettings(const std::string &key, const std::string &value)
{
        uiSettings[key] = value;
}

std::string UiSettings::getSettings(const std::string &key) const
{
        if (auto it = uiSettings.find(key); it != uiSettings.end())
                return it->second;
        return std::string();
}
