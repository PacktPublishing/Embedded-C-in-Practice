#ifndef DRIVERS_LOG_H
#define DRIVERS_LOG_H

typedef enum
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level_t;

void log_init(void);
void log_print(log_level_t level, const char *message);

#endif // DRIVERS_LOG_H
