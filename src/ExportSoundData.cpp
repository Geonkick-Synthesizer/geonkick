/**
 * File name: ExportSoundData.cpp
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

#include "ExportSoundData.h"

#include <sndfile.h>

ExportSoundData::ExportSoundData(RkObject *parent,
                                 const std::filesystem::path &file,
                                 const std::vector<float> &data,
                                 ExportFormat exportFormat)
        : ExportAbstract(parent, file, exportFormat)
        , soundData{data}
        , exportSubformat{getDefaultSubformat()}
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

ExportSoundData::Subformat ExportSoundData::subformat() const
{
        return exportSubformat;
}

void ExportSoundData::setSubformat(ExportSoundData::Subformat subFormat)
{
        if (!validateSubfromat(subFormat)) {
                GEONKICK_LOG_ERROR("wrong subformat " << static_cast<int>(subFormat)
                                   << " for format "
                                   << static_cast<int>(format()));
        } else {
                exportSubformat = subFormat;
        }
}

int ExportSoundData::getSampleRate() const
{
        return sampleRate;
}

void ExportSoundData::setSmapleRate(int srate)
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

bool ExportSoundData::validateSubfromat(Subformat subFormat)
{
        switch (format()) {
        case ExportFormat::Flac:
                return subFormat == Subformat::Flac16 || subFormat == Subformat::Flac24;
        case ExportFormat::Wav:
                return subFormat == Subformat::Wav16
                        || subFormat == Subformat::Wav24
                        || subFormat == Subformat::Wav32;
        case ExportFormat::Ogg:
                return subFormat == Subformat::Ogg;
        default:
                return false;
        }
}

ExportSoundData::Subformat
ExportSoundData::getDefaultSubformat() const
{
        switch (format()) {
        case ExportFormat::Wav:
                return Subformat::Wav16;
        case ExportFormat::Ogg:
                return Subformat::Ogg;
        case ExportFormat::Flac:
        default:
                return Subformat::Flac16;
        }
}

int ExportSoundData::sfExportFormat() const
{
        switch (subformat())
        {
        case Subformat::Flac16:
                return SF_FORMAT_FLAC | SF_FORMAT_PCM_16;
        case Subformat::Flac24:
                return SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
        case Subformat::Wav16:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        case Subformat::Wav24:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_24;
        case Subformat::Wav32:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_32;
        case Subformat::Ogg:
                return SF_FORMAT_OGG | SF_FORMAT_VORBIS;
        default:
                return SF_FORMAT_WAV | SF_FORMAT_PCM_24;
        }
}


