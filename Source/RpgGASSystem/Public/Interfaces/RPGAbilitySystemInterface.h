// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "RPGAbilitySystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPGAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGGASSYSTEM_API IRPGAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetDynamicSpawnPoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel = 1);

	
};
