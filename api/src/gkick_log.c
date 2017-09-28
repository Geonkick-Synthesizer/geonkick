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
