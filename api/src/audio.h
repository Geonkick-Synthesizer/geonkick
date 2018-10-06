struct audio_device {
        struct gkick_jack *jack;
        int is_play;
        double limiter;
        pthread_mutex_t lock;
};
