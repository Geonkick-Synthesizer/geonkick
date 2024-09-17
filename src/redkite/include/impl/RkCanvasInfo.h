/**
 * File name: RkCanvasInfo.h
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

#ifndef RK_CANVAS_INFO_H
#define RK_CANVAS_INFO_H

#include "Rk.h"

#ifdef RK_GRAPHICS_CAIRO_BACKEND

#include <cairo/cairo.h>
#if RK_OS_WIN
#include <cairo/cairo-win32.h>
#else // GNU/Linux
#include <cairo/cairo-xlib.h>
#endif 

struct RkCanvasInfo {
        cairo_surface_t* cairo_surface = nullptr;
        cairo_t* cairo_context = nullptr;
};
#else
#error No graphics backend defined
#endif

#endif // RK_CANVAS_INFO_H
