// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExtendedVarsBPLibrary.h"
#include "ExtendedVars.h"

// UE Includes.
#include "Algo/Reverse.h"                       // Reverse array

#include "Misc/Base64.h"                        // Encode & Decode Base64
#include "Misc/FileHelper.h"                    // Load File to Array

#include "HAL/FileManager.h"                    // Save texture as bitmap and select file from dialog.
#include "HAL/FileManagerGeneric.h"

#include "ImageUtils.h"                         // Save Texture as Jpeg
#include "Slate/WidgetRenderer.h"               // Widget to Texture2D
#include "Runtime/UMG/Public/UMG.h"             // Widget to Texture2D
#include "Kismet/KismetRenderingLibrary.h"	    // Texture2D

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Containers/UnrealString.h"            // Hex to Bytes + Bytes to Hex

THIRD_PARTY_INCLUDES_START
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <numbers>                              // C++20 math library.
#include <cmath>
THIRD_PARTY_INCLUDES_END

UExtendedVarsBPLibrary::UExtendedVarsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

// Fonts Group.

URuntimeFont* UExtendedVarsBPLibrary::Runtime_Font_Load(TArray<uint8> In_Bytes)
{
    UFontFace* FontFace = NewObject<UFontFace>();
    FontFace->LoadingPolicy = EFontLoadingPolicy::Inline;
    FontFace->FontFaceData = FFontFaceData::MakeFontFaceData(CopyTemp(In_Bytes));
    FontFace->AddToRoot();

    UFont* Font = NewObject<UFont>();
    Font->FontCacheType = EFontCacheType::Runtime;
    FTypefaceEntry& TypefaceEntry = Font->CompositeFont.DefaultTypeface.Fonts[Font->CompositeFont.DefaultTypeface.Fonts.AddDefaulted()];
    TypefaceEntry.Font = FFontData(FontFace);
    Font->AddToRoot();

    URuntimeFont* RuntimeFont = NewObject<URuntimeFont>();
    RuntimeFont->Font_Face = FontFace;
    RuntimeFont->Font = Font;

    return RuntimeFont;
}

bool UExtendedVarsBPLibrary::Runtime_Font_Release(UPARAM(ref)URuntimeFont*& In_RuntimeFont)
{
    if (IsValid(In_RuntimeFont) == false)
    {
        return false;
    }

    In_RuntimeFont->Font_Face->RemoveFromRoot();
    In_RuntimeFont->Font_Face->ConditionalBeginDestroy();

    In_RuntimeFont->Font->RemoveFromRoot();
    In_RuntimeFont->Font->ConditionalBeginDestroy();

    In_RuntimeFont = nullptr;

    return true;
}

// Sorters.

TArray<FString> UExtendedVarsBPLibrary::Sort_String(TArray<FString> TargetArray, bool bIsDescending)
{
    TArray<FString> SortedArray = TargetArray;

    if (bIsDescending == true)
    {
        SortedArray.Sort([](const FString Value1, const FString Value2)
            {
                return Value1 < Value2;
            });
    }

    else
    {
        SortedArray.Sort([](const FString Value1, const FString Value2)
            {
                return Value1 > Value2;
            });
    }

    return SortedArray;
}

TArray<int32> UExtendedVarsBPLibrary::Sort_Int32(TArray<int32> TargetArray, bool bIsDescending)
{
    TArray<int32> SortedArray = TargetArray;

    if (bIsDescending == true)
    {
        SortedArray.Sort([](const int32 Value1, const int32 Value2)
            {
                return Value1 < Value2;
            });
    }

    else
    {
        SortedArray.Sort([](const int32 Value1, const int32 Value2)
            {
                return Value1 > Value2;
            });
    }

    return SortedArray;
}

TArray<float> UExtendedVarsBPLibrary::Sort_Float(TArray<float> TargetArray, bool bIsDescending)
{
    TArray<float> SortedArray = TargetArray;

    if (bIsDescending == true)
    {
        SortedArray.Sort([](const float Value1, const float Value2)
            {
                return Value1 < Value2;
            });
    }

    else
    {
        SortedArray.Sort([](const float Value1, const float Value2)
            {
                return Value1 > Value2;
            });
    }

    return SortedArray;
}

TArray<double> UExtendedVarsBPLibrary::Sort_Double(TArray<double> TargetArray, bool bIsDescending)
{
    TArray<double> SortedArray = TargetArray;

    if (bIsDescending == true)
    {
        SortedArray.Sort([](const float Value1, const float Value2)
            {
                return Value1 < Value2;
            });
    }

    else
    {
        SortedArray.Sort([](const float Value1, const float Value2)
            {
                return Value1 > Value2;
            });
    }

    return SortedArray;
}

