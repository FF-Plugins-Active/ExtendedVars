#pragma once

// UE Math Includes.
#include "Algo/Reverse.h"                       // Reverse array
#include "Kismet/KismetMathLibrary.h"

// UE File Includes.
#include "Misc/Base64.h"                        // Encode & Decode Base64
#include "Misc/FileHelper.h"                    // Load File to Array
#include "HAL/FileManager.h"                    // Save texture as bitmap and select file from dialog.
#include "HAL/FileManagerGeneric.h"

// UE Widget Includes.
#include "Slate/WidgetRenderer.h"               // Widget to Texture2D
#include "Runtime/UMG/Public/UMG.h"             // Widget to Texture2D

// UE Render Includes.
#include "RHICommandList.h"
#include "ImageUtils.h"                         // Save Texture as Jpeg
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageCore.h"
#include "Kismet/KismetRenderingLibrary.h"	    // Texture2D
#include "MediaTexture.h"

// UE String Includes.
#include "Containers/UnrealString.h"            // Hex to Bytes + Bytes to Hex
#include "Kismet/KismetStringLibrary.h"

THIRD_PARTY_INCLUDES_START
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#ifdef _WIN64
#include <numbers>                              // C++20 math library. Windowas only.
#include <cmath>
#endif

THIRD_PARTY_INCLUDES_END