/**
 * File name: envelope.c
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

#include "envelope.h"

struct gkick_envelope*
gkick_envelope_create(void)
{
	struct gkick_envelope *envelope;

	envelope = (struct gkick_envelope*)malloc(sizeof(struct gkick_envelope));
	if (envelope == NULL) {
		return NULL;
	}
	memset(envelope, 0, sizeof(struct gkick_envelope));
	return envelope;
}

gkick_real
gkick_envelope_get_value(const struct gkick_envelope* envelope, gkick_real xm)
{
	gkick_real x1, y1, x2, y2, ym;
	struct gkick_envelope_point *p;

	if (envelope == NULL ||
	    (envelope->first == NULL
	     || envelope->last == NULL)) {
		return 0.0;
	}

	if (xm < envelope->first->x || xm > envelope->last->x) {
		return 0.0;
	} else if (fabsl(xm - envelope->first->x) < DBL_EPSILON) {
                return envelope->first->y;
        } else if (fabsl(envelope->last->x - xm) < DBL_EPSILON) {
                return envelope->last->y;
        }

        x2 = x1 = xm;
	p = envelope->first;
	while (p) {
                if (fabs(xm - p->x) < DBL_EPSILON) {
                        return p->y;
                } else if (xm < p->x) {
			x2 = p->x;
			y2 = p->y;
			break;
		}
		p = p->next;
	}

	p = envelope->last;
	while (p) {
                if (fabs(xm - p->x) < DBL_EPSILON) {
                        return p->y;
                } else if (p->x < xm) {
			x1 = p->x;
			y1 = p->y;
			break;
		}
		p = p->prev;
	}

	if (fabsl(x2 - x1) < DBL_EPSILON) {
		return y1;
	} else {
		ym = (y1 * (x2 - xm) + y2 * (xm - x1)) / (x2 - x1);
	}

	return ym;
}

struct gkick_envelope_point*
gkick_envelope_add_point(struct gkick_envelope *envelope,
                         float x,
                         float y)
{
	struct gkick_envelope_point *point;

	if (envelope == NULL) {
		return NULL;
	}

	point = (struct gkick_envelope_point*)malloc(sizeof(struct gkick_envelope_point));
	if (point == NULL) {
		return NULL;
	}
	point->x = x;
	point->y = y;
        point->next = point->prev = NULL;

	if (envelope->first == NULL ||  envelope->last == NULL) {
		envelope->first = envelope->last = point;
	} else {
		gkick_envelope_add_sorted(envelope, point);
	}
	envelope->npoints++;

	return point;
}

void gkick_envelope_add_sorted(struct gkick_envelope *envelope,
			       struct gkick_envelope_point *point)
{
	struct gkick_envelope_point *p;

        if (point->x >= envelope->last->x) {
                /* Add as a last element. */
		envelope->last->next = point;
	        point->prev = envelope->last;
		envelope->last = point;
	} else if (point->x <= envelope->first->x) {
                /* Add as a frist element. */
		envelope->first->prev = point;
	        point->next = envelope->first;
		envelope->first = point;
        } else {
                p = envelope->first;
		while (p) {
                        if (p->next != NULL && point->x < p->next->x) {
                                p->next->prev = point;
                                point->next = p->next;
                                point->prev = p;
                                p->next = point;
                                break;
                        }
                        p = p->next;
		}
	}
}

void gkick_envelope_destroy(struct gkick_envelope *envelope)
{
	struct gkick_envelope_point *point;
	if (envelope == NULL) {
		return;
	}

	if (envelope->npoints != 0) {
		while (envelope->first != NULL) {
			point = envelope->first;
			envelope->first = point->next;
			free(point);
		}
	}
	free(envelope);
}

void
gkick_envelope_get_points(struct gkick_envelope *env,
			  gkick_real **buff,
			  size_t *npoints)
{
        struct gkick_envelope_point *p;
        gkick_real *points;
        size_t i;

        if (buff == NULL) {
                return;
        }

        *buff = NULL;
        if (env->npoints < 1) {
                return;
        }

        points = (gkick_real *)malloc(sizeof(gkick_real) * (2 * env->npoints));
        memset(points, 0, sizeof(gkick_real) * (2 * env->npoints));
        p = env->first;
        i = 0;
        while (p) {
                points[i]     = p->x;
                points[i + 1] = p->y;
                p = p->next;
                i += 2;
        }

        *buff = points;
        *npoints = env->npoints;
}

void
gkick_envelope_set_points(struct gkick_envelope *env,
			  const gkick_real *buff,
			  size_t npoints)
{
        size_t i;

        if (env == NULL || buff == NULL) {
                return;
        }

        gkick_envelope_clear(env);
        for (i = 0; i < npoints; i++) {
                gkick_envelope_add_point(env, buff[2 * i], buff[2 * i + 1]);
        }
}

void gkick_envelope_clear(struct gkick_envelope* env)
{
        struct gkick_envelope_point *p = env->first;
        struct gkick_envelope_point *curr;
        while (p != NULL) {
                curr = p;
                p = p->next;
                free(curr);
                env->npoints--;
        }
        env->first = env->last = NULL;
}

void
gkick_envelope_remove_point(struct gkick_envelope *env, size_t index)
{
        struct gkick_envelope_point *p;
        size_t i;

        if (env == NULL) {
                return;
        }

        if (!(index >= 0 && index < env->npoints)) {
                return;
        }

        p = env->first;
        i = 0;
        while (p) {
                if (i == index) {
                        if (p == env->first) {
                                env->first = p->next;
                        } else if (p == env->last) {
                                env->last = p->prev;
                        } else {
                                p->prev->next = p->next;
                                p->next->prev = p->prev;
                        }
                        free(p);
                        env->npoints--;
                        break;
                }
                p = p->next;
                i++;
        }
}

void
gkick_envelope_update_point(struct gkick_envelope *env,
			    size_t index,
			    gkick_real x,
			    gkick_real y)
{
        struct gkick_envelope_point *p;
        size_t i;

        if (env == NULL) {
                return;
        }

        if (!(index >= 0 && index < env->npoints)) {
                return;
        }

        p = env->first;
        i = 0;
        while (p) {
                if (i == index) {
                        p->x = x;
                        p->y = y;
                        break;
                }
                p = p->next;
                i++;
        }
}