TArray<FDateTime> UExtendedVarsBPLibrary::Sort_Time(TArray<FDateTime> TargetArray, bool bIsDescending)
{
    TArray<FDateTime> SortedArray = TargetArray;

    if (bIsDescending == true)
    {
        SortedArray.Sort([](const FDateTime Value1, const FDateTime Value2)
            {
                return Value1 < Value2;
            });
    }

    else
    {
        SortedArray.Sort([](const FDateTime Value1, const FDateTime Value2)
            {
                return Value1 > Value2;
            });
    }

    return SortedArray;
}

// Read Group.

FString UExtendedVarsBPLibrary::Android_Path_Helper(FString In_FileName)
{
    if (UGameplayStatics::GetPlatformName() == "Android" || UGameplayStatics::GetPlatformName() == "IOS")
    {
        if (In_FileName.IsEmpty() == true)
        {
            return "";
        }

        FString Path_Referance = FPlatformFileManager::Get().GetPlatformFile().ConvertToAbsolutePathForExternalAppForRead(*(FPaths::ProjectSavedDir()));

        TArray<FString> Path_Sections;
        Path_Referance.ParseIntoArray(Path_Sections, TEXT("/"), true);
        FString Path_Absolute = "/" + Path_Sections[0] + "/" + Path_Sections[1] + "/" + Path_Sections[2] + "/" + In_FileName;

        return Path_Absolute;
    }

    else
    {
        return "";
    }
}

bool UExtendedVarsBPLibrary::Get_Folder_Contents(TArray<FFolderContent>& OutContents, FString& ErrorCode, FString InPath)
{
    if (InPath.IsEmpty() == true)
    {
        ErrorCode = "Path is empty.";
        return false;
    }

    if (FPaths::DirectoryExists(InPath) == false)
    {
        ErrorCode = "Directory doesn't exist.";
        return false;
    }

    class FFindDirectories : public IPlatformFile::FDirectoryVisitor
    {
    public:

        TArray<FFolderContent> Array_Contents;

        FFindDirectories() {}
        virtual bool Visit(const TCHAR* CharPath, bool bIsDirectory) override
        {
            if (bIsDirectory == true)
            {
                FFolderContent EachContent;

                FString Path = FString(CharPath) + "/";
                FPaths::NormalizeDirectoryName(Path);

                EachContent.Path = Path;
                EachContent.Name = FPaths::GetBaseFilename(Path);
                EachContent.bIsFile = false;

                Array_Contents.Add(EachContent);
            }

            else if (bIsDirectory == false)
            {
                FFolderContent EachContent;

                EachContent.Path = CharPath;
                EachContent.Name = FPaths::GetCleanFilename(CharPath);
                EachContent.bIsFile = true;

                Array_Contents.Add(EachContent);
            }

            return true;
        }
    };

    FFindDirectories GetFoldersVisitor;
    FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*InPath, GetFoldersVisitor);

    OutContents = GetFoldersVisitor.Array_Contents;

    return true;
}

void UExtendedVarsBPLibrary::Search_In_Folder(FDelegateSearch DelegateSearch, FString InPath, FString InSearch, bool bSearchExact)
{
    if (InPath.IsEmpty() == true)
    {
        return;
    }

    if (InSearch.IsEmpty() == true)
    {
        return;
    }

    if (FPaths::DirectoryExists(InPath) == false)
    {
        return;
    }

    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [DelegateSearch, InPath, InSearch, bSearchExact]()
        {
            IFileManager& FileManager = FFileManagerGeneric::Get();

            TArray<FString> Array_Contents;
            TArray<FFolderContent> Array_Founds;

            FileManager.FindFilesRecursive(Array_Contents, *InPath, TEXT("*"), true, true, false);

            for (int32 ContentIndex = 0; ContentIndex < Array_Contents.Num(); ContentIndex++)
            {
                FFolderContent EachContent;

                if (bSearchExact == true)
                {
                    if (FPaths::GetBaseFilename(Array_Contents[ContentIndex]) == InSearch)
                    {
                        EachContent.Name = FPaths::GetCleanFilename(Array_Contents[ContentIndex]);
                        EachContent.Path = Array_Contents[ContentIndex];
                        EachContent.bIsFile = FPaths::FileExists(Array_Contents[ContentIndex]);

                        Array_Founds.Add(EachContent);
                    }
                }

                else
                {
                    if (FPaths::GetBaseFilename(Array_Contents[ContentIndex]).Contains(InSearch) == true)
                    {
                        EachContent.Name = FPaths::GetCleanFilename(Array_Contents[ContentIndex]);
                        EachContent.Path = Array_Contents[ContentIndex];
                        EachContent.bIsFile = FPaths::FileExists(Array_Contents[ContentIndex]);

                        Array_Founds.Add(EachContent);
                    }
                }
            }

            AsyncTask(ENamedThreads::GameThread, [DelegateSearch, Array_Founds]()
                {
                    FFolderContentArray ArrayContainer;
                    ArrayContainer.OutContents = Array_Founds;

                    DelegateSearch.ExecuteIfBound(true, "Success", ArrayContainer);
                }
            );
        }
    );
}

