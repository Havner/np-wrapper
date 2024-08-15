// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "log.h"

// Don't use any allocators in this file, something conflicts with BMS

#define FALCON4_ID 1901

// simple config in degrees
typedef struct {
    bool use;
    float roll, pitch, yaw;
} config_t;

static config_t config = { false, 0, 5, 0 };

#pragma pack(push, 1)
extern "C" typedef struct {
    char DllSignature[200];
    char AppSignature[200];
} tir_signature_t;
#pragma pack(pop)

#pragma pack(push, 1)
extern "C" typedef struct {
    __int16 status;
    __int16 frame;
    unsigned __int32 cksum;
    float roll, pitch, yaw;
    float tx, ty, tz;
    float rawx, rawy, rawz;
    float smoothx, smoothy, smoothz;
} tir_data_t;
#pragma pack(pop)

typedef struct {
    HMODULE hlibrary;
    __int32(__stdcall* RegisterWindowHandle)(HWND hwnd);
    __int32(__stdcall* UnregisterWindowHandle)(void);
    __int32(__stdcall* RegisterProgramProfileID)(unsigned __int16 id);
    __int32(__stdcall* QueryVersion)(unsigned __int16* version);
    __int32(__stdcall* RequestData)(unsigned __int16 req);
    __int32(__stdcall* GetSignature)(tir_signature_t* sig);
    __int32(__stdcall* GetData)(tir_data_t* data);
    __int32(__stdcall* GetParameter)(void);
    __int32(__stdcall* SetParameter)(void);
    __int32(__stdcall* StartCursor)(void);
    __int32(__stdcall* StopCursor)(void);
    __int32(__stdcall* ReCenter)(void);
    __int32(__stdcall* StartDataTransmission)(void);
    __int32(__stdcall* StopDataTransmission)(void);
} np_state;

static np_state state = { 0 };

#define EXPORT(ret) extern "C" ret __declspec(dllexport)

EXPORT(__int32) NP_RegisterWindowHandle(HWND hwnd) { return state.RegisterWindowHandle(hwnd); }
EXPORT(__int32) NP_UnregisterWindowHandle(void) { return state.UnregisterWindowHandle(); }
EXPORT(__int32) NP_QueryVersion(unsigned __int16* version) { return state.QueryVersion(version); }
EXPORT(__int32) NP_RequestData(unsigned __int16 req) { return state.RequestData(req); }
EXPORT(__int32) NP_GetSignature(tir_signature_t* sig) { return state.GetSignature(sig); }
EXPORT(__int32) NP_GetParameter(void) { return state.GetParameter(); }   // TODO: I don't trust this definition
EXPORT(__int32) NP_SetParameter(void) { return state.SetParameter(); }   // TODO: I don't trust this definition
EXPORT(__int32) NP_StartCursor(void) { return state.StartCursor(); }
EXPORT(__int32) NP_StopCursor(void) { return state.StopCursor(); }
EXPORT(__int32) NP_ReCenter(void) { return state.ReCenter(); }
EXPORT(__int32) NP_StartDataTransmission(void) { return state.StartDataTransmission(); }
EXPORT(__int32) NP_StopDataTransmission(void) { return state.StopDataTransmission(); }

EXPORT(__int32) NP_RegisterProgramProfileID(unsigned __int16 id)
{
    config.use = id == FALCON4_ID;
    return state.RegisterProgramProfileID(id);
}

#define PI 3.14159265358979323846

EXPORT(__int32) NP_GetData(tir_data_t* data)
{
    __int32 ret = state.GetData(data);

    if (config.use) {
        if (config.roll != 0) {
            data->roll += (float)(config.roll * 8191 * PI / 180);
        }
        if (config.pitch != 0) {
            data->pitch += (float)(config.pitch * 8191 * PI / 180);
        }
        if (config.yaw != 0) {
            data->yaw += (float)(config.yaw * 8191 * PI / 180);
        }
    }

    return ret;
}

#define BUF_SIZE 2048

static bool initialize(void)
{
    HKEY hkey = NULL;
    DWORD size = BUF_SIZE - 1;
    char buf[BUF_SIZE] = { 0 };
    const char* function = NULL;

    LOG_INIT();

    LOG("Opening registry\n");
    RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\NaturalPoint\\NATURALPOINT\\NPClient Location", 0, KEY_QUERY_VALUE, &hkey);
    if (!hkey) {
        LOG("Error opening registry\n");
        goto error;
    }
    LOG("Reading registry key\n");
    if (RegQueryValueExA(hkey, "Path", NULL, NULL, (unsigned char*)buf, &size) != ERROR_SUCCESS) {
        LOG("Error reading registry key\n");
        goto error;
    }
    buf[size] = '\0';
    LOG("Read file path: %s\n", buf);
    RegCloseKey(hkey);
    hkey = NULL;

    /* in the registry path there's normally a trailing slash already */
#ifdef _WIN64
    strcat_s(buf, "NPClient64-orig.dll");
#else
    strcat_s(buf, "NPClient-orig.dll");
#endif

    memset(&state, 0, sizeof(np_state));

    LOG("Opening library: %s\n", buf);
    state.hlibrary = LoadLibraryA(buf);
    if (state.hlibrary == NULL) {
        LOG("Error opening library");
        goto error;
    }

    LOG("Looking for functions\n");
    function = "NP_RegisterWindowHandle";
    if ((state.RegisterWindowHandle = (__int32(__stdcall*)(HWND))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_UnregisterWindowHandle";
    if ((state.UnregisterWindowHandle = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_RegisterProgramProfileID";
    if ((state.RegisterProgramProfileID = (__int32(__stdcall*)(unsigned __int16))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_QueryVersion";
    if ((state.QueryVersion = (__int32(__stdcall*)(unsigned __int16*))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_RequestData";
    if ((state.RequestData = (__int32(__stdcall*)(unsigned __int16))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_GetSignature";
    if ((state.GetSignature = (__int32(__stdcall*)(tir_signature_t*))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_GetData";
    if ((state.GetData = (__int32(__stdcall*)(tir_data_t*))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_GetParameter";
    if ((state.GetParameter = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_SetParameter";
    if ((state.SetParameter = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_StartCursor";
    if ((state.StartCursor = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_StopCursor";
    if ((state.StopCursor = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_ReCenter";
    if ((state.ReCenter = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_StartDataTransmission";
    if ((state.StartDataTransmission = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = "NP_StopDataTransmission";
    if ((state.StopDataTransmission = (__int32(__stdcall*)(void))GetProcAddress(state.hlibrary, function)) == NULL)
        goto error;
    function = NULL;

    LOG("Initialization complete\n");
    LOG_CLOSE();

    return true;

error:
    if (function != NULL) {
        LOG("Error finding function: %s\n", function);
    }
    if (hkey != NULL) {
        RegCloseKey(hkey);
    }
    LOG_CLOSE();

    return false;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        return initialize();
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
