/**
 * File name: gkick_log.h
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

#ifndef GKICK_LOG_H
#define GKICK_LOG_H

#include <stdarg.h>
#include <stdio.h>

void
gkick_log_msg(const char *message, ...);

#ifdef GKICK_LIB_LOG_LEVEL_TRACE
#define GKICK_LIB_LOG_LEVEL_DEBUG
#ifdef GKICK_LIB_LOG_FUNCTION
#define gkick_log_trace(message, ...) \
  gkick_log_msg("[TRACE][%s] " message, __func__, ##__VA_ARGS__)
#else
#define gkick_log_trace(message, ...) gkick_log_msg("[TRACE] " message, ##__VA_ARGS__)
#endif
#else
#define gkick_log_trace(message, ...)
#endif /* GKICK_LIB_LOG_LEVEL_TRACE */

#ifdef GKICK_LIB_LOG_LEVEL_DEBUG
#define GKICK_LIB_LOG_LEVEL_INFO
#ifdef GKICK_LIB_LOG_FUNCTION
#define gkick_log_debug(message, ...) \
  gkick_log_msg("[DEBUG][%s] " message, __func__, ##__VA_ARGS__)
#else
#define gkick_log_debug(message, ...) gkick_log_msg("[DEBUG] " message, ##__VA_ARGS__)
  #endif
#else
#define gkick_log_debug(message, ...)
#endif /* GKICK_LIB_LOG_LEVEL_DEBUG */

#ifdef GKICK_LIB_LOG_LEVEL_INFO
#define GKICK_LIB_LOG_LEVEL_WARNING
#ifdef GKICK_LIB_LOG_FUNCTION
#define gkick_log_info(message, ...) \
  gkick_log_msg("[INFO][%s] " message, __func__, ##__VA_ARGS__)
#else
#define gkick_log_info(message, ...) gkick_log_msg("[INFO] " message, ##__VA_ARGS__)
  #endif
#else
#define gkick_log_info(message, ...)
#endif /* GKICK_LIB_LOG_LEVEL_INFO */

#ifdef GKICK_LIB_LOG_LEVEL_WARNING
  #define GKICK_LIB_LOG_LEVEL_ERROR
#ifdef GKICK_LIB_LOG_FUNCTION
#define gkick_log_warning(message, ...) \
  gkick_log_msg("[WARNING][%s] " message, __func__, ##__VA_ARGS__)
#else
#define gkick_log_warning(message, ...) gkick_log_msg("[WARNING] " message, ##__VA_ARGS__)
#endif
#else
#define gkick_log_warning(message, ...)
#endif /* GKICK_LIB_LOG_LEVEL_WARNING */

#ifdef GKICK_LIB_LOG_LEVEL_ERROR
#define GKICK_LIB_LOG_LEVEL_CRITICAL
#ifdef GKICK_LIB_LOG_FUNCTION
#define gkick_log_error(message, ...) \
  gkick_log_msg("[ERROR][%s] " message, __func__, ##__VA_ARGS__)
#else
#define gkick_log_error(message, ...) gkick_log_msg("[ERROR] " message, ##__VA_ARGS__)
#endif
#else
#define gkick_log_error(message, ...)
#endif /* GKICK_LIB_LOG_LEVEL_ERROR */

#ifdef GKICK_LIB_LOG_LEVEL_CRITICAL
#ifdef GKICK_LIB_LOG_FUNCTION
#define gkick_log_critical(message, ...) \
  gkick_log_msg("[CRITICAL] " message, __func__, ##__VA_ARGS__)
#else
#define gkick_log_critical(message, ...) \
  gkick_log_msg("[CRITICAL] " message, ##__VA_ARGS__)
#endif
#else
#define gkick_log_critical(message, ...)
#endif /* GKICK_LIB_LOG_LEVEL_CRITICAL */

#endif
