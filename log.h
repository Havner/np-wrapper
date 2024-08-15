#pragma once

#ifdef _DEBUG

#include <stdio.h>
#include <stdlib.h>

#define LOG_FILE "C:/Users/Havner/np.txt"

#define LOG_INIT(mode)                              \
    FILE* logfile = NULL;                           \
    do {                                            \
        fopen_s(&logfile, LOG_FILE, mode);          \
        if (logfile == NULL)                        \
            exit(1);                                \
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
