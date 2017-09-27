#include "geonkick_internal.h"
#include "oscillator.h"

enum geonkick_error
geonkick_create(struct geonkick **kick)
{
	if (kick == NULL) {
		return GEONKICK_ERROR_NULL_POINTER;
	}

	*kick = (struct geonkick*)malloc(sizeof(struct geonkick));
	if (*kick == NULL) {
		return GEONKICK_ERROR_MEM_ALLOC;
	}
	memset(*kick, 0, sizeof(struct geonkick));

	strcpy((*kick)->name, "GeonKick");
	(*kick)->length = 0.26; // One second;
	(*kick)->oscillators_number = 2;
	(*kick)->enable_midi_in = 1;

	if (pthread_mutex_init(&(*kick)->lock, NULL) != 0) {
                gkick_log_error("error on init mutex");
                geonkick_free(kick);
                return GEONKICK_ERROR_INIT_MUTEX;
	}

        if (gkick_create_oscillators(*kick) != GEONKICK_OK) {
                gkick_log_error("can't create oscillators");
                geonkick_free(kick);
                return GEONKICK_ERROR_CANT_CREATE_OSC;
	}

	if (gkick_create_jack(*kick)) {
                geonkick_free(kick);
                return GEONKICK_ERROR;
	}

	return GEONKICK_OK;
}

void geonkick_free(struct geonkick **kick)
{
        if (kick == NULL || *kick == NULL) {
                *kick = NULL;
                return;
        }

        gkick_jack_free(&((*kick)->jack));
        if ((*kick)->oscillators != NULL) {
                for (i = 0; i < (*kick)->oscillators_number; i++) {
                        gkick_osc_free(&((*kick)->oscillators[i]));
                }
                free((*kick)->oscillators);
        }

        pthread_mutex_destroy(&(*kick)->lock);
        free(*kick);
        *kick = NULL;
}

size_t
geonkick_get_oscillators_number(struct geonkick *kick)
{
        size_t n;

        if (kick == NULL || kick->oscillators == NULL) {
                return 0;
        }

        geonkick_lock(kick);
        n = kick->oscillators_number;
        geonkick_unlock(kick);

        return n;
}

enum geonkick_error
gkick_create_oscillators(struct geonkick *kick)
{
        size_t size;
        size_t i;
        struct gkick_oscillator *osc;

        size = sizeof(struct gkick_oscillator*)
                * kick->oscillators_number;
        kick->oscillators = (struct gkick_oscillator**)malloc(size);

        if (kick->oscillators == NULL) {
                return GEONKICK_ERROR_MEM_ALLOC;
        }
        memset(kick->oscillators, 0, size);

        for (i = 0; i < kick->oscillators_number; i++) {
                osc = gkick_osc_create(kick);
                if (osc == NULL) {
                        return GEONKICK_ERROR_CANT_CREATE_OSC;
                }
                kick->oscillators[i] = osc;
        }

        return GEONKICK_OK;
}


