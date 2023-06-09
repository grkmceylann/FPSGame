// Copyright (c) 2023, Gorkem CEYLAN All rights reserved.This source code is licensed under the Apache license found in the LICENSE file in the root directory of this source tree.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class AWeaponBase;

UCLASS(config=Game)
class FPSGAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "CharacterBase|Components")
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

public:

	// Sets default values for this character's properties
	ACharacterBase();

protected:

	/** Player's current weapon */
	AWeaponBase* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase|Weapon")
	TSubclassOf<AWeaponBase> StartWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "CharacterBase|Weapon")
	FName WeaponSocketName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouch */
	void BeginCrouch();

	/** Called for uncrouch */
	void EndCrouch();

	/** Called for shooting / firing */

	void StartFire();
	
public:

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase|Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase|Weapon")
	bool bHasWeapon;

	virtual FVector GetPawnViewLocation() const override;

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Weapon")
	bool GetHasWeapon() { return bHasWeapon;}
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const {	return FirstPersonMesh; }

	/** Returns FirstPersonCameraComponent subobject **/
	UFUNCTION(BlueprintCallable, Category = "CharacterBase | Weapon")
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Weapon")
	void SetHasWeapon(const bool bNewHasWeapon) { bHasWeapon = bNewHasWeapon; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CharacterBase|Getters")
	AWeaponBase* GetCurrentWeapon() const {	return CurrentWeapon; }
};

