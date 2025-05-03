/**
 * File name: ExportSoundData.cpp
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

#include "ExportSoundData.h"

#include <sndfile.h>

ExportSoundData::ExportSoundData(const std::filesystem::path &file,
                                 const std::vector<float> &data,
                                 ExportFormat exportFormat)
        : ExportAbstract(file, exportFormat)
        , soundData{data}
        , bitDepth{Geonkick::defaultBitDepth}
        , sampleRate{Geonkick::defaultSampleRate}
        , nChannels{2}
{
}

bool ExportSoundData::doExport()
{
        SF_INFO sndinfo;
        memset(&sndinfo, 0, sizeof(sndinfo));
        sndinfo.samplerate = getSampleRate();
        sndinfo.channels   = numberOfChannels();
        sndinfo.format     = sfExportFormat();

        auto tempBuffer = soundData;
        sndinfo.frames = tempBuffer.size();
        std::vector<gkick_real> dataBuffer;
        if (sndinfo.channels == 2) {
                dataBuffer.resize(2 * tempBuffer.size());
                size_t k = 0;
                while (k < tempBuffer.size()) {
                        dataBuffer[2 * k] = dataBuffer[2 * k + 1] = tempBuffer[k];
                        k++;
                }
        } else {
                dataBuffer = std::move(tempBuffer);
        }

        if (dataBuffer.empty() || !sf_format_check(&sndinfo)) {
                setError("error on exporting data");
                return false;
        }

        auto filePath = getExportPath();
        if (filePath.empty()) {
                setError("wrong file name is empty");
                return false;
        }

        SNDFILE *sndFile = sf_open(filePath.string().c_str(), SFM_WRITE, &sndinfo);
        if (!sndFile) {
                setError("can't open file " + filePath.string());
                return false;
        }

        size_t n;
        n = sf_write_float(sndFile, dataBuffer.data(), dataBuffer.size());
        if (n != dataBuffer.size()) {
                setError("error on exporting");
                sf_close(sndFile);
                return false;
        }
        sf_close(sndFile);
        return true;
}

int ExportSoundData::getBitDepth() const
{
        return bitDepth;
}

void ExportSoundData::setBitDepth(int depth)
{
        if (!validateBitDepth(depth)) {
                GEONKICK_LOG_ERROR("wrong bit depth " << bitDepth
                                   << " for format "
                                   << static_cast<int>(format()));
        } else {
                bitDepth = depth;
        }
}

int ExportSoundData::getSampleRate() const
{
        return sampleRate;
}

void ExportSoundData::setSampleRate(int srate)
{
        sampleRate = srate;
}

int ExportSoundData::numberOfChannels() const
{
        return nChannels;
}

void ExportSoundData::setNumberOfChannels(int channels)
{
        nChannels = channels;
}

bool ExportSoundData::validateBitDepth(int depth) const
{
        switch (format()) {
        case ExportFormat::Flac:
                return depth == 16 || depth == 24;
        case ExportFormat::Wav:
                return depth == 16 || depth == 24 || depth == 32;
        case ExportFormat::Ogg:
                return true;
        default:
                return false;
        }
}

int ExportSoundData::sfExportFormat() const
{
        switch (format()) {
        case ExportFormat::Flac:
                if (bitDepth == 16)
                        return SF_FORMAT_FLAC | SF_FORMAT_PCM_16;
                else
                        return SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
        case ExportFormat::Wav:
                if (bitDepth == 16)
                        return SF_FORMAT_WAV | SF_FORMAT_PCM_16;
                else if (bitDepth == 32)
                        return SF_FORMAT_WAV | SF_FORMAT_PCM_32;
                else
                        return SF_FORMAT_WAV | SF_FORMAT_PCM_24;
        case ExportFormat::Ogg:
                return SF_FORMAT_OGG | SF_FORMAT_VORBIS;
        default:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_24;
        }
}
