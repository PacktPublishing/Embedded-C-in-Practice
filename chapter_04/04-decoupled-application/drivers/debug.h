#ifndef DRIVERS_DEBUG_H
#define DRIVERS_DEBUG_H

typedef enum
{
    DEBUG_INFO,
    DEBUG_WARNING,
    DEBUG_ERROR
} debug_level_t;

void debug_init(void);
void debug_print(debug_level_t level, const char *message);

#endif // DRIVERS_DEBUG_H
