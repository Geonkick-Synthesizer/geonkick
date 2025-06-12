/**
 * File name: globals.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
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

#ifndef GEONKICK_GLOBALS_H
#define GEONKICK_GLOBALS_H

#include "geonkick.h"
#include "RkLog.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <array>
#include <mutex>
#include <thread>
#include <iomanip>
#include <math.h>
#include <optional>
#include <ranges>

namespace fs = std::filesystem;

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <RkObject.h>

#ifdef GKICK_LOG_DEBUG_LEVEL
#define GEONKICK_LOG_INFO(msg) std::cout << "[" << GEONKICK_NAME << "] " \
                               << " [" << std::this_thread::get_id() << "] " \
                               << __PRETTY_FUNCTION__ << "[INFO] " \
                               << msg << std::endl;
#define GEONKICK_LOG_ERROR(msg) std::cout << "[" << GEONKICK_NAME << "] " \
                                << " [" << std::this_thread::get_id() << "] " \
                                << __PRETTY_FUNCTION__ << "[ERROR] " \
                                << msg << std::endl;
#define GEONKICK_LOG_DEBUG(msg) std::cout << "[" << std::this_thread::get_id() << "][" << GEONKICK_NAME << "] " \
                                << " [" << std::this_thread::get_id() << "] " \
                                << __PRETTY_FUNCTION__ << "[DEBUG] " \
                                << msg << std::endl;
#else
#define GEONKICK_LOG_INFO(msg) std::cout << "[" << GEONKICK_NAME << "] " \
                               << "[INFO] " << msg << std::endl;
#define GEONKICK_LOG_ERROR(msg) std::cout << "[" << GEONKICK_NAME << "] " \
                               << "[ERROR] " << msg << std::endl;
#define GEONKICK_LOG_DEBUG(msg)
#endif //  // GKICK_LIB_LOG_LEVEL_DEBUG

namespace Geonkick
{
        constexpr int defaultSampleRate = GEONKICK_DEFAULT_SAMPLE_RATE;
        constexpr char appName[] = GEONKICK_NAME;
        constexpr char applicationName[] = GEONKICK_APP_NAME;
        constexpr char applicationVersionStr[] = GEOKICK_VERSION_STRING;
        constexpr std::string_view defaultExportFormat = "flac24";
        constexpr int defaultBitDepth = 16;
        constexpr double toDecibel(double val)
        {
                if (val < std::numeric_limits<double>::min())
                        return -80;
                return 20 * log10(val);
        }

        constexpr double fromDecibel(double decibel)
        {
                if (decibel < -80)
                        return 0;
                return pow(10,  decibel / 20);
        }

        constexpr double truncateDouble(double val, unsigned int precision)
        {
                double n = std::pow(10, precision);
                return std::trunc(val * n) / n;
        }

        inline std::string doubleToStr(double val, unsigned int precision)
        {
                std::stringstream ss;
                ss << std::fixed
                   << std::setprecision(precision)
                   << truncateDouble(val, precision);
                return  ss.str();
        }

        inline std::string toLower(const std::string& input)
        {
                std::string result = input;
                std::transform(result.begin(), result.end(), result.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                return result;
        }
} // namespace Geonkick

namespace GeonkickTypes
{
using MidiKey = unsigned char;
constexpr MidiKey geonkickAnyKey = GEONKICK_ANY_KEY;
constexpr int geonkickAnyMidiChannel = GEONKICK_ANY_MIDI_CHANNEL;

enum class Formats : int {
          Gkick  = 0,
          Gkit   = 1,
          Flac   = 2,
          Wav    = 4,
          Ogg    = 7,
          Sfz    = 8
};

} // namespace GeonkickTypes

#endif // GEONKICK_GLOBALS_H
