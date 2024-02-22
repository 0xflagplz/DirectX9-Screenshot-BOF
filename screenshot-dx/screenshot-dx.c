#include <windows.h>
#include <d3d9.h>
#include <stdio.h>
#pragma comment(lib, "d3d9.lib")

WINBASEAPI void __cdecl MSVCRT$free(void* memblock);
WINBASEAPI void* __cdecl MSVCRT$malloc(size_t _Size);
WINBASEAPI int __cdecl MSVCRT$printf(const char* _Format, ...);
WINBASEAPI int __cdecl MSVCRT$fclose(FILE *_File);
WINBASEAPI size_t __cdecl MSVCRT$fwrite(const void* _Str, size_t _Size, size_t _Count, FILE* _File);
WINBASEAPI FILE* WINAPI MSVCRT$fopen(const char* filename, const char* mode);
WINBASEAPI int WINAPI MSVCRT$sprintf(char* buffer, const char* format, ...);
DECLSPEC_IMPORT IDirect3D9* WINAPI D3D9$Direct3DCreate9(UINT SDKVersion);

#define RELEASE(x) if (x) { x->lpVtbl->Release(x); x = NULL; }

void SavePixelsToLogFile(int width, int height, int pitch, LPVOID pBits) {
    MSVCRT$printf("%doo%d.log\n", width, height);
    char fileName[256];
    MSVCRT$sprintf(fileName, "%doo%d.log", width, height);
    FILE* file = MSVCRT$fopen(fileName, "wb");
    if (file == NULL) {
        MSVCRT$printf("Failed to open file for writing: %s\n", fileName);
        return;
    }
    size_t bytesWritten = MSVCRT$fwrite(pBits, pitch * height, 1, file);
    if (bytesWritten != 1) {
        MSVCRT$printf("Failed to write pixel data to file: %s\n", fileName);
    }
    MSVCRT$fclose(file);
}

// BOF entry function
int go() {

    UINT adapter = D3DADAPTER_DEFAULT;
    HRESULT hr = S_OK;
    IDirect3DDevice9 *pDevice = NULL;
    IDirect3DSurface9* surface = NULL;
    D3DPRESENT_PARAMETERS parameters = { 0 };
    D3DDISPLAYMODE mode;
    D3DLOCKED_RECT rc;
    UINT pitch;
    LPBYTE* shots = NULL;

    IDirect3D9 *d3d = D3D9$Direct3DCreate9(D3D_SDK_VERSION);
    hr = d3d->lpVtbl->GetAdapterDisplayMode(d3d, D3DADAPTER_DEFAULT, &mode);
    if (FAILED(hr)) {
        MSVCRT$printf("GetAdapterDisplayMode Failed\n");
        return 0;
    }
    MSVCRT$printf("Resolution: %dx%d\n", mode.Width, mode.Height);

    parameters.Windowed = TRUE;
    parameters.BackBufferCount = 1;
    parameters.BackBufferHeight = mode.Height;
    parameters.BackBufferWidth = mode.Width;
    parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parameters.hDeviceWindow = NULL;

    D3DDEVTYPE Hardware = D3DDEVTYPE_HAL;
    hr = d3d->lpVtbl->CreateDevice(d3d, adapter, Hardware , NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &pDevice);
    if (FAILED(hr)) {
        return 1;
    }

    hr = pDevice->lpVtbl->CreateOffscreenPlainSurface(pDevice, mode.Width, mode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, NULL);
    if (FAILED(hr)) {
        MSVCRT$printf("CreateOffscreenPlainSurface Failed!\n");
        return 0;
    }
    hr = surface->lpVtbl->LockRect(surface, &rc, NULL, 0);
    if (FAILED(hr)) {
        MSVCRT$printf("LockRect Failed!\n");
        return 0;
    }
    pitch = rc.Pitch;
    hr = surface->lpVtbl->UnlockRect(surface);
    if (FAILED(hr)) {
        MSVCRT$printf("UnlockRect Failed!\n");
        return 0;
    }
    shots = (LPBYTE*)MSVCRT$malloc(1 * sizeof(LPBYTE));
    for (UINT i = 0; i < 1; i++) {
        shots[i] = (LPBYTE)MSVCRT$malloc(pitch * mode.Height);
    }

    hr = pDevice->lpVtbl->GetFrontBufferData(pDevice, 0, surface);
    if (FAILED(hr)) {return 0;}

    SavePixelsToLogFile(mode.Width, mode.Height, pitch, rc.pBits); 

    if (shots != NULL) {
        for (UINT i = 0; i < 1; i++) {
            MSVCRT$free(shots[i]);
        }
        MSVCRT$free(shots);
    }
    RELEASE(surface);
    RELEASE(pDevice);
    RELEASE(d3d);
    return 1;
}