bool UExtendedVarsBPLibrary::Read_File_From_Path_64(UBytesObject_64*& Out_Bytes_Object, FString In_Path, bool bUseLowLevel)
{
    if (In_Path.IsEmpty())
    {
        return false;
    }
    
    FPaths::NormalizeFilename(In_Path);
    FString Path = FPlatformFileManager::Get().GetPlatformFile().ConvertToAbsolutePathForExternalAppForRead(*In_Path);

    if (!FPaths::FileExists(Path))
    {
        return false;
    }

    TArray64<uint8> ByteArray;

    if (bUseLowLevel)
    {
        FPaths::MakePlatformFilename(Path);

        _Post_ _Notnull_ FILE* File = nullptr;

#ifdef _WIN64

        fopen_s(&File, TCHAR_TO_UTF8(*Path), "rb");
 
#else

        File = fopen(TCHAR_TO_UTF8(*Path), "rb");

#endif // _WIN64

        fseek(File, 0L, SEEK_END);
        long FileSize = ftell(File);
        fseek(File, 0L, SEEK_SET);

        if (FileSize == 0)
        {
            fclose(File);
            return false;
        }

        uint8* Buffer = (uint8*)malloc(FileSize);
        fread(Buffer, sizeof(uint8), FileSize, File);
        fclose(File);

        FMemory::Memcpy(ByteArray.GetData(), Buffer, FileSize);
    }

    else
    {
        FFileHelper::LoadFileToArray(ByteArray, *Path, FILEREAD_AllowWrite);

        if (ByteArray.Num() == 0)
        {
            return false;
        }
    }

    Out_Bytes_Object = NewObject<UBytesObject_64>();
    Out_Bytes_Object->ByteArray = ByteArray;

    return true;
}

bool UExtendedVarsBPLibrary::Read_File_From_Path_32(TArray<uint8>& Out_Bytes, FString In_Path, bool bUseLowLevel)
{
    if (In_Path.IsEmpty())
    {
        return false;
    }

    FPaths::NormalizeFilename(In_Path);
    FString Path = FPlatformFileManager::Get().GetPlatformFile().ConvertToAbsolutePathForExternalAppForRead(*In_Path);

    if (!FPaths::FileExists(Path))
    {
        return false;
    }

    if (bUseLowLevel)
    {
        FPaths::MakePlatformFilename(Path);

        _Post_ _Notnull_ FILE* File = nullptr;

#ifdef _WIN64

        fopen_s(&File, TCHAR_TO_UTF8(*Path), "rb");

#else

        File = fopen(TCHAR_TO_UTF8(*Path), "rb");

#endif // _WIN64

        fseek(File, 0L, SEEK_END);
        long FileSize = ftell(File);
        fseek(File, 0L, SEEK_SET);

        if (FileSize == 0)
        {
            fclose(File);
            return false;
        }

        uint8* Buffer = (uint8*)malloc(FileSize);
        fread(Buffer, sizeof(uint8), FileSize, File);
        fclose(File);

        FMemory::Memcpy(Out_Bytes.GetData(), Buffer, FileSize);
    }

    else
    {
        FFileHelper::LoadFileToArray(Out_Bytes, *Path, FILEREAD_AllowWrite);

        if (Out_Bytes.Num() == 0)
        {
            return false;
        }
    }

    return true;
}

bool UExtendedVarsBPLibrary::Read_Text_From_Path(FString& Out_String, FString In_Path)
{
    if (In_Path.IsEmpty())
    {
        return false;
    }

    FPaths::MakeStandardFilename(In_Path);
    if (!FPaths::FileExists(In_Path))
    {

        return false;
    }

    return FFileHelper::LoadFileToString(Out_String, *In_Path);
}

// Bytes Group | Convert To.

bool UExtendedVarsBPLibrary::Bytes_To_Object(UBytesObject_64*& Out_Bytes_Object, TArray<uint8> In_Bytes)
{
    if (In_Bytes.Num() == 0)
    {
        return false;
    }

    TArray64<uint8> ByteArray;
    ByteArray.SetNum(In_Bytes.Num(), true);
    FMemory::Memcpy(ByteArray.GetData(), In_Bytes.GetData(), In_Bytes.GetAllocatedSize());

    UBytesObject_64* BytesObject_64 = NewObject<UBytesObject_64>();
    BytesObject_64->ByteArray = ByteArray;

    Out_Bytes_Object = BytesObject_64;

    return true;
}

