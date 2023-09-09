// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Custom Includes.
#include "Extended_Enums.h"

// UE Includes.
#include "Engine/FontFace.h"

#include "ExtendedVarsBPLibrary.generated.h"

UCLASS(BlueprintType)
class EXTENDEDVARS_API UBytesObject_64 : public UObject
{
	GENERATED_BODY()

public:

	TArray64<uint8> ByteArray;

};

UCLASS(BlueprintType)
class EXTENDEDVARS_API UBytesObject_32 : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> ByteArray;

};

UCLASS(BlueprintType)
class EXTENDEDVARS_API URuntimeFont : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	UFont* Font;

	UFontFace* Font_Face;

};

UCLASS()
class UExtendedVarsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// Read Group.
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Android Path Helper", ToolTip = "This node automatically gives Internal Storage absolute path. So, you just need to define sub-folder and file. Example: Download/sample.pdf", Keywords = "android, ios, mobile, folder, file, path, helper, absolute"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API FString Android_Path_Helper(FString In_FileName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read File from Path x64", ToolTip = "You need to use absolute path.", Keywords = "read, load, path, bytes, import, file"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API bool Read_File_From_Path_x64(UBytesObject_64*& Out_Bytes_Object, FString In_Path, bool bUseLowLevel = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read File from Path x32", ToolTip = "You need to use absolute path.", Keywords = "read, load, path, bytes, import, file"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API bool Read_File_From_Path_x32(UBytesObject_32*& Out_Bytes_Object, FString In_Path, bool bUseLowLevel = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Text from Path", ToolTip = "You need to use absolute path.", Keywords = "read, load, path, bytes, import, text"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API bool Read_Text_From_Path(FString& Out_String, FString In_Path);

	// Convert To Bytes.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "String to Bytes", Keywords = "bytes, string"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API TArray<uint8> String_To_Bytes(FString In_String);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Base64 to Bytes", Keywords = "bytes, base64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API TArray<uint8> Base64_To_Bytes(FString In_Base64, bool bUseUrl);

	// Convert From Bytes.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes To String", Keywords = "bytes, string"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_To_String(TArray<uint8> In_Bytes, bool bUseUnreal = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bytes to Hex", ToolTip = "", Keywords = "bytes, string, hex"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_To_Hex(TArray<uint8> In_Bytes, int32 In_Size);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bytes to Each Hex", ToolTip = "", Keywords = "bytes, string, hex"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_To_Each_Hex(TArray<uint8> In_Bytes, int32 Index);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes To Base64", Keywords = "bytes, base64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_To_Base64(TArray<uint8> In_Bytes, bool bUseUrl);

	// Convert To Bytes Objects.

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bytes to Bytes64 Object", ToolTip = "", Keywords = "bytes, object, x64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API bool Bytes_To_B64_OBject(UBytesObject_64*& Out_Bytes_Object, TArray<uint8> In_Bytes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bytes to Bytes32 Object", ToolTip = "", Keywords = "bytes, object, x86, 32"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API bool Bytes_To_B32_OBject(UBytesObject_32*& Out_Bytes_Object, TArray<uint8> In_Bytes);

	// Convert From Bytes Objects

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes Object To Bytes", ToolTip = "It works with only 32 bit objects because blueprints doesn't support TArray64.", Keywords = "bytes, object"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API TArray<uint8> B32_Object_To_Bytes(UBytesObject_32* In_Bytes_Object);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes Object To String", Keywords = "bytes, string"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_Object_To_String(UObject* In_Bytes_Object, bool bUseUnreal = false);

	// Fonts Group.

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Runtime Font Load", ToolTip = "", Keywords = "runtime, font, load"), Category = "Extended Variables|Font")
	static EXTENDEDVARS_API URuntimeFont* RuntimeFont_Load(TArray<uint8> In_Bytes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Runtime Font Release", ToolTip = "", Keywords = "runtime, font, release"), Category = "Extended Variables|Font")
	static EXTENDEDVARS_API bool RuntimeFont_Release(UPARAM(ref)URuntimeFont*& In_RuntimeFont);

	// String Group.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Base64 URL to String", ToolTip = "Description.", Keywords = "sort, string, fstring, ascending, descending"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool Base64ToString(FString In_Base64, FString& OutDecoded);
		
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int64 To String", Keywords = "int64, string, fstring, convert"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API FString Int64ToString(int64 TargetInt64);
		
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Strings", ToolTip = "Description.", Keywords = "sort, string, fstring, ascending, descending"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API TArray<FString> StringSort(TArray<FString> TargetArray, bool bIsDescending);

	// Math Group | Integer.

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

	// Math Group | Float.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Float Precision", ToolTip = "Description.", Keywords = "float, get, precision"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API float FloatPrecision(float TargetFloat, int32 Precision);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Fraction Count", ToolTip = "Description.", Keywords = "float, get, fraction, count"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API int32 FloatFractionCount(float TargetFloat);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Round Next Float", ToolTip = "Description.", Keywords = "float, round, next"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API float FloatRoundNext(float TargetFloat, double Decimal);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Floats", ToolTip = "Description.", Keywords = "sort, float, ascending, descending"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API TArray<float> FloatSort(TArray<float> TargetArray, bool bIsDescending);
	
	// Time Group.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Times", ToolTip = "Description.", Keywords = "sort, times, ascending, descending"), Category = "Extended Variables|Time")
	static EXTENDEDVARS_API TArray<FDateTime> TimeSort(TArray<FDateTime> TargetArray, bool bIsDescending);

	// Render Group.

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Widget To Texture Render Target 2D", Keywords = "texture, render, target, 2d, widget, convert"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API UTextureRenderTarget2D* Widget_To_TRT2D(FString& OutCode, UUserWidget* InWidget, FVector2D InDrawSize);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Texture Render Target 2D to UTexture2D", Keywords = "texture, render, target, 2d, widget, convert"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API UTexture2D* TRT2D_To_T2D(UTextureRenderTarget2D* In_TRT_2D);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Texture2D As Colors", Keywords = "t2d, texture2d, utexture2d, get, export, color, fcolor, array"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Export_T2D_Colors(TArray<FColor>&Out_Array, UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Texture2D As Bitmap", Keywords = "t2d, texture2d, utexture2d, export, bitmap, bmp"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Export_T2D_Bitmap(FString& Out_Path, UTexture2D* Texture, FString In_Path, bool bUseTemp);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Texture2D As Jpeg", Keywords = "t2d, texture2d, utexture2d, export, jpeg, jpg"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Export_T2D_Jpeg(FString& Out_Path, UTexture2D* Texture, FString In_Path, bool bUseTemp);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Export Texture2D As Bytes Low Level", Keywords = "t2d, texture2d, utexture2d, export, byte, bytes, low, level"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Export_T2D_Bytes(TArray<uint8>& Out_Bytes, UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Import Texture2D From Bytes", Keywords = "t2d, texture2d, utexture2d, import, create, bytes"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Import_T2D_Bytes(UTexture2D*& Out_Texture, TArray<uint8> In_Bytes, bool bUseSrgb);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Import Texture2D From Bytes Low Level", Keywords = "t2d, texture2d, utexture2d, import, create, bytes, low, level"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Import_T2D_Bytes_LL(UTexture2D*& Out_Texture, TArray<uint8> In_Bytes, FVector2D Size, bool bUseSrgb);

};