enum geonkick_error
geonkick_start(struct geonkick *kick)
{
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_remove_envelope_point(struct geonkick *kick,
                               enum geonkick_envelope_type type,
                               size_t index)
{
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_envelope_add_point(struct geonkick *kick,
				size_t osc_index,
				size_t env_index,
				double x,
				double y)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (kick == NULL) {
                return GEONKICK_ERROR_NULL_POINTER;
        }

        geonkick_lock(kick);

        osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscillator %d", osc_index);
                geonkick_unlock(kick);
                return GEONKICK_ERROR_NULL_POINTER;
        }

        env = NULL;
        if (env_index >=0 && env_index < osc->env_number
            && osc->envelopes != NULL) {
                env = osc->envelopes[env_index];
        }

        if (env == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR_NULL_POINTER;
        }

        if (gkick_envelope_add_point(env, x, y) == NULL) {
                gkick_log_error("can't get envelope");
                geonkick_unlock(kick);
                return GEONKICK_ERROR_NULL_POINTER;
        }

        geonkick_unlock(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_update_envelope_point(struct geonkick *kick,
			       enum geonkick_envelope_type type,
			       size_t index,
			       double x,
			       double y)
{
        return GEONKICK_OK;
}

double geonkick_get_oscillators_value(struct geonkick *kick, double t)
{
        double val = 0.0;
        size_t i;

        geonkick_lock(kick);
        val = 0.0;
        for (i = 0; i < kick->oscillators_number; i++) {
                val += gkick_osc_value(kick->oscillators[i], t, kick->length);
                gkick_osc_increment_phase(kick->oscillators[i], t, kick->length);
        }
        geonkick_unlock(kick);

        return val;
}

enum geonkick_error
geonkick_set_sample_rate(struct geonkick *kick, double rate)
{
        return GEONKICK_OK;
}

void geonkick_reset_oscillators(struct geonkick *kick)
{
        int i = 0;

        if (kick != NULL) {
                geonkick_lock(kick);
                for (i = 0; i < kick->oscillators_number; i++) {
                        (kick->oscillators[i])->phase = 0.0;
                }
                geonkick_unlock(kick);
        }
}

enum geonkick_error
geonkick_osc_envelope_get_points(struct geonkick *kick,
				 size_t osc_index,
				 size_t env_index,
				 double **buf,
				 size_t *npoints)
{
        if (kick == NULL || buf == NULL || npoints == NULL) {
                return GEONKICK_ERROR_NULL_POINTER;
        }

        *npoints = 0;
        *buf = NULL;
        geonkick_lock(kick);
        gkick_log_trace("get oscillator");
        struct gkick_oscillator *osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                gkick_log_error("can't get oscillator %d", osc_index);
                geonkick_unlock(kick);
                return GEONKICK_ERROR_NULL_POINTER;
        }
        gkick_osc_get_envelope_points(osc, env_index, buf, npoints);
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

struct gkick_oscillator*
geonkick_get_oscillator(struct geonkick *kick, size_t index)
{
        if (index >= 0 && index < kick->oscillators_number) {
                return kick->oscillators[index];
        } else {
                gkick_log_error("index = %d out of range", index);
        }

        return NULL;
}

void geonkick_lock(struct geonkick *kick)
{
        pthread_mutex_lock(&kick->lock);
}

void geonkick_unlock(struct geonkick *kick)
{
        pthread_mutex_unlock(&kick->lock);
}

enum geonkick_error
geonkick_osc_envelope_remove_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (kick == NULL) {
                return GEONKICK_ERROR_NULL_POINTER;
        }

        geonkick_lock(kick);
        osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get oscillator %d", osc_index);
                return GEONKICK_ERROR_NULL_POINTER;
        }

        env = NULL;
        if (env_index >=0 && env_index < osc->env_number
            && osc->envelopes != NULL) {
                env = osc->envelopes[env_index];
        }

        if (env == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get envelope");
                return GEONKICK_ERROR_NULL_POINTER;
        }

        gkick_envelope_remove_point(env, index);

        geonkick_unlock(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_osc_envelope_update_point(struct geonkick *kick,
				   size_t osc_index,
				   size_t env_index,
  				   size_t index,
				   double x,
				   double y)
{
        struct gkick_oscillator *osc;
        struct gkick_envelope   *env;

        if (kick == NULL) {
                return GEONKICK_ERROR_NULL_POINTER;
        }

        geonkick_lock(kick);
        osc = geonkick_get_oscillator(kick, osc_index);
        if (osc == NULL) {
                geonkick_unlock(kick);
                gkick_log_error("can't get oscillator %d", osc_index);
                return GEONKICK_ERROR_NULL_POINTER;
        }

        env = NULL;
        if (env_index >=0 && env_index < osc->env_number
            && osc->envelopes != NULL) {
                env = osc->envelopes[env_index];
        }

        if (env == NULL) {
                gkick_log_error("can't get envelope");
                geonkick_unlock(kick);
                return GEONKICK_ERROR_NULL_POINTER;
        }

        gkick_envelope_update_point(env, index, x, y);
        geonkick_unlock(kick);
        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_function(struct geonkick *kick,
			  size_t oscillatorIndex,
			  enum geonkick_osc_func_type type)
{
        struct gkick_oscillator *osc;
        gkick_log_debug("SET FUNCTION: %d", oscillatorIndex);
        if (kick == NULL) {
                return GEONKICK_ERROR_NULL_POINTER;
        }

        geonkick_lock(kick);
        osc = geonkick_get_oscillator(kick, oscillatorIndex);
        if (osc != NULL) {
                osc->func = type;
        }
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_length(struct geonkick *kick, double t)
{
        if (kick == NULL) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        kick->length = t;
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_length(struct geonkick *kick, double *t)
{
        if (kick == NULL || t == 0) {
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        *t = kick->length;
        geonkick_unlock(kick);

        return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_frequency_val(struct geonkick *kick,
                               size_t osc_index,
                               double v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	osc->frequency = v;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_set_osc_amplitude_val(struct geonkick *kick,
                               size_t osc_index,
                               double v)
{
	if (kick == NULL) {
		return GEONKICK_ERROR;
	}

	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	osc->amplitude = v;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_osc_amplitude_val(struct geonkick *kick,
			       size_t osc_index,
			       double *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

	*v = 0.0;
	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	*v = osc->amplitude;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_get_osc_frequency_val(struct geonkick *kick,
			       size_t osc_index,
			       double *v)
{
	if (kick == NULL || v == NULL) {
		return GEONKICK_ERROR;
	}

	*v = 0.0;
	geonkick_lock(kick);
	struct gkick_oscillator* osc = geonkick_get_oscillator(kick, osc_index);
	if (osc == NULL) {
		gkick_log_error("can't get oscillator");
		geonkick_unlock(kick);
		return GEONKICK_ERROR;
	}
	*v = osc->frequency;
	geonkick_unlock(kick);
	return GEONKICK_OK;
}

enum geonkick_error
geonkick_enable_midi_in(struct geonkick *kick, const char *name, int enable)
{
        if (kick == NULL || name != NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        return gkick_jack_set_midi_in(kick->jack, name, enable);
}

enum geonkick_error
geonkick_play(struct geonkick *kick, int play)
{
        if (kick == NULL || name != NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        kick->is_play = play;
        geonkick_unlock(kick);
}

int
geonkick_is_play(struct geonkick *kick, enum geonkick_error *error)
{
        int is_play;

        if (kick == NULL || name != NULL) {
                gkick_log_error("wrong arugment");
                if (error != NULL) {
                        *error = GEONKICK_ERROR;
                }
                return 0;
        }

        geonkick_lock(kick);
        is_play = kick->is_play;
        geonkick_unlock(kick);

        return is_play;
}

int geonckick_is_play_stopped(struct geonkick *kick)
{
        int is_end;

        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        is_end = 0;
        geonkick_lock(kick);
        if (kick->current_time > kick->length) {
                is_end = 1;
        }
        geonkick_unlock(kick);

        return is_end;
}

enum geonkick_error
geonkick_start_play(struct geonkick *kick)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        kick->current_time = 0.0;
        kick->play = 1;
        geonkick_unlock(kick);
}

enum geonkick_error
geonkick_stop_play(struct geonkick *kick)
{
        if (kick == NULL) {
                gkick_log_error("wrong arugment");
                return GEONKICK_ERROR;
        }

        geonkick_lock(kick);
        kick->current_time = 0.0;
        kick->play = 0;
        geonkick_unlock(kick);
}
