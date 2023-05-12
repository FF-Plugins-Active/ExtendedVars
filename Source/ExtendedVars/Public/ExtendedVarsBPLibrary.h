// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Extended_Enums.h"

#include "ExtendedVarsBPLibrary.generated.h"

UCLASS(BlueprintType)
class EXTENDEDVARS_API UBytesObject : public UObject
{
	GENERATED_BODY()

public:

	TArray64<uint8> ByteArray;

};

UCLASS()
class UExtendedVarsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// Bytes Group

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bytes Array To String", Keywords = "http, web, server, api, bind, route, post"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString BytesArrayToString(TArray<uint8> In_Bytes);
		
	UFUNCTION(BlueprintPure, meta = (DisplayName = "String to Bytes", Keywords = "string, to, bytes"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API TArray<uint8> StringToBytesArray(FString In_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read from Path", ToolTip = "You need to use absolute platform path.", Keywords = "read, load, path, bytes"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API bool Read_From_Path(UBytesObject*& Out_Bytes_Object, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bytes to Bytes Object", ToolTip = "", Keywords = "read, load, path"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API bool BytesToBytesObject(UBytesObject*& Out_Bytes_Object, TArray<uint8> In_Bytes);
	
	// String Group

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Base64 URL to String", ToolTip = "Description.", Keywords = "sort, string, fstring, ascending, descending"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool Base64ToString(FString In_Base64, FString& OutDecoded);
		
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int64 To String", Keywords = "int64, string, fstring, convert"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API FString Int64ToString(int64 TargetInt64);
		
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Strings", ToolTip = "Description.", Keywords = "sort, string, fstring, ascending, descending"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API TArray<FString> StringSort(TArray<FString> TargetArray, bool bIsDescending);

	// Math Group | Integer

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Number to Byte", ToolTip = "It converts number to a byte form.", Keywords = "byte, string, fstring, convert"), Category = "Extended Variables|Math")
	static EXTENDEDVARS_API uint8 NumberToByte(int32 In_Number);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Int32 Place Family", Keywords = "int32, get, place, family"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API int32 Int32PlaceFamily(int32 TargetInteger);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 Truncate to Whole (Big)", Keywords = "int32, truncate, truncation, full, whole, big"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API int32 Int32TruncateToWholeBig(int32 TargetInteger);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 Truncate to Whole (Small)", Keywords = "int32, truncate, truncation, full, whole, small"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API int32 Int32TruncateToWholeSmall(int32 TargetInteger);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Integers", ToolTip = "Description.", Keywords = "sort, int32, ascending, descending"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API TArray<int32> Int32Sort(TArray<int32> TargetArray, bool bIsDescending);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 To Graphics", ToolTip = "Unit Value gives scale as a float between 0 and 1.", Keywords = "int32, graphics, pie"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API void Int32ToGraphics(EGraphicsType GraphicsType, int32 TargetInteger, int32 FullInteger, float& Scale, float& UnitValue);

	// Math Group | Float

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Fraction Count", ToolTip = "Description.", Keywords = "float, get, fraction, count"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API int32 FloatFractionCount(float TargetFloat);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Round Next Float", ToolTip = "Description.", Keywords = "float, round, next"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API float FloatRoundNext(float TargetFloat, double Decimal);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Floats", ToolTip = "Description.", Keywords = "sort, float, ascending, descending"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API TArray<float> FloatSort(TArray<float> TargetArray, bool bIsDescending);
	
	// Time Group

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Times", ToolTip = "Description.", Keywords = "sort, times, ascending, descending"), Category = "Extended Variables|Time")
	static EXTENDEDVARS_API TArray<FDateTime> TimeSort(TArray<FDateTime> TargetArray, bool bIsDescending);

	// Render Group

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Widget To Texture Render Target 2D", Keywords = "texture, render, target, 2d, widget, convert"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API UTextureRenderTarget2D* WidgetToTextureRenderTarget2d(FString& OutCode, UUserWidget* InWidget, FVector2D InDrawSize);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Colors Array of Texture2D", Keywords = "t2d, texture2d, utexture2d, create, get, export, color, fcolor, array"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Texture2dColorsArray(TArray<FColor>&Out_Array, UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Texture2D As Bitmap", Keywords = "t2d, texture2d, utexture2d, bitmap, export, create"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool ExportT2dAsBitmap(UTexture2D* Texture, FString Path);

	// Path Group
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Android Path Helper", ToolTip = "This node automatically gives Internal Storage absolute path. So, you just need to define sub-folder and file. Example: Download/sample.pdf", Keywords = "android, ios, mobile, folder, file, path, helper, absolute"), Category = "Extended Variables|Path")
	static EXTENDEDVARS_API FString Android_Path_Helper(FString InFileName);

};