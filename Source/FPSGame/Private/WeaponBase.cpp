// Copyright (c) 2023, Gorkem CEYLAN All rights reserved.This source code is licensed under the Apache license found in the LICENSE file in the root directory of this source tree.


#include "WeaponBase.h"

#include "CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"
#include "FPSGame/FPSGame.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing (
	TEXT("FPSGame.DebugWeapons"), 
	DebugWeaponDrawing, 
	TEXT("Draw debug lines for weapons."), 
	ECVF_Cheat);

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Components
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Name Definitions
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	// Damage
	BaseDamage = 20.f;

	// RateOfFire
	RateOfFire = 600;

	// Recoil
	RecoilAmountOnPitch = 1.f;
	RecoilAmountOnYaw = 1.f;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;
}

void AWeaponBase::Fire()
{
	// TODO: Make recoil for player

	// Trace the world, from pawn eyes to crosshair location
	if (ACharacter* MyOwner = Cast<ACharacter>(GetOwner()))
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

		// Collision query parameters for HitTrace
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		// Particle "Target" parameter
		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			// Blocking hit process damage

			AActor* HitActor = Hit.GetActor();

			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 4.f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(),
			                                   this, DamageType);

			UParticleSystem* SelectedParticleEffect = nullptr;
			switch (SurfaceType)
			{
				case SURFACE_FLESHDEFAULT:
				case SURFACE_FLESHVULNERABLE:
					SelectedParticleEffect = FleshImpactEffect;
					break;

				default:
					SelectedParticleEffect = DefaultImpactEffect;
					break;
			}

			if (SelectedParticleEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
		}

		// TODO: Make the recoil based on function so we can have actual recoil. You can connect it to a counter which is resets on input released or canceled.
		// Weapon recoil
		MyOwner->AddControllerPitchInput(-RecoilAmountOnPitch);
		MyOwner->AddControllerYawInput(-RecoilAmountOnYaw);

		if (DebugWeaponDrawing)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
		}

		PlayFireEffects(TracerEndPoint);

		LastFiredTime = GetWorld()->TimeSeconds;

	}
}

void AWeaponBase::StartFire()
{
	float time = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red,FString::Printf(TEXT("time: %f"), time));
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red,FString::Printf(TEXT("time: %f"), TimeBetweenShots));
	}
	if (0 == time)
	{
		Fire();
	}
}

void AWeaponBase::AttachWeapon(const ACharacterBase* Character)
{
	if (Character)
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
				Subsystem->AddMappingContext(WeaponMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Fire
				EnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Triggered, this, &AWeaponBase::StartFire);
			}
		}
	}
}

void AWeaponBase::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
	}

	if (TraceEffect)
	{
		const FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

		if (UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect,
			MuzzleLocation))
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}

	if (const APawn* MyOwner = Cast<APawn>(GetOwner()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(MyOwner->GetController()))
		{
			PlayerController->ClientStartCameraShake(FireCamShake);
		}
	}
}

