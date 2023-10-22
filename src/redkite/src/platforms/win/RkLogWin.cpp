/**
 * File name: RkLogWin.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#include "RkLog.h"
#include "RkPlatform.h"

void rkLogMessage(RkLogLevel level, const std::string &func_name, const std::string &msg)
{
        std::string strLevel;

        switch (level)
        {
        case RkLogLevel::Debug:
                strLevel = "Debug";
        break;
        case RkLogLevel::Info:
                strLevel = "Info";
        break;
        case RkLogLevel::Warning:
                strLevel = "Warning";
        break;
        case RkLogLevel::Error:
                strLevel = "Error";
        break;
        default:
                strLevel = "Unknown";
        }

        std::cout << "[" << std::this_thread::get_id() << "][" << strLevel << "] " << func_name << ": " << msg << std::endl;

        //OutputDebugString(("[" + strLevel + "] " + func_name + ": " + msg).c_str());
}

