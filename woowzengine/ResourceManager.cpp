#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include "ResourceManager.h"
#include "resource.h"
#include "Base.h"

HMODULE GCM() {
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GCM,
		&hModule);
	return hModule;
}

HGLOBAL LoadImage_DefaultFont() {
    HMODULE hModule = GCM();
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDB_PNG_DEFAULT_FONT), RT_RCDATA);
    if (hResource == NULL) {
        return NULL;
    }

    HGLOBAL hResourceData = LoadResource(hModule, hResource);
    if (hResourceData == NULL) {
        return NULL;
    }

    return hResourceData;
}

unsigned char* GetImageDataFromResource(HGLOBAL hResource) {
    unsigned char* resourceData = reinterpret_cast<unsigned char*>(LockResource(hResource));

    if (resourceData == NULL) {
        PF("Failed to get an image data from the resources! GetImageDataFromResource()","C0031",true);
    }
	return resourceData;
}