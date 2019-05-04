/**
 * File name: globals.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <experimental/filesystem>
#include <fstream>
#include <algorithm>

#include "geonkick.h"

#include "RkLog.h"

#define GEOKICK_APP_NAME "Geonkick"
#define GEOKICK_APP_VERION 0x020000
#define GEOKICK_APP_VERION_STRING "2.0"

#define GEONKICK_LOG_INFO(msg) std::cout << __PRETTY_FUNCTION__ << "[INFO] " << msg << std::endl;
#define GEONKICK_LOG_ERROR(msg) std::cout << __PRETTY_FUNCTION__ << "[ERROR] " << msg << std::endl;
#define GEONKICK_LOG_DEBUG(msg) std::cout << __PRETTY_FUNCTION__ << "[DEBUG] " << msg << std::endl;

#endif // GEONKICK_GLOBALS_H
