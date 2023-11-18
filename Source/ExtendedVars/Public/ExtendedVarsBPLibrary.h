// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

// Custom Includes.
#include "Extended_Enums.h"

// UE Includes.
#include "Engine/FontFace.h"

#include "ExtendedVarsBPLibrary.generated.h"

// Select folder from dialog (each content).
USTRUCT(BlueprintType)
struct EXTENDEDVARS_API FFolderContent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString Path = "";

	UPROPERTY(BlueprintReadOnly)
	FString Name = "";

	UPROPERTY(BlueprintReadOnly)
	bool bIsFile = false;
};

// Select folder from dialog (array container).
USTRUCT(BlueprintType)
struct EXTENDEDVARS_API FFolderContentArray
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TArray<FFolderContent> OutContents;
};

UCLASS(BlueprintType)
class EXTENDEDVARS_API UBytesObject_64 : public UObject
{
	GENERATED_BODY()

public:

	TArray64<uint8> ByteArray;

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

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDelegateSearch, bool, bIsSearchSuccessful, FString, ErrorCode, FFolderContentArray, Out);

UCLASS()
class UExtendedVarsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// Fonts Group.

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Runtime Font Load", ToolTip = "", Keywords = "runtime, font, load"), Category = "Extended Variables|Font")
	static EXTENDEDVARS_API URuntimeFont* Runtime_Font_Load(TArray<uint8> In_Bytes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Runtime Font Release", ToolTip = "", Keywords = "runtime, font, release"), Category = "Extended Variables|Font")
	static EXTENDEDVARS_API bool Runtime_Font_Release(UPARAM(ref)URuntimeFont*& In_RuntimeFont);

	// Sorters

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Strings", ToolTip = "Description.", Keywords = "string, fstring, ascending, descending, sort"), Category = "Extended Variables|Sorters")
	static EXTENDEDVARS_API TArray<FString> Sort_String(TArray<FString> TargetArray, bool bIsDescending);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Integers", ToolTip = "Description.", Keywords = "sort, int32, ascending, descending"), Category = "Extended Variables|Sorters")
	static EXTENDEDVARS_API TArray<int32> Sort_Int32(TArray<int32> TargetArray, bool bIsDescending);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Floats", ToolTip = "Description.", Keywords = "sort, float, ascending, descending"), Category = "Extended Variables|Sorters")
	static EXTENDEDVARS_API TArray<float> Sort_Float(TArray<float> TargetArray, bool bIsDescending);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Floats", ToolTip = "Description.", Keywords = "sort, float, ascending, descending"), Category = "Extended Variables|Sorters")
	static EXTENDEDVARS_API TArray<double> Sort_Double(TArray<double> TargetArray, bool bIsDescending);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Sort Times", ToolTip = "Description.", Keywords = "sort, times, ascending, descending"), Category = "Extended Variables|Sorters")
	static EXTENDEDVARS_API TArray<FDateTime> Sort_Time(TArray<FDateTime> TargetArray, bool bIsDescending);

	// Read Group.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Android Path Helper", ToolTip = "This node automatically gives Internal Storage absolute path. So, you just need to define sub-folder and file. Example: Download/sample.pdf", Keywords = "android, ios, mobile, folder, file, path, helper, absolute"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API FString Android_Path_Helper(FString In_FileName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Folder Contents", ToolTip = "Description.", Keywords = "explorer, load, file, folder, content"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API bool Get_Folder_Contents(TArray<FFolderContent>& OutContents, FString& ErrorCode, FString InPath);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Search In Folder", ToolTip = "Description.", Keywords = "explorer, load, file, folder, content"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API void Search_In_Folder(FDelegateSearch DelegateSearch, FString InPath, FString InSearch, bool bSearchExact);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read File from Path (64 Bit)", ToolTip = "You need to use absolute path.", Keywords = "read, load, path, bytes, import, file"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API bool Read_File_From_Path_64(UBytesObject_64*& Out_Bytes_Object, FString In_Path, bool bUseLowLevel = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read File from Path (32 Bit)", ToolTip = "You need to use absolute path.", Keywords = "read, load, path, bytes, import, file"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API bool Read_File_From_Path_32(TArray<uint8>& Out_Bytes, FString In_Path, bool bUseLowLevel = false);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read Text from Path", ToolTip = "You need to use absolute path.", Keywords = "read, load, path, bytes, import, text"), Category = "Extended Variables|Read")
	static EXTENDEDVARS_API bool Read_Text_From_Path(FString& Out_String, FString In_Path);

	// Bytes Group | Convert To.

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Bytes To Bytes64 Object", ToolTip = "", Keywords = "bytes, object, x64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API bool Bytes_To_Object(UBytesObject_64*& Out_Bytes_Object, TArray<uint8> In_Bytes);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes x64 To Hex", Keywords = "bytes, string, fstring, convert, to, hex"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_x64_To_Hex(UBytesObject_64* B64_Object, int32 Hex_Start, int32 Hex_End, bool bIsFull);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes x64 To Base64", Keywords = "bytes, string, fstring, convert, to, base64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_x64_To_Base64(UBytesObject_64* B64_Object, bool bUseUrl);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes x64 To UTF8 (Decoded)", Keywords = "bytes, string, fstring, convert, to, utf8"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_x64_To_UTF8(UBytesObject_64* B64_Object);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes x86 To Hex", Keywords = "bytes, string, fstring, convert, to, hex"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_x86_To_Hex(TArray<uint8> In_Bytes, int32 Hex_Start, int32 Hex_End, bool bIsFull);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes x86 To Base64", Keywords = "bytes, string, fstring, convert, to, base64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_x86_To_Base64(TArray<uint8> In_Bytes, bool bUseUrl);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bytes x86 To UTF8 (Decoded)", Keywords = "bytes, string, fstring, convert, to, utf8"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API FString Bytes_x86_To_UTF8(TArray<uint8> In_Bytes);

	// Bytes Group | Convert From.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Hex To Bytes x64", Keywords = "bytes, string, fstring, convert, to, hex"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API UBytesObject_64* Hex_To_Bytes_x64(FString In_Hex);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "UTF8 To Bytes x64", Keywords = "bytes, string, fstring, convert, to, utf8"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API UBytesObject_64* UTF8_To_Bytes_x64(FString In_UTF8);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Base64 To Bytes x64", Keywords = "bytes, string, fstring, convert, to, base64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API UBytesObject_64* Base64_To_Bytes_x64(FString In_Base64, bool bUseUrl);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Hex To Bytes x86", Keywords = "bytes, string, fstring, convert, to, hex"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API TArray<uint8> Hex_To_Bytes_x86(FString In_Hex);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "UTF8 To Bytes x86", Keywords = "bytes, string, fstring, convert, to, utf8"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API TArray<uint8> UTF8_To_Bytes_x86(FString In_UTF8);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Base64 To Bytes x86", Keywords = "bytes, string, fstring, convert, to, base64"), Category = "Extended Variables|Bytes")
	static EXTENDEDVARS_API TArray<uint8> Base64_To_Bytes_x86(FString In_Base, bool bUseUrl);

	// String Group.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FDateTime To String", ToolTip = "Description.", Keywords = "string, fstring, parse, date, time, convert, to"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API FString FDateTime_To_String(FDateTime In_Time);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int64 To String", Keywords = "string, fstring, convert, int64"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API FString Int64_To_String(int64 TargetInt64);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Int32", ToolTip = "It converts string to int32 with Atoi.", Keywords = "string, fstring, convert, to, int, int32, int64"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool String_To_Int32(int32& Out_Int32, FString SourceString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Int64", ToolTip = "It converts string to int64 with Atoi64.", Keywords = "string, fstring, convert, to, int, int32, int64"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool String_To_Int64(int64& Out_Int64, FString SourceString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Double", ToolTip = "It converts string to double with Atod.", Keywords = "string, fstring, convert, to, float, double"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool String_To_Double(double& Out_Double, FString SourceString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Float", ToolTip = "It converts string to float with Atof.", Keywords = "string, fstring, convert, to, float, double"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool String_To_Float(float& Out_Float, FString SourceString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Hex", ToolTip = "", Keywords = "string, fstring, convert, to, hex"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool String_To_Hex(FString& Out_Hex, FString SourceString, bool bIsLower);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Base64", ToolTip = "", Keywords = "string, fstring, convert, to, base64"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool String_To_Base64(FString& Out_Base64, FString SourceString, bool bUseUrl);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Hex To String", ToolTip = "", Keywords = "string, fstring, convert, to, hex"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool Hex_To_String(FString& Out_Decoded, FString In_Hex);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Base64 To String", ToolTip = "", Keywords = "string, fstring, convert, to, base64"), Category = "Extended Variables|String")
	static EXTENDEDVARS_API bool Base64_To_String(FString& Out_Decoded, FString In_Hex, bool bUseUrl);

	// Integer Group.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Number To Byte", ToolTip = "It converts number to a byte form.", Keywords = "byte, string, fstring, convert"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API uint8 Int32_To_Byte(int32 In_Number);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Int32 Place Family", Keywords = "int32, get, place, family"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API int32 Int32_Place_Family(int32 TargetInteger);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 Truncate To Whole (Big)", Keywords = "int32, truncate, truncation, full, whole, big"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API int32 Int32_Truncate_To_Big(int32 TargetInteger);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 Truncate To Whole (Small)", Keywords = "int32, truncate, truncation, full, whole, small"), Category = "Extended Variables|Integer")
	static EXTENDEDVARS_API int32 Int32_Truncate_To_Small(int32 TargetInteger);

	// Float/Double Group.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Float Precision", ToolTip = "Description.", Keywords = "float, get, precision"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API float Float_Precision(float TargetFloat, int32 Precision);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Fraction Count", ToolTip = "Description.", Keywords = "float, get, fraction, count"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API int32 Float_Fraction_Count(float TargetFloat);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Round Next Float", ToolTip = "Description.", Keywords = "float, round, next"), Category = "Extended Variables|Float")
	static EXTENDEDVARS_API float Float_Round_Next(float TargetFloat, double Decimal);
	
	// Time Group.

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Time Counter To FDateTime", ToolTip = "Description.", Keywords = "string, fstring, parse, date, time, convert, to"), Category = "Extended Variables|Time")
	static EXTENDEDVARS_API bool Time_Counter_To_FDateTime(FDateTime& Out_Time, FString In_Time, FString Delimiter, EStringToDate ConvertType);

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
	static EXTENDEDVARS_API bool Export_T2D_Bytes(TArray<uint8>& Out_Bytes, UTexture2D* Texture, bool bWithoutExtension);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Import Texture2D From Bytes", Keywords = "t2d, texture2d, utexture2d, import, create, bytes"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Import_T2D_Bytes(UTexture2D*& Out_Texture, TArray<uint8> In_Bytes, bool bUseSrgb);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Import Texture2D From Bytes Low Level", Keywords = "t2d, texture2d, utexture2d, import, create, bytes, low, level"), Category = "Extended Variables|Render")
	static EXTENDEDVARS_API bool Import_T2D_Bytes_LL(UTexture2D*& Out_Texture, TArray<uint8> In_Bytes, FVector2D Size, bool bUseSrgb);

};