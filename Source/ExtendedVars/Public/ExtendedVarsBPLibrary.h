// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExtendedVarsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UENUM()
enum class EGraphicsType : uint8
{
	ChartPie	UMETA(DisplayName = "Pie Chart"),
	ChartBar	UMETA(DisplayName = "Bar Chart"),
};

UCLASS()
class UExtendedVarsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int64 To FString", Keywords = "int64, fstring, convert"), Category = "Extended Variables|String")
	static FString Int64ToFString(int64 TargetInt64);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "FString to Byte", Keywords = "byte, fstring, convert"), Category = "Extended Variables|String")
	static uint8 FStringToByte(const FString TargetString);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Int32 Place Family", Keywords = "int32, get, place, family"), Category = "Extended Variables|Integer")
	static int32 GetInt32PlaceFamily(int32 Integer);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 Truncate to Whole (Big)", Keywords = "int32, truncate, truncation, full, whole, big"), Category = "Extended Variables|Integer")
	static int32 Int32TruncateToWholeBig(int32 Integer);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 Truncate to Whole (Small)", Keywords = "int32, truncate, truncation, full, whole, small"), Category = "Extended Variables|Integer")
	static int32 Int32TruncateToWholeSmall(int32 Integer);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Int32 To Graphics", ToolTip = "Unit Value gives scale as a float between 0 and 1.", Keywords = "int32, graphics, pie"), Category = "Extended Variables|Graphics")
	static void Int32ToGraphics(EGraphicsType GraphicsType, int32 ValueTarget, int32 ValueFull, float& Scale, float& UnitValue);

};
