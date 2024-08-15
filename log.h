#pragma once

#ifdef _DEBUG

#include <cstdio>

static FILE* logfile = NULL;
#define LOG_FILE "C:/Users/Havner/np.txt"
#define LOG_INIT()                                  \
    do {                                            \
        fopen_s(&logfile, LOG_FILE, "w");           \
        if (logfile == NULL)                        \
            return false;                           \
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

#define LOG_INIT()
#define LOG(...)
#define LOG_CLOSE()

#endif // _DEBUG
