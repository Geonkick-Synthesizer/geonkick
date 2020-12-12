/**
 * File name: globals.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include <RkObject.h>

#ifdef RK_LOG_DEBUG_LEVEL
#define GEONKICK_LOG_INFO(msg) std::cout << __PRETTY_FUNCTION__ << "[INFO] " << msg << std::endl;
#define GEONKICK_LOG_ERROR(msg) std::cout << __PRETTY_FUNCTION__ << "[ERROR] " << msg << std::endl;
#define GEONKICK_LOG_DEBUG(msg) std::cout << __PRETTY_FUNCTION__ << "[DEBUG] " << msg << std::endl;
#else
#define GEONKICK_LOG_INFO(msg) std::cout << "[INFO] " << msg << std::endl;
#define GEONKICK_LOG_ERROR(msg) std::cout << "[ERROR] " << msg << std::endl;
#define GEONKICK_LOG_DEBUG(msg)
#endif //  // GKICK_LIB_LOG_LEVEL_DEBUG

namespace Geonkick
{
        constexpr int defaultSampleRate = 48000;
        constexpr char appName[] = GEONKICK_NAME;
        constexpr char applicationName[] = GEONKICK_APP_NAME;
        constexpr char applicationVersionStr[] = GEOKICK_VERSION_STRING; 
} // namespace Geonkick

namespace GeonkickTypes
{
using MidiKey = unsigned char;
constexpr MidiKey geonkickAnyKey = GEONKICK_ANY_KEY;

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