FString UExtendedVarsBPLibrary::Bytes_x64_To_Hex(UBytesObject_64* B64_Object, int32 Hex_Start, int32 Hex_End, bool bIsFull)
{
    if (B64_Object->ByteArray.Num() == 0)
    {
        return "";
    }

    if (bIsFull)
    {
        std::stringstream Stream;
        Stream << std::hex << std::setfill('0');
        for (int Index = 0; Index < B64_Object->ByteArray.Num(); Index++)
        {
            Stream << std::hex << std::setw(2) << static_cast<int>(B64_Object->ByteArray.GetData()[Index]);
        }

        return Stream.str().c_str();
    }

    else if (Hex_Start <= Hex_End && Hex_End < B64_Object->ByteArray.Num())
    {
        std::stringstream Stream;
        Stream << std::hex << std::setfill('0');
        for (int Index = Hex_Start; Index <= Hex_End; Index++)
        {
            Stream << std::hex << std::setw(2) << static_cast<int>(B64_Object->ByteArray.GetData()[Index]);
        }

        return Stream.str().c_str();
    }

    else
    {
        return "";
    }
}

FString UExtendedVarsBPLibrary::Bytes_x64_To_Base64(UBytesObject_64* B64_Object, bool bUseUrl)
{
    if (B64_Object->ByteArray.Num() == 0)
    {
        return "";
    }

    return FBase64::Encode(B64_Object->ByteArray.GetData(), B64_Object->ByteArray.Num(), bUseUrl ? EBase64Mode::UrlSafe : EBase64Mode::Standard);
}

FString UExtendedVarsBPLibrary::Bytes_x64_To_UTF8(UBytesObject_64* B64_Object)
{
    if (B64_Object->ByteArray.Num() == 0)
    {
        return "";
    }

    int32 Index = 0;
    int32 Length = 0x7FFFFFFF;

    if (Index < 0)
    {
        Length += Index;
        Index = 0;
    }

    if (Length > B64_Object->ByteArray.Num() - Index)
    {
        Length = B64_Object->ByteArray.Num() - Index;
    }

    const FUTF8ToTCHAR Src(reinterpret_cast<const ANSICHAR*>(B64_Object->ByteArray.GetData() + Index), Length);
    return FString(Src.Length(), Src.Get());
}

FString UExtendedVarsBPLibrary::Bytes_x86_To_Hex(TArray<uint8> In_Bytes, int32 Hex_Start, int32 Hex_End, bool bIsFull)
{
    if (In_Bytes.Num() == 0)
    {
        return "";
    }

    if (bIsFull)
    {
        std::stringstream Stream;
        Stream << std::hex << std::setfill('0');
        for (int Index = 0; Index < In_Bytes.Num(); Index++)
        {
            Stream << std::hex << std::setw(2) << static_cast<int>(In_Bytes.GetData()[Index]);
        }

        return Stream.str().c_str();

        /**
        * UE's implementation.
        * return BytesToHex(In_Bytes.GetData(), In_Bytes.Num());
        */
    }

    else if (Hex_Start <= Hex_End && Hex_End < In_Bytes.Num())
    {
        std::stringstream Stream;
        Stream << std::hex << std::setfill('0');
        for (int Index = Hex_Start; Index <= Hex_End; Index++)
        {
            Stream << std::hex << std::setw(2) << static_cast<int>(In_Bytes.GetData()[Index]);
        }

        return Stream.str().c_str();
    }

    else
    {
        return "";
    }
}

FString UExtendedVarsBPLibrary::Bytes_x86_To_Base64(TArray<uint8> In_Bytes, bool bUseUrl)
{
    if (In_Bytes.Num() == 0)
    {
        return "";
    }

    return FBase64::Encode(In_Bytes.GetData(), In_Bytes.GetAllocatedSize(), bUseUrl ? EBase64Mode::UrlSafe : EBase64Mode::Standard);
}

FString UExtendedVarsBPLibrary::Bytes_x86_To_UTF8(TArray<uint8> In_Bytes)
{
    if (In_Bytes.Num() == 0)
    {
        return "";
    }

    int32 Index = 0;
    int32 Length = 0x7FFFFFFF;

    if (Index < 0)
    {
        Length += Index;
        Index = 0;
    }

    if (Length > In_Bytes.Num() - Index)
    {
        Length = In_Bytes.Num() - Index;
    }

    const FUTF8ToTCHAR Src(reinterpret_cast<const ANSICHAR*>(In_Bytes.GetData() + Index), Length);
    return FString(Src.Length(), Src.Get());
}

// Bytes Group | Convert From.

UBytesObject_64* UExtendedVarsBPLibrary::Hex_To_Bytes_x64(FString In_Hex)
{
    if (In_Hex.IsEmpty())
    {
        return nullptr;
    }

    UBytesObject_64* BytesObject = NewObject<UBytesObject_64>();
    
    FString Decoded_String;
    for (size_t Index_Chars = 0; Index_Chars < In_Hex.Len(); Index_Chars += 2)
    {
        //taking two characters from hex string
        FString Part = UKismetStringLibrary::GetSubstring(In_Hex, Index_Chars, 2);

        //changing it into base 16
        char Character = std::stoul(TCHAR_TO_UTF8(*Part), nullptr, 16);

        //putting it into the ASCII string
        Decoded_String += Character;
    }

    FTCHARToUTF8 Source = FTCHARToUTF8(Decoded_String.GetCharArray().GetData());
    BytesObject->ByteArray.Append((uint8*)Source.Get(), Source.Length());

    return BytesObject;
}

