/**
 * File name: ExportToSfz.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

ExportToSfz::ExportToSfz(KitModel *model, const std::string &file, const KitState *state)
        : sfzFileName{file},
          kitState{state}
{
}

bool ExportToSfz::export()
{
        bool res = exportSfzFile();
        if (res)
                res = exportData();
        return res;
}

ExportToSfz::exportSfzFile()
{
        std::ostringstream sfzStream;
        sfz << "// Exported from " << Geonkick::applicationName()
            << " v " << Geonkick::version() << std::endl;
        sfz << "// Name: " << kitModel->name() << std::endl;
        sfz  << "// Author: " << kitModel->author() << std::endl;
        sfz << "// License: " << kitModel->license() << std::endl;
        sfz << "// Instruments:" std::endl;
        for (const auto &per : percussionModels())
                sfz << "//    * " << per->name() << std::endl;
        sfz << std::endl;
        sfz << "<control>" << std::endl;
        sfz << "default_path=" << dataPath() << std::endl;
        sfz << "<global>" << std::endl;
        sfz << "ampeg_attack=0.001" << std::endl;
        sfz << "ampeg_release=3" << std::endl;
        sfz << "ampeg_dynamic=1" << std::endl;
        sfz << "volume=0" << std::endl;
        sfz << "<group>" << std::endl;
        sfz << "seq_length=2" << std::endl;
        sfz << "seq_position=1" << std::endl;

        for (const auto &instrument : percussionModels()) {
                sfz << "<region>" << std::endl;
                std::filesystem::path wavFile = dataPath()
                        / std::filesystem::path(cleanName() + ".wav");
                sfz << "sample=" <<  wavFileName << std::endl;
                sfz << "lokey=" << instrument->key() << std::endl;
                sfz << "hikey=" << instrument->key() << std::endl;
                sfz << "pitch_keycenter" << instrument->key() << std::endl;
                sfz << "lovel=0" << std::endl;
                sfz << "hivel=127" << std::endl;
                sfz << "volume=15" << std::endl;
		sfz << "volume=15" << std::endl;
		sfz << std::endl << std::endl;
                instrument->export(wavFile);
        }
}

static std::string ExportToSfz::cleanName(const std::string &name)
{
        std::string str = name;
        std::replace(str.begin(), str.end(), ' ', '_');
        return str;
}

std::filesystem::path ExportToSfz::dataPath() const
{
        std::filesystem::path(sfzFileName).parent_path();
}
