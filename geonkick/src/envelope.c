/*
  Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "envelope.h"

/*
 * TODO: reduce the averall
 * complexity of the envelope.
 *
*/

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

/**
 * Compexity = O(N), where N are the nuber of evelope points.
 * TODO: Reduce the complexity. With an array instead of list
 * it can be reducedl to O(log(N)) with binary search.
 */
double
gkick_envelope_get_value(const struct gkick_envelope* envelope, double xm)
{
	double x1, y1, x2, y2, ym;
	struct gkick_envelope_point *p;

	if (envelope == NULL ||
	    (envelope->first == NULL
	     || envelope->last == NULL)) {
		return 0.0;
	}

	if (xm < envelope->first->x || xm > envelope->last->x) {
		return 0.0;
	} else if (fabsl(xm - envelope->first->x) < 1e-40) {
                return envelope->first->x;
        } else if (fabsl(envelope->last->x - xm) < 1e-40) {
                return envelope->last->x;
        }

        x2 = x1 = xm;
	p = envelope->first;
	while (p) {
		if (xm < p->x) {
			x2 = p->x;
			y2 = p->y;
			break;
		}
		p = p->next;
	}

	p = envelope->last;
	while (p) {
		if (p->x < xm) {
			x1 = p->x;
			y1 = p->y;
			break;
		}
		p = p->prev;
	}

	if (fabsl(x2 - x1) < 1e-40) {
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
	memset(point, 0, sizeof(struct gkick_envelope_point));
	point->x = x;
	point->y = y;

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
                p = envelope->first->next;
		while (p) {
			if (point->x >= p->x) {
                                if (p->next != NULL) {
                                        p->next->prev = point;
                                }
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
			  double **buff,
			  size_t *npoints)
{
        struct gkick_envelope_point *p;
        double *points;
        size_t i;

        if (buff == NULL) {
                gkick_log_error("buff null");
                return;
        }

        *buff = NULL;
        if (env->npoints < 1) {
                return;
        }

        points = (double *)malloc(sizeof(double) * (2 * env->npoints));
        memset(points, 0, sizeof(double) * (2 * env->npoints));
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
                        break;
                }
                p = p->next;
                i++;
        }
}

void
gkick_envelope_update_point(struct gkick_envelope *env,
			    size_t index,
			    double x,
			    double y)
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
