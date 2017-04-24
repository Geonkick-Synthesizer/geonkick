#ifndef GKICk_LOG_H
#define GKICK_LOG_H

#include "geonkick_internal.h"

#include <stdarg.h>
#include<stdio.h>

void
gkick_log_msg(const char *message, ...);

#ifdef GKICK_LIB_LOG_LEVEL_TRACE
#define GKICK_LIB_LOG_LEVEL_DEBUG
#ifdef GKICK_LIB_LOG_FUNCTION
#define gkick_log_trace(message, ...) \
  gkick_log_msg("[TRACE][%s] "message, __func__, ##__VA_ARGS__)
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
  gkick_log_msg("[DEBUG][%s] "message, __func__, ##__VA_ARGS__)
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
  gkick_log_msg("[INFO][%s] "message, __func__, ##__VA_ARGS__)
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
  gkick_log_msg("[WARNING][%s] "message, __func__, ##__VA_ARGS__)
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
  gkick_log_msg("[ERROR][%s] "message, __func__, ##__VA_ARGS__)
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
