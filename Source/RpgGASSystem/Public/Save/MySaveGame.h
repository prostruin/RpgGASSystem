// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AbilitySystem/Abilities/RPGGameplayAbility.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RPGGASSYSTEM_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 일반 데이터 (슬롯 정보)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGameData")
	FString SaveSlotName = TEXT("MySaveSlot");
    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGameData")
	int32 UserIndex = 0;

	// --- 1. 획득 능력 데이터 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GASPersistence")
	TArray<TSubclassOf<URPGGameplayAbility>> GrantedAbilities;

	// --- 2. 영구 속성 (Attribute) 데이터 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GASData|HP")
	float CurrentHealth;      // 현재 HP
    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GASData|HP")
	float BaseHealth;         // Health 속성의 베이스 값
    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GASData|HP")
	float BaseMaxHealth;      // MaxHealth 속성의 베이스 값
	
};
