
        gkick_log_debug("limit: %.5f", limit);
        if (limit < 0) {
                limit = 0;
        } else if (limit > 1.0) {
                limit = 1.0;
        } else {
                geonkick_lock(kick);
                kick->limiter = 0.1 * limit;
                geonkick_unlock(kick);
        }


-----
gkick_audio_set_limiter_val(kick->audio_dev, limit);
