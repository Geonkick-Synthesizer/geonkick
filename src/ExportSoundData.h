/**
 * File name: ExportSoundData.h
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

#ifndef GEONGKICK_SOUND_DATA_H
#define GEONGKICK_SOUND_DATA_H

#include "ExportAbstract.h"

class ExportSoundData : public ExportAbstract {
 public:
        ExportSoundData(const std::filesystem::path &file,
                        const std::vector<float> &data,
                        ExportFormat exportFormat = ExportFormat::Flac);
        bool doExport() override;
        void setBitDepth(int depth);
        int getBitDepth() const;
        int getSampleRate() const;
        void setSampleRate(int srate);
        int numberOfChannels() const;
        void setNumberOfChannels(int channels);

 protected:
        bool validateBitDepth(int depth) const;
        int sfExportFormat() const;

 private:
        std::vector<float> soundData;
        int bitDepth;
        int sampleRate;
        int nChannels;
};

#endif // GEONGKICK_SOUND_DATA_H