UBytesObject_64* UExtendedVarsBPLibrary::UTF8_To_Bytes_x64(FString In_UTF8)
{
    if (In_UTF8.IsEmpty())
    {
        return nullptr;
    }

    UBytesObject_64* BytesObject = NewObject<UBytesObject_64>();

    FTCHARToUTF8 Source = FTCHARToUTF8(In_UTF8.GetCharArray().GetData());
    BytesObject->ByteArray.Append((uint8*)Source.Get(), Source.Length());

    return BytesObject;
}

UBytesObject_64* UExtendedVarsBPLibrary::Base64_To_Bytes_x64(FString In_Base64, bool bUseUrl)
{
    if (In_Base64.IsEmpty())
    {
        return nullptr;
    }

    FWideStringView String = In_Base64;

    UBytesObject_64* BytesObject = NewObject<UBytesObject_64>();

    FBase64::Decode(String.GetData(), String.Len(), BytesObject->ByteArray.GetData(), bUseUrl ? EBase64Mode::UrlSafe : EBase64Mode::Standard);

    return BytesObject;
}

TArray<uint8> UExtendedVarsBPLibrary::Hex_To_Bytes_x86(FString In_Hex)
{
    TArray<uint8> Array_Bytes;

    if (In_Hex.IsEmpty())
    {
        return Array_Bytes;
    }

    Array_Bytes.AddUninitialized(In_Hex.Len() / 2);
    HexToBytes(In_Hex, Array_Bytes.GetData());

    return Array_Bytes;
}

TArray<uint8> UExtendedVarsBPLibrary::UTF8_To_Bytes_x86(FString In_UTF8)
{
    TArray<uint8> Array_Bytes;

    if (In_UTF8.IsEmpty())
    {
        return Array_Bytes;
    }

    FTCHARToUTF8 Source = FTCHARToUTF8(In_UTF8.GetCharArray().GetData());
    Array_Bytes.Append((uint8*)Source.Get(), Source.Length());

    return Array_Bytes;
}

TArray<uint8> UExtendedVarsBPLibrary::Base64_To_Bytes_x86(FString In_Base64, bool bUseUrl)
{
    TArray<uint8> Array_Bytes;

    if (In_Base64.IsEmpty())
    {
        return Array_Bytes;
    }

    FBase64::Decode(In_Base64, Array_Bytes, bUseUrl ? EBase64Mode::UrlSafe : EBase64Mode::Standard);

    return Array_Bytes;
}

// String Group.

FString UExtendedVarsBPLibrary::FDateTime_To_String(FDateTime In_Time)
{
    return In_Time.ToString();
}

FString UExtendedVarsBPLibrary::Int64_To_String(int64 TargetInt64)
{
    return FString::Printf(TEXT("%llu"), TargetInt64);
}

bool UExtendedVarsBPLibrary::String_To_Int32(int32& Out_Int32, FString SourceString)
{
    if (SourceString.IsEmpty())
    {
        return false;
    }
    
    Out_Int32 = FCString::Atoi(*SourceString);

    return true;
}

bool UExtendedVarsBPLibrary::String_To_Int64(int64& Out_Int64, FString SourceString)
{
    if (SourceString.IsEmpty())
    {
        return false;
    }

    Out_Int64 = FCString::Atoi64(*SourceString);

    return true;
}

bool UExtendedVarsBPLibrary::String_To_Double(double& Out_Double, FString SourceString)
{
    if (SourceString.IsEmpty())
    {
        return false;
    }

    Out_Double = FCString::Atod(*SourceString);

    return true;
}

bool UExtendedVarsBPLibrary::String_To_Float(float& Out_Float, FString SourceString)
{
    if (SourceString.IsEmpty())
    {
        return false;
    }

    Out_Float = FCString::Atof(*SourceString);

    return true;
}

bool UExtendedVarsBPLibrary::String_To_Hex(FString& Out_Hex, FString SourceString, bool bIsLower)
{
    if (SourceString.IsEmpty())
    {
        return false;
    }

    FTCHARToUTF8 Source = FTCHARToUTF8(SourceString.GetCharArray().GetData());
   
    TArray<uint8> Temp_Buffer;
    Temp_Buffer.Append((uint8*)Source.Get(), Source.Length());
    
    Out_Hex = bIsLower ? BytesToHexLower(Temp_Buffer.GetData(), Temp_Buffer.Num()) : BytesToHex(Temp_Buffer.GetData(), Temp_Buffer.Num());

    return true;
}

