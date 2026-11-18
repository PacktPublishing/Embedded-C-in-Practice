#ifndef DRIVERS_LOG_H
#define DRIVERS_LOG_H

#define LOG_LEVEL_DEBUG   0
#define LOG_LEVEL_INFO    1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_ERROR   3

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level_t;

void log_init(void);
void log_print(log_level_t level, const char *message);

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define log_debug(message) log_print(LOG_DEBUG, (message))
#else
#define log_debug(message) ((void)0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_INFO
#define log_info(message) log_print(LOG_INFO, (message))
#else
#define log_info(message) ((void)0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_WARNING
#define log_warning(message) log_print(LOG_WARNING, (message))
#else
#define log_warning(message) ((void)0)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define log_error(message) log_print(LOG_ERROR, (message))
#else
#define log_error(message) ((void)0)
#endif

#endif // DRIVERS_LOG_H
