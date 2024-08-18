#include "pch.h"

#ifdef _DEBUG

#ifdef _WIN64
#define LOG_FILE "\\NPWrapper64.txt"
#else
#define LOG_FILE "\\NPWrapper.txt"
#endif

// It's intentional that this function does exit(1) in case of failure.
// It's only used in debug code and inability to create the log needs to
// be communicated somehow.
void log_open(FILE** logfile, char* mode)
{
    char buf[MAX_PATH] = { 0 };
    DWORD result = GetEnvironmentVariableA("USERPROFILE", buf, MAX_PATH);
    if (result <= 0 || result >= MAX_PATH)
        exit(1);
    strcat_s(buf, MAX_PATH, LOG_FILE);
    fopen_s(logfile, buf, mode);
    if (*logfile == NULL)
        exit(1);
}

#endif
