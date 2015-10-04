//log.h
//debug.h
#define LOG_NONE 0
#define LOG_ERROR 1
#define LOG_INFO 2
#define LOG_DEBUG 3
#define LOG_ALL 99



void log_string(int level, char *s, int sz, int cn);
void log_set_level(int level);
void log_init();
void log_log(int level, const char *str, ...);
