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

int32 UExtendedVarsBPLibrary::GetInt32PlaceFamily(int32 TargetInteger)
{
    return FMath::Pow(10, ((FString::FromInt(TargetInteger).Len())-1));
}

int32 UExtendedVarsBPLibrary::Int32TruncateToWholeBig(int32 TargetInteger)
{
    float Remainder;
    UKismetMathLibrary::FMod(TargetInteger, UExtendedVarsBPLibrary::GetInt32PlaceFamily(TargetInteger), Remainder);

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

void UExtendedVarsBPLibrary::Int32ToGraphics(EGraphicsType GraphicsType, int32 ValueTarget, int32 ValueFull, float& Scale, float& UnitValue)
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
    
    Scale = (ValueTarget * FullScale) / ValueFull;
    
    UnitValue = Scale / FullScale;
}