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
//#include "RkLog.h"

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

//#include <RkObject.h>

#ifdef RK_LOG_DEBUG_LEVEL
#define GEONKICK_LOG_INFO(msg) std::cout << "[" << GEONKICK_NAME << "] " \
                               << " [" << std::this_thread::get_id() << "] " \
                               << __PRETTY_FUNCTION__ << "[INFO] " \
                               << msg << std::endl;
#define GEONKICK_LOG_ERROR(msg) std::cout << "[" << GEONKICK_NAME << "] " \
                                << " [" << std::this_thread::get_id() << "] " \
                                << __PRETTY_FUNCTION__ << "[ERROR] " \
                                << msg << std::endl;
#define GEONKICK_LOG_DEBUG(msg) std::cout << "[" << GEONKICK_NAME << "] " \
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


// =========== TEMPORARY ==========

using rk_real = double;

class RkPoint {
 public:

       constexpr RkPoint() : xValue{0}, yValue{0}
       {
       }

       constexpr RkPoint(int x, int y) : xValue{x}, yValue{y}
       {
       }

       friend constexpr bool operator==(const RkPoint &p1, const RkPoint &p2)
       {
               return p1.xValue == p2.xValue && p1.yValue == p2.yValue;
       }

       friend constexpr bool operator!=(const RkPoint &p1, const RkPoint &p2)
       {
               return p1.xValue != p2.xValue || p1.yValue != p2.yValue;
       }

       constexpr int x() const
       {
               return xValue;
       }

       constexpr void setX(int x)
       {
               xValue = x;
       }

       constexpr int y() const
       {
               return yValue;
       }

       constexpr void setY(int y)
       {
               yValue = y;
       }

 private:
        int xValue;
        int yValue;
};


class RkRealPoint {
 public:

       constexpr RkRealPoint() : xValue{0}, yValue{0}
       {
       }

       constexpr RkRealPoint(const RkPoint &p) : xValue(p.x()), yValue(p.y())
       {
       }

       constexpr RkRealPoint(rk_real x, rk_real y) : xValue{x}, yValue{y}
       {
       }

       constexpr rk_real x() const
       {
               return xValue;
       }

       constexpr void setX(rk_real x)
       {
               xValue = x;
       }

       constexpr rk_real y() const
       {
               return yValue;
       }

       constexpr void setY(rk_real y)
       {
               yValue = y;
       }

 private:
        rk_real xValue;
        rk_real yValue;
};
//============= TEMPORARY ==========



namespace Geonkick
{
        constexpr int defaultSampleRate = GEONKICK_DEFAULT_SAMPLE_RATE;
        constexpr char appName[] = GEONKICK_NAME;
        constexpr char applicationName[] = GEONKICK_APP_NAME;
        constexpr char applicationVersionStr[] = GEOKICK_VERSION_STRING;
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
            //double n = std::pow(10, precision);
            return 0;// std::trunc(val * n) / n;
        }

        inline std::string doubleToStr(double val, unsigned int precision)
        {
               // std::stringstream ss;
               // ss << std::fixed
               //    << std::setprecision(precision)
               //    << truncateDouble(val, precision);
            return "";// ss.str();
        }

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
