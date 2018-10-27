/**
 * File name: gluidgeon.h
 * Project: FluidGeon
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of FluidGeon.
 *
 * FluidGeon is free software; you can redistribute it and/or modify
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

/**
 * FluidGeon is a LV2 plugin that plays SoundFont files using FluidSynth.
 */

#ifndef FLUID_GEON_H
#define FLUID_GEON_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>


#define AMP_URI "http://geontime.com/plugins/fluidgeon"

#define FLUIDGEON_VERSION       0x000100
#define FLUIDGEON_VERSION_MAJOR 0x00
#define FLUIDGEON_VERSION_MINOR 0x01
#define FLUIDGEON_VERSION_PATCH 0x00

#define UNUSED(x) (void)(x)

void
fgeon_log_msg(const char *message, ...)
{
  va_list args;

  va_start (args, message);
  vprintf (message, args);
  va_end (args);
  printf("\n");
}

#ifdef FGEON_LOG_LEVEL_TRACE
#define FGEON_LOG_LEVEL_DEBUG
#ifdef FGEON_LOG_FUNCTION
#define fgeon_log_trace(message, ...) \
  fgeon_log_msg("[TRACE][%s] "message, __func__, ##__VA_ARGS__)
#else
#define fgeon_log_trace(message, ...) fgeon_log_msg("[TRACE] " message, ##__VA_ARGS__)
#endif
#else
#define fgeon_log_trace(message, ...)
#endif /* FGEON_LOG_LEVEL_TRACE */

#ifdef FGEON_LOG_LEVEL_DEBUG
#define FGEON_LOG_LEVEL_INFO
#ifdef FGEON_LOG_FUNCTION
#define fgeon_log_debug(message, ...) \
  fgeon_log_msg("[DEBUG][%s] "message, __func__, ##__VA_ARGS__)
#else
#define fgeon_log_debug(message, ...) fgeon_log_msg("[DEBUG] " message, ##__VA_ARGS__)
    #endif
#else
#define fgeon_log_debug(message, ...)
#endif /* FGEON_LOG_LEVEL_DEBUG */

#ifdef FGEON_LOG_LEVEL_INFO
  #define FGEON_LOG_LEVEL_WARNING
#ifdef FGEON_LOG_FUNCTION
#define fgeon_log_info(message, ...) \
  fgeon_log_msg("[INFO][%s] "message, __func__, ##__VA_ARGS__)
#else
#define fgeon_log_info(message, ...) fgeon_log_msg("[INFO] " message, ##__VA_ARGS__)
  #endif
#else
#define fgeon_log_info(message, ...)
#endif /* FGEON_LOG_LEVEL_INFO */

#ifdef FGEON_LOG_LEVEL_WARNING
  #define FGEON_LOG_LEVEL_ERROR
#ifdef FGEON_LOG_FUNCTION
#define fgeon_log_warning(message, ...) \
  fgeon_log_msg("[WARNING][%s] "message, __func__, ##__VA_ARGS__)
#else
#define fgeon_log_warning(message, ...) fgeon_log_msg("[WARNING] " message, ##__VA_ARGS__)
#endif
#else
#define fgeon_log_warning(message, ...)
#endif /* FGEON_LOG_LEVEL_WARNING */

#ifdef FGEON_LOG_LEVEL_ERROR
#define FGEON_LOG_LEVEL_CRITICAL
#ifdef FGEON_LOG_FUNCTION
#define fgeon_log_error(message, ...) \
  fgeon_log_msg("[ERROR][%s] "message, __func__, ##__VA_ARGS__)
#else
#define fgeon_log_error(message, ...) fgeon_log_msg("[ERROR] " message, ##__VA_ARGS__)
#endif
#else
#define fgeon_log_error(message, ...)
#endif /* FGEON_LOG_LEVEL_ERROR */

#ifdef FGEON_LOG_LEVEL_CRITICAL
#ifdef FGEON_LOG_FUNCTION
#define fgeon_log_critical(message, ...) \
  fgeon_log_msg("[CRITICAL] " message, __func__, ##__VA_ARGS__)
#else
#define fgeon_log_critical(message, ...) \
  fgeon_log_msg("[CRITICAL] " message, ##__VA_ARGS__)
#endif
#else
#define fgeon_log_critical(message, ...)

#endif

#endif
