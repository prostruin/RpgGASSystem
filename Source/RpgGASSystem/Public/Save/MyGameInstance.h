// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MySaveGame.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGGASSYSTEM_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 메모리에 로드된 SaveGame 데이터
	UPROPERTY(BlueprintReadWrite, Category = "SaveLoad")
	UMySaveGame* CurrentGameData;

	virtual void Init() override;

	// --- Save/Load/Reset 함수 (BlueprintCallable로 설정) ---
    
	/** * TargetPawn의 GAS 데이터를 추출하여 파일로 저장합니다.
	 * @param TargetPawn - 데이터를 가져올 캐릭터 (APawn)
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool SaveGameData(APawn* TargetPawn);

	/**
	 * 저장된 데이터를 메모리(CurrentGameData)로 로드하고, 유효한 Pawn이 있으면 데이터를 적용합니다.
	 * @param TargetPawn - 데이터를 적용할 캐릭터 (NULL 가능)
	 * @return 로드 성공 여부 (파일 존재 및 메모리 로드 성공 시 true)
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool LoadGameData(APawn* TargetPawn);

	/**
	 * 저장 파일을 삭제하고 메모리 데이터를 기본값으로 재설정합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	bool ResetSaveData();

protected:
	// CurrentGameData가 없을 때 기본값으로 초기화합니다.
	void InitializeDefaultSaveData();

private:
	// Save/Load 슬롯 정보
	FString GetSaveSlotName() const;
	int32 GetUserIndex() const;
};
