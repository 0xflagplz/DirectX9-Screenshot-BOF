#include <windows.h>
#include <d3d9.h>
#include <stdio.h>

DECLSPEC_IMPORT HRESULT WINAPI OLE32$CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
DECLSPEC_IMPORT void WINAPI OLE32$CoUninitialize(void);
WINBASEAPI void __cdecl MSVCRT$free(void* memblock);
WINBASEAPI void* __cdecl MSVCRT$malloc(size_t _Size);
DECLSPEC_IMPORT char* WINAPI MSVCRT$getenv(const wchar_t *varname);
WINBASEAPI int __cdecl MSVCRT$printf(const char* _Format, ...);
WINBASEAPI int __cdecl MSVCRT$fclose(FILE *_File);
WINBASEAPI void *__cdecl MSVCRT$calloc(size_t number, size_t size);
WINBASEAPI size_t __cdecl MSVCRT$fwrite(const void* _Str, size_t _Size, size_t _Count, FILE* _File);
WINBASEAPI FILE* WINAPI MSVCRT$fopen(const char* filename, const char* mode);
WINBASEAPI int WINAPI MSVCRT$sprintf(char* buffer, const char* format, ...);
WINBASEAPI int __cdecl MSVCRT$rand();
WINBASEAPI size_t WINAPI MSVCRT$strlen(const char* str);
WINBASEAPI void __cdecl MSVCRT$srand(int initial);
WINBASEAPI size_t __cdecl MSVCRT$strftime(char* strDest, size_t maxsize, const char* format, const struct tm* timeptr);
WINBASEAPI time_t __cdecl MSVCRT$time(time_t* timer);
WINBASEAPI struct tm* __cdecl MSVCRT$localtime(const time_t* _Time);
DECLSPEC_IMPORT IDirect3D9* WINAPI D3D9$Direct3DCreate9(UINT SDKVersion);
