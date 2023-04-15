// Copyright (c) 2023, Gorkem CEYLAN All rights reserved.This source code is licensed under the Apache license found in the LICENSE file in the root directory of this source tree.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class FPSGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Components")
	USkeletalMeshComponent* MeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
