/**
 * File name: geonkick_synthesizer.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#ifndef GEONKICK_SYNTHESIZER_H
#define GEONKICK_SYNTHESIZER_H

struct geonkick_synthesizer {
        double current_time;
        struct gkick_oscillator **oscillators;
        size_t oscillators_number;

        /* General synthesizer amplitude envelope. */
        struct gkick_envelope *envelope;

        /* Time length of the kick (float value from 0 to 1.0). */
        double length;
        pthread_mutex_t lock;
};

#endif // GEONKICK_SYNTHESIZER_H
