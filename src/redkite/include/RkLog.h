/**
 * File name: RkLog.h
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

#ifndef RK_LOGGING_H
#define RK_LOGGING_H

#include "Rk.h"

enum class RkLogLevel : int {
        Debug,
        Info,
        Warning,
        Error
};

void rkLogMessage(RkLogLevel level, const std::string &func_name, const std::string &msg);

#if defined(__GNUC__) || defined(__MINGW32__)
#define RK_LOG_FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define RK_LOG_FUNC_NAME __FUNCTION__
#else
	#define RK_LOG_FUNC_NAME __PRETTY_FUNCTION__
#endif

#define RK_LOG_DEV_DEBUG(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Debug, RK_LOG_FUNC_NAME, rk_stream.str()); } while(0)

#ifdef RK_LOG_DEBUG_LEVEL
#define RK_LOG_DEBUG(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Debug, RK_LOG_FUNC_NAME, rk_stream.str()); } while(0)
#define RK_LOG_INFO(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Info, RK_LOG_FUNC_NAME, rk_stream.str()); } while(0)
#define RK_LOG_WARN(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Warning, RK_LOG_FUNC_NAME,  rk_stream.str()); } while(0)
#define RK_LOG_ERROR(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Error, RK_LOG_FUNC_NAME, rk_stream.str()); } while(0)
#else
#define RK_LOG_DEBUG(msg)
#define RK_LOG_INFO(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Info, RK_LOG_FUNC_NAME, rk_stream.str()); } while(0)
#define RK_LOG_WARN(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Warning, RK_LOG_FUNC_NAME,  rk_stream.str()); } while(0)
#define RK_LOG_ERROR(msg) do { \
        std::ostringstream rk_stream; \
        rk_stream << msg; \
        rkLogMessage(RkLogLevel::Error, RK_LOG_FUNC_NAME, rk_stream.str()); } while(0)
#endif // RK_LOG_ERROR_LEVEL

#endif // RK_LOGGING_H
