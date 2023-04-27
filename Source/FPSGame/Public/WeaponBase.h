// Copyright (c) 2023, Gorkem CEYLAN All rights reserved.This source code is licensed under the Apache license found in the LICENSE file in the root directory of this source tree.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class FPSGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AWeaponBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Components")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	UParticleSystem* TraceEffect;

	virtual void PlayFireEffects(FVector TraceEnd);

public:

	UFUNCTION(BlueprintCallable, Category = "WeaponBase")
	virtual void Fire();

};
