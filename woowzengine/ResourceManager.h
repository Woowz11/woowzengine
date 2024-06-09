#pragma once

#define NOMINMAX 1
#define byte win_byte_override
#include "Windows.h"

#include <iostream>
#include "ResourceManager.h"
#include "resource.h"

HMODULE GCM();
HGLOBAL LoadImage_DefaultFont();
unsigned char* GetImageDataFromResource(HGLOBAL hResource);