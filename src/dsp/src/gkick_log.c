/**
 * File name: gkick_log.c
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

#include "gkick_log.h"

void
gkick_log_msg(const char *message, ...)
{
        va_list args;
        va_start (args, message);
        vprintf (message, args);
        va_end (args);
        printf("\n");
}

void
gkick_msg_info(const char *msg, ...)
{
        va_list args;
        printf("Info: ");
        va_start (args, msg);
        vprintf (msg, args);
        va_end (args);
        printf("\n");
}


void
gkick_msg_error(const char *msg, ...)
{
        va_list args;
        printf("Error: ");
        va_start (args, msg);
        vprintf (msg, args);
        va_end (args);
        printf("\n");
}
