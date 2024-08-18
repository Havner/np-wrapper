#pragma once

#ifdef _DEBUG

void log_open(FILE** logfile, char* mode);

#define LOG_INIT(mode)                              \
    FILE* logfile = NULL;                           \
    do {                                            \
        log_open(&logfile, mode);                   \
    } while(0)

#define LOG(...)                                    \
    do {                                            \
        fprintf(logfile, __VA_ARGS__);              \
    } while(0)

#define LOG_CLOSE()                                 \
    do {                                            \
        if (logfile != NULL) {                      \
            fclose(logfile);                        \
            logfile = NULL;                         \
        }                                           \
    } while (0)

#else // _DEBUG

#define LOG_INIT(mode)
#define LOG(...)
#define LOG_CLOSE()

#endif // _DEBUG
