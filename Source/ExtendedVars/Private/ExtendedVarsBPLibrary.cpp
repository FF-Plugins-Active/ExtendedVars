// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExtendedVarsBPLibrary.h"
#include "ExtendedVars.h"

// UE Includes.
#include "Slate/WidgetRenderer.h"               // Widget to Texture2D
#include "Runtime/UMG/Public/UMG.h"             // Widget to Texture2D
#include "Kismet/KismetRenderingLibrary.h"	    // Texture2D
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/Base64.h"                        // Encode / Decode Base64 
#include "Misc/FileHelper.h"                    // Load File to Array
#include "HAL/FileManager.h"                    // Save Texture as Bitmap
#include "ImageUtils.h"                         // Save Texture as Jpeg

THIRD_PARTY_INCLUDES_START
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
THIRD_PARTY_INCLUDES_END

UExtendedVarsBPLibrary::UExtendedVarsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

// Path Group.

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

bool UExtendedVarsBPLibrary::Read_File_From_Path_x64(UBytesObject_64*& Out_Bytes_Object, FString In_Path, bool bUseLowLevel)
{
    if (In_Path.IsEmpty())
    {
        return false;
    }
    
    FPaths::NormalizeFilename(In_Path);
    if (!FPaths::FileExists(In_Path))
    {
        return false;
    }

    TArray64<uint8> ByteArray;

    if (bUseLowLevel)
    {
        FPaths::MakePlatformFilename(In_Path);

        FILE* File = nullptr;

#ifdef _WIN64

        fopen_s(&File, TCHAR_TO_UTF8(*In_Path), "rb");
 
#else

        File = fopen(TCHAR_TO_UTF8(*In_Path), "rb");

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
        FFileHelper::LoadFileToArray(ByteArray, *In_Path);

        if (ByteArray.Num() == 0)
        {
            return false;
        }
    }

    Out_Bytes_Object = NewObject<UBytesObject_64>();
    Out_Bytes_Object->ByteArray = ByteArray;

    return true;
}

