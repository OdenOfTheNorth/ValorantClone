// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

#define SURFACE_FLESHDEFAULT 	SurfaceType1
#define SURFACE_FLESHCRITICAL 	SurfaceType2
#define COLLISION_WEAPON 		ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{	
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm				UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel				UMETA(DisplayName = "Cancel"),
	// 3 
	QAbility			UMETA(DisplayName = "QAbility"),
	// 4 
	EAbility			UMETA(DisplayName = "EAbility"),
	// 5 Q
	CAbility			UMETA(DisplayName = "CAbility"),
	// 6 E
	XAbility			UMETA(DisplayName = "XAbility"),
	// 7 R
	FAbility			UMETA(DisplayName = "FAbility"),
	// 8 Jump
	Jump				UMETA(DisplayName = "Jump"),
	// 9 LMB
	PrimaryFire			UMETA(DisplayName = "Primary Fire"),
	// 10 RMB
	SecondaryFire		UMETA(DisplayName = "Secondary Fire"),
	// 11 Alternate Fire
	AlternateFire		UMETA(DisplayName = "Alternate Fire"),
	// 12 Reload
	Reload				UMETA(DisplayName = "Reload"),
	// 13 NextWeapon
	NextWeapon			UMETA(DisplayName = "Next Weapon"), 
	// 14 PrevWeapon
	PrevWeapon			UMETA(DisplayName = "Previous Weapon"),
	// 15 Interact
	Interact			UMETA(DisplayName = "Interact")
};