bool UExtendedVarsBPLibrary::String_To_Base64(FString& Out_Base64, FString SourceString, bool bUseUrl)
{
    if (SourceString.IsEmpty())
    {
        return false;
    }

    TArray<uint8> Temp_Buffer;
    FTCHARToUTF8 Source = FTCHARToUTF8(SourceString.GetCharArray().GetData());
    Temp_Buffer.Append((uint8*)Source.Get(), Source.Length());

    Out_Base64 = FBase64::Encode(Temp_Buffer, bUseUrl ? EBase64Mode::UrlSafe : EBase64Mode::Standard);

    return true;
}

bool UExtendedVarsBPLibrary::Hex_To_String(FString& Out_Decoded, FString In_Hex)
{
    if (In_Hex.IsEmpty())
    {
        return false;
    }

    TArray<uint8> Temp_Buffer;
    Temp_Buffer.AddUninitialized(In_Hex.Len() / 2);
    HexToBytes(In_Hex, Temp_Buffer.GetData());

    int32 Index = 0;
    int32 Length = 0x7FFFFFFF;

    if (Index < 0)
    {
        Length += Index;
        Index = 0;
    }

    if (Length > Temp_Buffer.Num() - Index)
    {
        Length = Temp_Buffer.Num() - Index;
    }

    const FUTF8ToTCHAR Src(reinterpret_cast<const ANSICHAR*>(Temp_Buffer.GetData() + Index), Length);
    Out_Decoded = FString(Src.Length(), Src.Get());

    return true;
}

bool UExtendedVarsBPLibrary::Base64_To_String(FString& Out_Decoded, FString In_Base64, bool bUseUrl)
{
    if (In_Base64.IsEmpty())
    {
        return false;
    }

    TArray<uint8> Temp_Array;
    FBase64::Decode(In_Base64, Temp_Array, bUseUrl ? EBase64Mode::UrlSafe : EBase64Mode::Standard);

    int32 Index = 0;
    int32 Length = 0x7FFFFFFF;

    if (Index < 0)
    {
        Length += Index;
        Index = 0;
    }

    if (Length > Temp_Array.Num() - Index)
    {
        Length = Temp_Array.Num() - Index;
    }

    const FUTF8ToTCHAR Src(reinterpret_cast<const ANSICHAR*>(Temp_Array.GetData() + Index), Length);
    Out_Decoded = FString(Src.Length(), Src.Get());

    return true;
}

// Integer Group.

uint8 UExtendedVarsBPLibrary::Int32_To_Byte(int32 In_Number)
{
    if (In_Number >= 0 && In_Number <= 255)
    {
        return In_Number;
    }

    else if (In_Number < 0)
    {
        return 0;
    }

    else if (In_Number > 255)
    {
        return 255;
    }

    return 0;
}

int32 UExtendedVarsBPLibrary::Int32_Place_Family(int32 TargetInteger)
{
    /*
    10 power integer character lenght minus 1 will equal its place family.
    For example to find 10's place family, 10^2(Char Lenght) - 1 = 10. 10's place family will 10.
    */
    
    return (int32)FMath::Pow(10.0, (double)((FString::FromInt(TargetInteger).Len()) - 1));
}

int32 UExtendedVarsBPLibrary::Int32_Truncate_To_Big(int32 TargetInteger)
{
    double Remainder;
    UKismetMathLibrary::FMod(TargetInteger, UExtendedVarsBPLibrary::Int32_Place_Family(TargetInteger), Remainder);

    if (Remainder != 0)
    {
        const FString IntString = FString::FromInt(TargetInteger);

        int32 PlaceDigit = FCString::Atoi(*(UKismetStringLibrary::GetCharacterArrayFromString(IntString)[0]));
        float PlaceFamily = (int32)FMath::Pow(10.0, (double)((IntString.Len()) - 1));

        return PlaceFamily * (PlaceDigit + 1);
    }

    else
    {
        return TargetInteger;
    }
}

int32 UExtendedVarsBPLibrary::Int32_Truncate_To_Small(int32 TargetInteger)
{
    double Remainder;
    UKismetMathLibrary::FMod(TargetInteger, 10, Remainder);
    
    if (Remainder != 0)
    {
        const FString IntString = FString::FromInt(TargetInteger);

        int32 LastDigit = FCString::Atoi(*(UKismetStringLibrary::GetCharacterArrayFromString(IntString).Last()));
        int32 Difference = 10 - LastDigit;

        return TargetInteger + Difference;
    }

    else
    {
        return TargetInteger;
    }
}

// Float/Double Group.

float UExtendedVarsBPLibrary::Float_Precision(float TargetFloat, int32 Precision)
{
    int32 PrecisionFixer = pow(10, Precision);
    return floor(PrecisionFixer * TargetFloat) / PrecisionFixer;
}