bool UExtendedVarsBPLibrary::Read_File_From_Path_x32(UBytesObject_32*& Out_Bytes_Object, FString In_Path, bool bUseLowLevel)
{
    if (In_Path.IsEmpty())
    {
        return false;
    }

    FPaths::NormalizeFilename(In_Path);
    if (!FPaths::FileExists(In_Path))
    {
        return false;
    }

    TArray<uint8> ByteArray;

    if (bUseLowLevel)
    {
        FPaths::MakePlatformFilename(In_Path);

        FILE* File = nullptr;

#ifdef _WIN64

        fopen_s(&File, TCHAR_TO_UTF8(*In_Path), "rb");

#else

        File = fopen(TCHAR_TO_UTF8(*In_Path), "rb");

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
        FFileHelper::LoadFileToArray(ByteArray, *In_Path);

        if (ByteArray.Num() == 0)
        {
            return false;
        }
    }

    Out_Bytes_Object = NewObject<UBytesObject_32>();
    Out_Bytes_Object->ByteArray = ByteArray;

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

// Convert To Bytes.

TArray<uint8> UExtendedVarsBPLibrary::String_To_Bytes(FString In_String)
{
    TArray<uint8> Array_Bytes;

    if (In_String.IsEmpty() == true)
    {
        return Array_Bytes;
    }

    FTCHARToUTF8 Source = FTCHARToUTF8(In_String.GetCharArray().GetData());
    Array_Bytes.Append((uint8*)Source.Get(), Source.Length());

    return Array_Bytes;
}

TArray<uint8> UExtendedVarsBPLibrary::Base64_To_Bytes(FString In_Base64, bool bUseUrl)
{
    TArray<uint8> Array_Bytes;

    if (In_Base64.IsEmpty() == true)
    {
        return Array_Bytes;
    }

    FString Base64_String = In_Base64;

    if (bUseUrl == true)
    {
        Base64_String.ReplaceInline(TEXT("-"), TEXT("+"));
        Base64_String.ReplaceInline(TEXT("_"), TEXT("/"));
    }

    TArray<uint8> Bytes;
    FBase64::Decode(Base64_String, Bytes);

    return Bytes;
}

// Convert From Bytes.

FString UExtendedVarsBPLibrary::Bytes_To_String(TArray<uint8> In_Bytes, bool bUseUnreal)
{
    if (In_Bytes.Num() == 0)
    {
        return "";
    }

    if (bUseUnreal)
    {
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

    else
    {
        const std::string Body_String(reinterpret_cast<const char*>(In_Bytes.GetData()), In_Bytes.Num());
        return Body_String.c_str();
    }
}

FString UExtendedVarsBPLibrary::Bytes_To_Hex(TArray<uint8> In_Bytes, int32 In_Size)
{
    std::stringstream ss;

    ss << std::hex << std::setfill('0');

    for (int i = 0; i < In_Size; i++) {
        ss << std::hex << std::setw(2) << static_cast<int>(In_Bytes.GetData()[i]);
    }

    return ss.str().c_str();
}

FString UExtendedVarsBPLibrary::Bytes_To_Each_Hex(TArray<uint8> In_Bytes, int32 Index)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::hex << std::setw(2) << static_cast<int>(In_Bytes.GetData()[Index]);

    return ss.str().c_str();
}

FString UExtendedVarsBPLibrary::Bytes_To_Base64(TArray<uint8> In_Bytes, bool bUseUrl)
{
    if (In_Bytes.Num() == 0)
    {
        return "";
    }

    FString Base64 = FBase64::Encode(In_Bytes.GetData(), In_Bytes.GetAllocatedSize());

    if (bUseUrl)
    {
        Base64.ReplaceInline(TEXT("+"), TEXT("-"));
        Base64.ReplaceInline(TEXT("/"), TEXT("_"));
        Base64.ReplaceInline(TEXT("="), TEXT(""));
    }

    return Base64;
}

// Convert To Bytes Objects.

bool UExtendedVarsBPLibrary::Bytes_To_B64_OBject(UBytesObject_64*& Out_Bytes_Object, TArray<uint8> In_Bytes)
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

bool UExtendedVarsBPLibrary::Bytes_To_B32_OBject(UBytesObject_32*& Out_Bytes_Object, TArray<uint8> In_Bytes)
{
    if (In_Bytes.Num() == 0)
    {
        return false;
    }

    TArray64<uint8> ByteArray;
    ByteArray.SetNum(In_Bytes.Num(), true);
    FMemory::Memcpy(ByteArray.GetData(), In_Bytes.GetData(), In_Bytes.GetAllocatedSize());

    UBytesObject_32* BytesObject_32 = NewObject<UBytesObject_32>();
    BytesObject_32->ByteArray = ByteArray;

    Out_Bytes_Object = BytesObject_32;

    return true;
}

// Convert From Bytes Objects

TArray<uint8> UExtendedVarsBPLibrary::B32_Object_To_Bytes(UBytesObject_32* In_Bytes_Object)
{
    if (IsValid(In_Bytes_Object))
    {
        return In_Bytes_Object->ByteArray;
    }

    else
    {
        TArray<uint8> EmptyArray;
        return EmptyArray;
    }
}

FString UExtendedVarsBPLibrary::Bytes_Object_To_String(UObject* In_Bytes_Object, bool bUseUnreal)
{
    if (!IsValid(In_Bytes_Object))
    {
        return "";
    }
    
    UBytesObject_64* Object_B64 = Cast<UBytesObject_64>(In_Bytes_Object);

    if (IsValid(Object_B64))
    {
        if (Object_B64->ByteArray.Num() == 0)
        {
            return "";
        }

        if (bUseUnreal)
        {
            int32 Index = 0;
            int32 Length = 0x7FFFFFFF;

            if (Index < 0)
            {
                Length += Index;
                Index = 0;
            }

            if (Length > Object_B64->ByteArray.Num() - Index)
            {
                Length = Object_B64->ByteArray.Num() - Index;
            }

            const FUTF8ToTCHAR Src(reinterpret_cast<const ANSICHAR*>(Object_B64->ByteArray.GetData() + Index), Length);
            return FString(Src.Length(), Src.Get());
        }

        else
        {
            const std::string Body_String(reinterpret_cast<const char*>(Object_B64->ByteArray.GetData()), Object_B64->ByteArray.Num());
            return Body_String.c_str();
        }
    }

    UBytesObject_32* Object_B32 = Cast<UBytesObject_32>(In_Bytes_Object);

    if (IsValid(Object_B32))
    {
        if (Object_B32->ByteArray.Num() == 0)
        {
            return "";
        }

        if (bUseUnreal)
        {
            int32 Index = 0;
            int32 Length = 0x7FFFFFFF;

            if (Index < 0)
            {
                Length += Index;
                Index = 0;
            }

            if (Length > Object_B32->ByteArray.Num() - Index)
            {
                Length = Object_B32->ByteArray.Num() - Index;
            }

            const FUTF8ToTCHAR Src(reinterpret_cast<const ANSICHAR*>(Object_B32->ByteArray.GetData() + Index), Length);
            return FString(Src.Length(), Src.Get());
        }

        else
        {
            const std::string Body_String(reinterpret_cast<const char*>(Object_B32->ByteArray.GetData()), Object_B32->ByteArray.Num());
            return Body_String.c_str();
        }
    }

    return "";
}

// Font Group.

URuntimeFont* UExtendedVarsBPLibrary::RuntimeFont_Load(TArray<uint8> In_Bytes)
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

bool UExtendedVarsBPLibrary::RuntimeFont_Release(UPARAM(ref)URuntimeFont*& In_RuntimeFont)
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

// String Group.

bool UExtendedVarsBPLibrary::Base64ToString(FString In_Base64, FString& OutDecoded)
{
    if (In_Base64.IsEmpty() == true)
    {
        return false;
    }

    return FBase64::Decode(In_Base64, OutDecoded);
}

FString UExtendedVarsBPLibrary::Int64ToString(int64 TargetInt64)
{
    return FString::Printf(TEXT("%llu"), TargetInt64);
}

TArray<FString> UExtendedVarsBPLibrary::StringSort(TArray<FString> TargetArray, bool bIsDescending)
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

// Math Group | Integer.

uint8 UExtendedVarsBPLibrary::NumberToByte(int32 In_Number)
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

int32 UExtendedVarsBPLibrary::Int32PlaceFamily(int32 TargetInteger)
{
    /*
    10 power integer character lenght minus 1 will equal its place family.
    For example to find 10's place family, 10^2(Char Lenght) - 1 = 10. 10's place family will 10.
    */
    
    return (int32)FMath::Pow(10.0, (double)((FString::FromInt(TargetInteger).Len()) - 1));
}

int32 UExtendedVarsBPLibrary::Int32TruncateToWholeBig(int32 TargetInteger)
{
    double Remainder;
    UKismetMathLibrary::FMod(TargetInteger, UExtendedVarsBPLibrary::Int32PlaceFamily(TargetInteger), Remainder);

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

int32 UExtendedVarsBPLibrary::Int32TruncateToWholeSmall(int32 TargetInteger)
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

TArray<int32> UExtendedVarsBPLibrary::Int32Sort(TArray<int32> TargetArray, bool bIsDescending)
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

void UExtendedVarsBPLibrary::Int32ToGraphics(EGraphicsType GraphicsType, int32 TargetInteger, int32 FullInteger, float& Scale, float& UnitValue)
{
    int32 FullScale = 0;

    switch (GraphicsType)
    {
        case EGraphicsType::ChartPie:
            FullScale = 360;
            break;
        case EGraphicsType::ChartBar:
            FullScale = 100;
            break;
    }
    
    Scale = (static_cast<float>(TargetInteger) * FullScale) / FullInteger;
    
    UnitValue = Scale / FullScale;
}

// Math Group | Float.

float UExtendedVarsBPLibrary::FloatPrecision(float TargetFloat, int32 Precision)
{
    int32 PrecisionFixer = pow(10, Precision);
    return floor(PrecisionFixer * TargetFloat) / PrecisionFixer;
}

int32 UExtendedVarsBPLibrary::FloatFractionCount(float TargetFloat)
{
    FString FractionString;
    (FString::SanitizeFloat(UKismetMathLibrary::Fraction(TargetFloat))).Split(TEXT("."), NULL, &FractionString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
   
    return FractionString.Len();
}

float UExtendedVarsBPLibrary::FloatRoundNext(float TargetFloat, double Decimal)
{
    int32 FractionCount = UExtendedVarsBPLibrary::FloatFractionCount(TargetFloat);
    
    if (FractionCount > Decimal)
    {
        return (FMath::TruncToInt(TargetFloat * FMath::Pow(10.0, Decimal)) + 1) / FMath::Pow(10.0, Decimal);
    }

    else
    {
        return TargetFloat;
    }
}

TArray<float> UExtendedVarsBPLibrary::FloatSort(TArray<float> TargetArray, bool bIsDescending)
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

// Time Group.

TArray<FDateTime> UExtendedVarsBPLibrary::TimeSort(TArray<FDateTime> TargetArray, bool bIsDescending)
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