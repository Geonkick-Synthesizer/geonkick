/**
 * File name: ExportToSfz.cpp
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

#include "ExportToSfz.h"
#include "ExportSoundData.h"
#include "kit_model.h"
#include "percussion_model.h"

ExportToSfz::ExportToSfz(const KitModel *model, const std::filesystem::path &file)
        : ExportAbstract(file, ExportFormat::Sfz)
        , kitModel{model}
{
}

bool ExportToSfz::doExport()
{
        std::ostringstream sfzStream;
        sfzStream << "// Exported from " << Geonkick::appName
            << " version " << Geonkick::applicationVersionStr << std::endl;
        sfzStream << "// Name: " << kitModel->name() << std::endl;
        sfzStream << "// Author: " << kitModel->author() << std::endl;
        sfzStream << "// License: " << kitModel->license() << std::endl;
        sfzStream << "// Instruments:" << std::endl;
        for (const auto &per : kitModel->percussionModels())
                sfzStream << "//    * " << per->name() << std::endl;
        sfzStream << std::endl;
        sfzStream << "<control>" << std::endl;
        sfzStream << "default_path=" << (dataPath() / std::filesystem::path()).string() << std::endl;
        sfzStream << "<global>" << std::endl;
        sfzStream << "ampeg_attack=0.001" << std::endl;
        sfzStream << "ampeg_release=3" << std::endl;
        sfzStream << "ampeg_dynamic=1" << std::endl;
        sfzStream << "volume=0" << std::endl << std::endl;
        sfzStream << "<group>" << std::endl;
        sfzStream << "seq_length=2" << std::endl;
        sfzStream << "seq_position=1" << std::endl << std::endl;

        try {
                if (!std::filesystem::exists(dataPath())) {
                        if (!std::filesystem::create_directories(dataPath())) {
                                GEONKICK_LOG_ERROR("can't create path " << dataPath());
                                return false;
                        }
                }
        } catch(const std::exception& e) {
                GEONKICK_LOG_ERROR("error on setup user data paths: " << e.what());
                return false;
        }

        for (const auto &instrument : kitModel->percussionModels()) {
                sfzStream << "<region>" << std::endl;
                std::filesystem::path wavFile = dataPath()
                        / std::filesystem::path(cleanName(instrument->name()) + ".wav");
                sfzStream << "sample=" <<  wavFile.filename().string() << std::endl;
                sfzStream << "lokey=" << static_cast<int>(instrument->key()) << std::endl;
                sfzStream << "hikey=" << static_cast<int>(instrument->key()) << std::endl;
                sfzStream << "pitch_keycenter=" << static_cast<int>(instrument->key()) << std::endl;
                sfzStream << "lovel=0" << std::endl;
                sfzStream << "hivel=127" << std::endl;
                sfzStream << "volume=15" << std::endl;
		sfzStream << std::endl;
                ExportSoundData exportToWav(wavFile,
                                            instrument->data(),
                                            ExportSoundData::ExportFormat::Wav);
                exportToWav.setBitDepth(32);
                exportToWav.setSampleRate(kitModel->api()->getSampleRate());
                exportToWav.doExport();
        }

        auto sfzFilePath = getExportPath();
        if (!sfzFilePath.has_extension() && Geonkick::toLower(sfzFilePath.extension().string()) != ".sfz")
                sfzFilePath += ".sfz";
        std::ofstream sfzFile (sfzFilePath);
        if (!sfzFile.is_open()) {
                GEONKICK_LOG_ERROR("can't open sfz file " << getExportPath());
                return false;
        } else {
                sfzFile << sfzStream.str();
        }
        sfzFile.close();
        return true;
}

std::string ExportToSfz::cleanName(const std::string &name)
{
        std::string str = name;
        std::replace(str.begin(), str.end(), ' ', '_');
        return str;
}

std::filesystem::path ExportToSfz::dataPath() const
{
        return getExportPath().parent_path() / getExportPath().stem();
}
