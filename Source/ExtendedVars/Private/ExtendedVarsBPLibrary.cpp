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
#include "HAL/FileManager.h"                    // Save Bitmap to Disk

UExtendedVarsBPLibrary::UExtendedVarsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

// Bytes Group

FString UExtendedVarsBPLibrary::BytesArrayToString(TArray<uint8> In_Bytes)
{
    if (In_Bytes.Num() > 0)
    {
        const std::string Body_String(reinterpret_cast<const char*>(In_Bytes.GetData()), In_Bytes.Num());
        FString Body_FString = Body_String.c_str();

        return Body_FString;
    }

    return "";
}

TArray<uint8> UExtendedVarsBPLibrary::StringToBytesArray(FString In_String)
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

bool UExtendedVarsBPLibrary::Read_From_Path(UBytesObject*& Out_Bytes_Object, FString In_Path)
{
    if (In_Path.IsEmpty() == true)
    {
        return false;
    }

    FPaths::NormalizeFilename(In_Path);
    if (FPaths::FileExists(In_Path) == false)
    {
        return false;
    }

    TArray64<uint8> ByteArray;
    FFileHelper::LoadFileToArray(ByteArray, *In_Path);

    UBytesObject* BytesObject = NewObject<UBytesObject>();
    BytesObject->ByteArray = ByteArray;

    Out_Bytes_Object = BytesObject;

    return true;
}

bool UExtendedVarsBPLibrary::BytesToBytesObject(UBytesObject*& Out_Bytes_Object, TArray<uint8> In_Bytes)
{
    if (In_Bytes.Num() == 0)
    {
        return false;
    }

    TArray64<uint8> ByteArray;
    ByteArray.SetNum(In_Bytes.Num(), true);
    FMemory::Memcpy(ByteArray.GetData(), In_Bytes.GetData(), In_Bytes.GetAllocatedSize());

    UBytesObject* BytesObject = NewObject<UBytesObject>();
    BytesObject->ByteArray = ByteArray;

    Out_Bytes_Object = BytesObject;

    return true;
}

// String Group

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

// Math Group | Integer

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
    
    Scale = (TargetInteger * FullScale) / FullInteger;
    
    UnitValue = Scale / FullScale;
}

// Math Group | Float

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

// Time Group

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

// Render Group

UTextureRenderTarget2D* UExtendedVarsBPLibrary::WidgetToTextureRenderTarget2d(FString& OutCode, UUserWidget* InWidget, FVector2D InDrawSize)
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

bool UExtendedVarsBPLibrary::Texture2dColorsArray(TArray<FColor>& Out_Array, UTexture2D* Texture)
{
    if (IsValid(Texture) == false)
    {
        return false;
    }

    int32 Texture_Width = Texture->GetSizeX();
    int32 Texture_Height = Texture->GetSizeY();

    FTexture2DMipMap& Texture_Mip = Texture->GetPlatformData()->Mips[0];
    void* Texture_Data = Texture_Mip.BulkData.Lock(LOCK_READ_WRITE);

    TArray<FColor> Array_Colors;
    Array_Colors.SetNum(Texture_Width * Texture_Height);

    // Texture data is BGRA formatted. So, we need to multiply with 4 for each color.
    FMemory::Memcpy(Array_Colors.GetData(), static_cast<FColor*>(Texture_Data), static_cast<SIZE_T>(Array_Colors.Num()) * 4);

    Texture_Mip.BulkData.Unlock();
    Texture->UpdateResource();

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

bool UExtendedVarsBPLibrary::ExportT2dAsBitmap(UTexture2D* Texture, FString Path)
{
    if (IsValid(Texture) == false)
    {
        return false;
    }

    if (Path.IsEmpty() == true)
    {
        return false;
    }

    FPaths::MakeStandardFilename(Path);

    TArray<FColor> Array_Colors;
    bool IsArrayCreated = UExtendedVarsBPLibrary::Texture2dColorsArray(Array_Colors, Texture);

    if (IsArrayCreated == true)
    {
        bool IsBitmapCreated = FFileHelper::CreateBitmap(*Path, Texture->GetSizeX(), Texture->GetSizeY(), Array_Colors.GetData(), NULL, &IFileManager::Get(), NULL, true);
        return IsBitmapCreated;
    }

    else
    {
        return false;
    }
}

// Path Group

FString UExtendedVarsBPLibrary::Android_Path_Helper(FString InFileName)
{
    if (UGameplayStatics::GetPlatformName() == "Android" || UGameplayStatics::GetPlatformName() == "IOS")
    {
        if (InFileName.IsEmpty() == true)
        {
            return "";
        }

        FString Path_Referance = FPlatformFileManager::Get().GetPlatformFile().ConvertToAbsolutePathForExternalAppForRead(*(FPaths::ProjectSavedDir()));

        TArray<FString> Path_Sections;
        Path_Referance.ParseIntoArray(Path_Sections, TEXT("/"), true);
        FString Path_Absolute = "/" + Path_Sections[0] + "/" + Path_Sections[1] + "/" + Path_Sections[2] + "/" + InFileName;

        return Path_Absolute;
    }

    else
    {
        return "";
    }
}