/**
 * File name: envelope.c
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

#include "envelope.h"

struct gkick_envelope*
gkick_envelope_create(void)
{
	struct gkick_envelope *envelope;
	envelope = (struct gkick_envelope*)calloc(1, sizeof(struct gkick_envelope));
	envelope->apply_type = GEONKICK_ENVELOPE_APPLY_LINEAR;
        envelope->curve_type = GEONKICK_ENVELOPE_CURVE_LINEAR;
	return envelope;
}

void gkick_envelope_set_apply_type(struct gkick_envelope* envelope,
				   enum gkick_envelope_apply_type apply_type)
{
        envelope->apply_type = apply_type;
}

enum gkick_envelope_apply_type
gkick_envelope_get_apply_type(const struct gkick_envelope* envelope)
{
        return envelope->apply_type;
}

void gkick_envelope_set_curve_type(struct gkick_envelope* envelope,
				   enum gkick_envelope_curve_type type)
{
        envelope->curve_type = type;
}

enum gkick_envelope_curve_type
gkick_envelope_get_curve_type(struct gkick_envelope* envelope)
{
        return envelope->curve_type;
}

static gkick_real linear_interpolate(gkick_real x0,
                                     gkick_real y0,
                                     gkick_real x1,
                                     gkick_real y1,
                                     gkick_real x)
{
    return y0 + (y1 - y0) * ((x - x0) / (x1 - x0));
}

static gkick_real bezier_x(gkick_real t,
                           gkick_real x0,
                           gkick_real x1,
                           gkick_real x2,
                           gkick_real x3)
{
        const gkick_real one_minus_t = 1.0f - t;
        return pow(one_minus_t, 3.0f) * x0
                + 3.0f * pow(one_minus_t, 2.0f) * t * x1
                + 3.0f * one_minus_t * pow(t, 2.0f) * x2
                + pow(t, 3.0f) * x3;
}

static gkick_real bezier_y(gkick_real t,
                           gkick_real y0,
                           gkick_real y1,
                           gkick_real y2,
                           gkick_real y3)
{
        const gkick_real one_minus_t = 1.0f - t;
        return pow(one_minus_t, 3.0f) * y0
                + 3.0f * pow(one_minus_t, 2.0f) * t * y1
                + 3.0f * one_minus_t * pow(t, 2.0f) * y2
                + pow(t, 3.0f) * y3;
}

gkick_real find_t(const struct gkick_envelope_point *p1,
                  const struct gkick_envelope_point *p2,
                  const struct gkick_envelope_point *p3,
                  gkick_real xm)
{
        gkick_real t0 = 0.0f;
        gkick_real t1 = 1.0f;
        gkick_real t;
        const gkick_real p1_x = p1->x;
        const gkick_real p2_x = p2->x;
        const gkick_real p3_x = p3->x;
        // Binary search to find t corresponding to xm.
        while (t1 - t0 > 1e-6) {
                t = (t0 + t1) * 0.5f;
                gkick_real x_t = bezier_x(t, p1_x, p1_x, p2_x, p3_x);
                if (x_t < xm)
                        t0 = t;
                else
                        t1 = t;
        }
        return t;
}

gkick_real gkick_envelope_get_value(const struct gkick_envelope* envelope, gkick_real xm)
{
    if (envelope == NULL
        || envelope->npoints < 2
        || envelope->first == NULL
        || envelope->last == NULL)
        return 0.0f;

    struct gkick_envelope_point *p = envelope->first;
    if (xm < p->x || xm > envelope->last->x)
            return 0.0f;
    else if (fabsl(xm - p->x) < DBL_EPSILON)
            return p->y;
    else if (fabsl(envelope->last->x - xm) < DBL_EPSILON)
            return envelope->last->y;

    if (envelope->npoints == 2)
        return linear_interpolate(envelope->first->x, envelope->first->y,
                                  envelope->last->x, envelope->last->y, xm);

    struct gkick_envelope_point *p1 = NULL, *p2 = NULL, *p3 = NULL;
    while (p) {
        p1 = p;
        p2 = p->next;
        p3 = p2 ? p2->next : NULL;

        if (p2 == NULL)
                return 0.0f;

        if (p3 == NULL && xm >= p1->x && xm <= p2->x )
                return linear_interpolate(p1->x, p1->y, p2->x, p2->y, xm);

        if (xm >= p1->x && xm <= p3->x)
                break;
        p = p3;
    }

    if (fabsl(p3->x - p1->x) < DBL_EPSILON)
        return p1->y;

    //    if (p2->is_control) {
    //        if (x <= p1->x)
    //                return linear_interpolate(p1->x, p1->y, p2->x, p2->y, xm)
    //}

    gkick_real t = find_t(p1, p2, p3, xm);
    return bezier_y(t, p1->y, p1->y, p2->y, p3->y);
}

struct gkick_envelope_point*
gkick_envelope_add_point(struct gkick_envelope *envelope,
                         float x,
                         float y)
{
	struct gkick_envelope_point *point;
	if (envelope == NULL)
		return NULL;
	point = (struct gkick_envelope_point*)malloc(sizeof(struct gkick_envelope_point));
	if (point == NULL)
		return NULL;
	point->x = x;
	point->y = y;
        point->next = point->prev = NULL;

	if (envelope->first == NULL ||  envelope->last == NULL)
		envelope->first = envelope->last = point;
	else
		gkick_envelope_add_sorted(envelope, point);
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
                /* Add as a first element. */
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
	if (envelope == NULL)
		return;

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

        if (buff == NULL)
                return;

        *buff = NULL;
        if (env->npoints < 1)
                return;

        points = (gkick_real *)calloc(1, sizeof(gkick_real) * (2 * env->npoints));
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
        if (env == NULL || buff == NULL)
                return;

        gkick_envelope_clear(env);
        for (size_t i = 0; i < npoints; i++)
                gkick_envelope_add_point(env, buff[2 * i], buff[2 * i + 1]);
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
        if (env == NULL || index >= env->npoints)
                return;

        struct gkick_envelope_point *p = env->first;
        size_t i = 0;
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
        if (env == NULL || index >= env->npoints)
                return;

        struct gkick_envelope_point *p = env->first;
        size_t i = 0;
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