int32 UExtendedVarsBPLibrary::Float_Fraction_Count(float TargetFloat)
{
    FString FractionString;
    (FString::SanitizeFloat(UKismetMathLibrary::Fraction(TargetFloat))).Split(TEXT("."), NULL, &FractionString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
   
    return FractionString.Len();
}

float UExtendedVarsBPLibrary::Float_Round_Next(float TargetFloat, double Decimal)
{
    int32 FractionCount = UExtendedVarsBPLibrary::Float_Fraction_Count(TargetFloat);
    
    if (FractionCount > Decimal)
    {
        return (FMath::TruncToInt(TargetFloat * FMath::Pow(10.0, Decimal)) + 1) / FMath::Pow(10.0, Decimal);
    }

    else
    {
        return TargetFloat;
    }
}

// Time Group.

bool UExtendedVarsBPLibrary::Time_Counter_To_FDateTime(FDateTime& Out_Time, FString In_Time, FString Delimiter, EStringToDate ConvertType)
{
    if (In_Time.IsEmpty())
    {
        return false;
    }

    TArray<FString> TimeSections;

    switch (ConvertType)
    {
    case EStringToDate::None:

        return false;

    case EStringToDate::UnrealDateTime:

        return FDateTime::Parse(In_Time, Out_Time);

    case EStringToDate::Http:

        return FDateTime::ParseHttpDate(In_Time, Out_Time);

    case EStringToDate::Iso8601:

        return FDateTime::ParseIso8601(*In_Time, Out_Time);

    case EStringToDate::Custom:

        TimeSections = UKismetStringLibrary::ParseIntoArray(In_Time, Delimiter, false);

        if (TimeSections.Num() == 3)
        {
            Out_Time = FDateTime(1, 1, 1, FCString::Atoi(*TimeSections[0]), FCString::Atoi(*TimeSections[1]), FCString::Atoi(*TimeSections[2]), 0);
            return true;
        }
        
        else
        {
            return false;
        }

    default:
        return false;
    }
}

// Render Group.

UTextureRenderTarget2D* UExtendedVarsBPLibrary::Widget_To_TRT2D(FString& OutCode, UUserWidget* InWidget, FVector2D InDrawSize)
{
    if (IsValid(InWidget) == false)
    {
        OutCode = "Source widget is not valid";
        return nullptr;
    }

    UTextureRenderTarget2D* TextureTarget = FWidgetRenderer::CreateTargetFor(InDrawSize, TextureFilter::TF_Default, false);
    TextureTarget->RenderTargetFormat = RTF_RGBA8;

    FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(true);
    WidgetRenderer->DrawWidget(TextureTarget, InWidget->TakeWidget(), InDrawSize, 0, false);

    if (IsValid(TextureTarget) == true)
    {
        return TextureTarget;
    }

    else
    {
        OutCode = "Unable to create Texture Render Target 2D";
        return nullptr;
    }
}

UTexture2D* UExtendedVarsBPLibrary::TRT2D_To_T2D(UTextureRenderTarget2D* In_TRT_2D)
{
    if (IsValid(In_TRT_2D) == false)
    {
        return nullptr;
    }

    TArray64<uint8> Buffer;
    FImageUtils::GetRawData(In_TRT_2D, Buffer);
    SIZE_T BufferSize = static_cast<size_t>(Buffer.GetAllocatedSize());

    UTexture2D* Texture = UTexture2D::CreateTransient(In_TRT_2D->SizeX, In_TRT_2D->SizeY, PF_B8G8R8A8);
    FTexture2DMipMap& Texture_Mip = Texture->GetPlatformData()->Mips[0];
    void* Texture_Data = Texture_Mip.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(Texture_Data, Buffer.GetData(), BufferSize);

    Texture_Mip.BulkData.Unlock();
    Texture->UpdateResource();

    if (IsValid(Texture) == true)
    {
        return Texture;
    }

    else
    {
        return nullptr;
    }
}

bool UExtendedVarsBPLibrary::Export_T2D_Colors(TArray<FColor>& Out_Array, UTexture2D* Texture)
{
    if (IsValid(Texture) == false)
    {
        return false;
    }

    if (Texture->GetPixelFormat() == EPixelFormat::PF_B8G8R8A8 && Texture->CompressionSettings.GetIntValue() == 5 || Texture->CompressionSettings.GetIntValue() == 7)
    {
        int32 Texture_Width = Texture->GetSizeX();
        int32 Texture_Height = Texture->GetSizeY();

        FTexture2DMipMap& Texture_Mip = Texture->GetPlatformData()->Mips[0];
        void* Texture_Data = Texture_Mip.BulkData.Lock(LOCK_READ_WRITE);

        TArray<FColor> Array_Colors;
        Array_Colors.SetNum(Texture_Width * Texture_Height);
        FMemory::Memcpy(Array_Colors.GetData(), static_cast<FColor*>(Texture_Data), static_cast<SIZE_T>(Array_Colors.Num()) * 4);

        Texture_Mip.BulkData.Unlock();

        if (Array_Colors.Num() > 0)
        {
            Out_Array = Array_Colors;
            return true;
        }

        else
        {
            return false;
        }
    }

    else
    {
        return false;
    }
}

bool UExtendedVarsBPLibrary::Export_T2D_Bytes(TArray<uint8>& Out_Bytes, UTexture2D* Texture)
{
    if (IsValid(Texture) == false)
    {
        return false;
    }

    if (Texture->GetPixelFormat() == EPixelFormat::PF_B8G8R8A8 && Texture->CompressionSettings.GetIntValue() == 5 || Texture->CompressionSettings.GetIntValue() == 7)
    {
        FTexture2DMipMap& Texture_Mip = Texture->GetPlatformData()->Mips[0];
        void* Texture_Data = Texture_Mip.BulkData.Lock(LOCK_READ_WRITE);

        TArray<uint8> Array_Bytes;
        size_t Lenght = static_cast<size_t>(Texture->GetSizeX() * Texture->GetSizeY() * 4);
        Array_Bytes.SetNum(Lenght);
        FMemory::Memcpy(Array_Bytes.GetData(), Texture_Data, Lenght);

        Texture_Mip.BulkData.Unlock();

        if (Array_Bytes.Num() > 0)
        {
            Out_Bytes = Array_Bytes;
            return true;
        }

        else
        {
            return false;
        }
    }

    else
    {
        return false;
    }
}

bool UExtendedVarsBPLibrary::Export_T2D_Bitmap(FString& Out_Path, UTexture2D* Texture, FString In_Path, bool bUseTemp)
{
    if (IsValid(Texture) == false)
    {
        return false;
    }

    if (bUseTemp == false && In_Path.IsEmpty() == true)
    {
        return false;
    }

    FString FileName;

    if (bUseTemp == true)
    {
        FileName = FPaths::ProjectSavedDir() + "Temp/" + Texture->GetFName().ToString() + ".bmp";
    }

    else
    {
        FPaths::MakeStandardFilename(In_Path);
        FileName = In_Path;
    }

    TArray<FColor> Array_Colors;
    if (UExtendedVarsBPLibrary::Export_T2D_Colors(Array_Colors, Texture))
    {
        bool IsBitmapCreated = FFileHelper::CreateBitmap(*FileName, Texture->GetSizeX(), Texture->GetSizeY(), Array_Colors.GetData(), NULL, &IFileManager::Get(), NULL, true);
        Out_Path = FileName;

        return IsBitmapCreated;
    }

    else
    {
        return false;
    }
}

bool UExtendedVarsBPLibrary::Export_T2D_Jpeg(FString& Out_Path, UTexture2D* Texture, FString In_Path, bool bUseTemp)
{
    if (IsValid(Texture) == false)
    {
        return false;
    }

    if (bUseTemp == false && In_Path.IsEmpty() == true)
    {
        return false;
    }

    FString FileName;

    if (bUseTemp == true)
    {
        FileName = FPaths::ProjectSavedDir() + "Temp/" + Texture->GetFName().ToString() + ".jpg";
    }

    else
    {
        FPaths::MakeStandardFilename(In_Path);
        FileName = In_Path;
    }

    FImage JpegExporter;
    FImageUtils::GetTexture2DSourceImage(Texture, JpegExporter);
    bool bIsJpegCreated = FImageUtils::SaveImageByExtension(*FileName, JpegExporter, 0);

    if (bIsJpegCreated == false)
    {
        return false;
    }

    Out_Path = FileName;

    return true;
}

bool UExtendedVarsBPLibrary::Import_T2D_Bytes(UTexture2D*& Out_Texture, TArray<uint8> In_Bytes, bool bUseSrgb)
{
    if (In_Bytes.Num() == 0)
    {
        return false;
    }

    UTexture2D* Texture = FImageUtils::ImportBufferAsTexture2D(In_Bytes);
    Texture->SRGB = bUseSrgb;

    if (IsValid(Texture) == true)
    {
        Out_Texture = Texture;
        return true;
    }

    else
    {
        return false;
    }
}

bool UExtendedVarsBPLibrary::Import_T2D_Bytes_LL(UTexture2D*& Out_Texture, TArray<uint8> In_Bytes, FVector2D Size, bool bUseSrgb)
{
    if (In_Bytes.Num() == 0)
    {
        return false;
    }

    UTexture2D* Texture = UTexture2D::CreateTransient(Size.X, Size.Y, PF_B8G8R8A8);
    Texture->SRGB = bUseSrgb;

    FTexture2DMipMap& Texture_Mip = Texture->GetPlatformData()->Mips[0];
    void* Texture_Data = Texture_Mip.BulkData.Lock(LOCK_READ_WRITE);

    FMemory::Memcpy(Texture_Data, In_Bytes.GetData(), In_Bytes.Num());
    Texture_Mip.BulkData.Unlock();
    Texture->UpdateResource();

    Out_Texture = Texture;

    return true;
}