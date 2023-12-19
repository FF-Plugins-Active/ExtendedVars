#pragma once

// UE Math Includes.
#include "Algo/Reverse.h"                       // Reverse array
#include "Kismet/KismetMathLibrary.h"

// UE String Includes.
#include "Containers/UnrealString.h"            // Hex to Bytes + Bytes to Hex
#include "Kismet/KismetStringLibrary.h"

// UE File Includes.
#include "Misc/Base64.h"                        // Encode & Decode Base64
#include "Misc/FileHelper.h"                    // Load File to Array
#include "HAL/FileManager.h"                    // Save texture as bitmap and select file from dialog.
#include "HAL/FileManagerGeneric.h"

// UE Widget Includes.
#include "Slate/WidgetRenderer.h"               // Widget to Texture2D
#include "Runtime/UMG/Public/UMG.h"             // Widget to Texture2D

// UE Render Includes.
#include "ImageUtils.h"                         // Save Texture as File
#include "RHICommandList.h"						// Encode Texture as JPG/BMP/PNG
#include "IImageWrapper.h"						// Encode Texture as JPG/BMP/PNG
#include "IImageWrapperModule.h"				// Encode Texture as JPG/BMP/PNG
#include "Kismet/KismetRenderingLibrary.h"	    // Texture2D
#include "MediaTexture.h"						// Media Texture

THIRD_PARTY_INCLUDES_START
#include <cstdlib>								// Bytes to Hex
#include <string>								// Bytes to Hex
#include <sstream>								// Bytes to Hex
#include <iostream>								// Bytes to Hex
#include <iomanip>								// Bytes to Hex
THIRD_PARTY_INCLUDES_END