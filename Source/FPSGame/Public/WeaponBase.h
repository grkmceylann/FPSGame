// Copyright (c) 2023, Gorkem CEYLAN All rights reserved.This source code is licensed under the Apache license found in the LICENSE file in the root directory of this source tree.

#pragma once

#include "CoreMinimal.h"
#include "LegacyCameraShake.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class ACharacterBase;

UCLASS()
class FPSGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AWeaponBase();

protected:

	virtual void BeginPlay() override;

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
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBase")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponBase")
	TSubclassOf<ULegacyCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponBase|Config")
	float BaseDamage;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponBase")
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponBase|Config")
	int32 MaxAmmo;

	/* RPM - Bullets per minute fired by weapon*/
	UPROPERTY(EditDefaultsOnly, Category = "WeaponBase|Config")
	float RateOfFire;

	/* How much camera move up on Fire*/
	UPROPERTY(EditDefaultsOnly, Category="WeaponBase|Config")
	float RecoilAmountOnPitch;

	/* How much camera move to sides on Fire. Positive for right and negative for left. */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponBase|Config")
	float RecoilAmountOnYaw;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponBase|Input")
	class UInputMappingContext* WeaponMappingContext;

	/** Shooting Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponBase|Input")
	class UInputAction* WeaponFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponBase|Input")
	class UInputAction* WeaponReloadAction;

	virtual void PlayFireEffects(FVector TraceEnd);

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void Reload();

	float LastFiredTime;

	// Derived from RateOfFire
	float TimeBetweenShots;

public:

	void StartFire();

	void AttachWeapon(const ACharacterBase* Character);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponBase|Getters")
	int32 GetMaxAmmo() const { return MaxAmmo; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WeaponBase|Getters")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }
};
