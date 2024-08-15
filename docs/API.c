// DLL

NPPriv_ClientNotify
NPPriv_GetLastError
NPPriv_SetData
NPPriv_SetLastError
NPPriv_SetParameter
NPPriv_SetSignature
NPPriv_SetVersion
NP_GetData
NP_GetParameter
NP_GetSignature
NP_QueryVersion
NP_ReCenter
NP_RegisterProgramProfileID
NP_RegisterWindowHandle
NP_RequestData
NP_SetParameter
NP_StartCursor
NP_StartDataTransmission
NP_StopCursor
NP_StopDataTransmission
NP_UnregisterWindowHandle


// https://github.com/opentrack/opentrack/blob/master/contrib/trackir-client/client.c

__int32 (__stdcall* tir_reghwnd)(HWND hwnd);
__int32 (__stdcall* tir_regprogid)(__int16 progid);
__int32 (__stdcall* tir_queryver)(__int16* version);
__int32 (__stdcall* tir_reqdata)(__int16 bitmask);
__int32 (__stdcall* tir_stopcurs)(void);
__int32 (__stdcall* tir_startxmit)(void);
__int32 (__stdcall* tir_getdata)(tir_headpose_t* pose);

#pragma pack(push, 1)
typedef struct {
	__int16 status; /* zero means success, it's easy to forget! */
	__int16 frame; /* if idempotent, device paused */
	__int32 padding;
	float roll, pitch, yaw; /* divide by 8191 when received, in radians */
	float x, y, z; /* between -16383 and 16383 */
	float padding2[9];
} tir_headpose_t;
#pragma pack(pop)


// https://github.com/johnflux/python_trackir/blob/master/trackir.py

// We have now loaded the DLL.  This has the following functions:
int NP_RegisterWindowHandle(HWND hwnd);
int NP_UnregisterWindowHandle(void);
int NP_RegisterProgramProfileID(unsigned short id);
int NP_QueryVersion(unsigned short *version);
int NP_RequestData(unsigned short req);
int NP_GetSignature(tir_signature *sig);
int NP_GetData(tir_data *data);
int NP_GetParameter(void);
int NP_SetParameter(void);
int NP_StartCursor(void);
int NP_StopCursor(void);
int NP_ReCenter(void);
int NP_StartDataTransmission(void);
int NP_StopDataTransmission(void);

#pragma pack(push, 1)
typedef struct {
	char DllSignature[200];
	char AppSignature[200];
} tir_signature;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	short status;
	short frame;
	unsigned int cksum;
	float roll, pitch, yaw;
	float tx, ty, tz;
	float rawx, rawy, rawz;
	float smoothx, smoothy, smoothz;
} tir_data;
#pragma pack(pop)
