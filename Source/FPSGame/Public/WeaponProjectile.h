// Copyright (c) 2023, Gorkem CEYLAN All rights reserved.This source code is licensed under the Apache license found in the LICENSE file in the root directory of this source tree.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponProjectile.generated.h"

/**
 *
 */
UCLASS()
class FPSGAME_API AWeaponProjectile : public AWeaponBase
{
	GENERATED_BODY()

protected:
	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponProjectile")
	TSubclassOf<AActor> ProjectileClass;
};
