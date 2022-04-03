// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExtendedVarsBPLibrary.h"
#include "ExtendedVars.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UExtendedVarsBPLibrary::UExtendedVarsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FString UExtendedVarsBPLibrary::Int64ToFString(int64 TargetInt64)
{
    return FString::Printf(TEXT("%llu"), TargetInt64);
}

uint8 UExtendedVarsBPLibrary::FStringToByte(const FString TargetString)
{
    int32 Number = FCString::Atoi(*TargetString);

    if (Number >= 0 && Number <= 255)
    {
        return Number;
    }

    else if (Number < 0)
    {
        return 0;
    }

    else if (Number > 255)
    {
        return 255;
    }

    return 0;
}

TArray<FString> UExtendedVarsBPLibrary::FStringSort(TArray<FString> TargetArray, bool bIsDescending)
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

int32 UExtendedVarsBPLibrary::Int32PlaceFamily(int32 TargetInteger)
{
    return FMath::Pow(10, ((FString::FromInt(TargetInteger).Len())-1));
}

int32 UExtendedVarsBPLibrary::Int32TruncateToWholeBig(int32 TargetInteger)
{
    float Remainder;
    UKismetMathLibrary::FMod(TargetInteger, UExtendedVarsBPLibrary::Int32PlaceFamily(TargetInteger), Remainder);

    if (Remainder != 0)
    {
        const FString IntString = FString::FromInt(TargetInteger);

        int32 PlaceDigit = FCString::Atoi(*(UKismetStringLibrary::GetCharacterArrayFromString(IntString)[0]));
        int32 PlaceFamily = FMath::Pow(10, ((IntString.Len()) - 1));

        return PlaceFamily * (PlaceDigit + 1);
    }

    else
    {
        return TargetInteger;
    }
}

int32 UExtendedVarsBPLibrary::Int32TruncateToWholeSmall(int32 TargetInteger)
{
    float Remainder;
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

void UExtendedVarsBPLibrary::Int32ToGraphics(EGraphicsType GraphicsType, int32 TargetInteger, int32 FullInteger, float& Scale, float& UnitValue)
{
    int32 FullScale;

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

int32 UExtendedVarsBPLibrary::FloatFractionCount(float TargetFloat)
{
    FString FractionString;
    (FString::SanitizeFloat(UKismetMathLibrary::Fraction(TargetFloat))).Split(TEXT("."), NULL, &FractionString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
   
    return FractionString.Len();
}

float UExtendedVarsBPLibrary::FloatRoundNext(float TargetFloat, int32 Decimal)
{
    int32 FractionCount = UExtendedVarsBPLibrary::FloatFractionCount(TargetFloat);
    
    if (FractionCount > Decimal)
    {
        return (FMath::TruncToInt(TargetFloat * FMath::Pow(10, Decimal)) + 1) / FMath::Pow(10, Decimal);
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