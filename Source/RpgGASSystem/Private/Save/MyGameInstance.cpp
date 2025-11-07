// MyGameInstance.cpp
#include "Save/MyGameInstance.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/RPGAttributeSet.h" // 반드시 사용자 정의 AttributeSet의 헤더를 포함해야 함

// --- Utility Functions (슬롯 이름/인덱스, 초기화) ---

void UMyGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Warning, TEXT("게임 인스턴스 init"));
    // 게임 시작 시, 저장 파일이 있으면 메모리로 로드하고, 없으면 기본 데이터로 초기화합니다.
    // TargetPawn이 아직 생성되지 않았을 수 있으므로 nullptr을 전달합니다.
    if (!LoadGameData(nullptr)) 
    {
        InitializeDefaultSaveData();
        UE_LOG(LogTemp, Warning, TEXT("Loading game data"));
    }
}

FString UMyGameInstance::GetSaveSlotName() const
{
    return TEXT("MySaveSlot");
}

int32 UMyGameInstance::GetUserIndex() const
{
    return 0;
}

void UMyGameInstance::InitializeDefaultSaveData()
{
    CurrentGameData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    if (CurrentGameData)
    {
        // 필요하다면 여기서 기본 Level, HP 등을 설정할 수 있습니다.
        CurrentGameData->BaseHealth = 100.0f;
        CurrentGameData->BaseMaxHealth = 100.0f;
        CurrentGameData->CurrentHealth = 100.0f;
        UE_LOG(LogTemp, Warning, TEXT("GameInstance: New default save data initialized."));
    }
}

// --- Save Game Data ---

bool UMyGameInstance::SaveGameData(APawn* TargetPawn)
{
    if (!TargetPawn || !CurrentGameData)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveGameData failed: TargetPawn or CurrentGameData is NULL."));
        return false;
    }
    
    UMySaveGame* SaveGameInstance = CurrentGameData;
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetPawn);

    if (ASC)
    {
        const URPGAttributeSet* AS = ASC->GetSet<URPGAttributeSet>();

        // ----------------------------------------------------
        // 1. 획득 능력 (Granted Abilities) 저장
        // ----------------------------------------------------
        SaveGameInstance->GrantedAbilities.Empty();
        TArray<FGameplayAbilitySpecHandle> AbilityHandles;
        ASC->GetAllAbilities(AbilityHandles);
        
        for (const FGameplayAbilitySpecHandle& Handle : AbilityHandles)
        {
            const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
            if (Spec && Spec->Ability)
            {
                // 어빌리티 클래스 (TSubclassOf) 저장
                SaveGameInstance->GrantedAbilities.Add(Spec->Ability->GetClass());
            }
        }
        
        // ----------------------------------------------------
        // 2. 영구 속성 (Attribute Base/Current Value) 저장
        // ----------------------------------------------------
        if (AS)
        {
            SaveGameInstance->CurrentHealth = AS->GetHealth(); 
            SaveGameInstance->BaseHealth = AS->Health.GetBaseValue();
            SaveGameInstance->BaseMaxHealth = AS->MaxHealth.GetBaseValue();

            UE_LOG(LogTemp, Log, TEXT("GAS Save Data Prepared: CurrentHP=%.2f, BaseMaxHP=%.2f, GrantedAbilities=%d"),
                SaveGameInstance->CurrentHealth, 
                SaveGameInstance->BaseMaxHealth, 
                SaveGameInstance->GrantedAbilities.Num());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveGameData: ASC not found on Pawn. Saving only non-GAS data."));
    }
    
    // 파일로 저장
    if (UGameplayStatics::SaveGameToSlot(CurrentGameData, GetSaveSlotName(), GetUserIndex()))
    {
        UE_LOG(LogTemp, Log, TEXT("✅ SAVE SUCCESS: All data saved to slot '%s'."), *GetSaveSlotName());
        return true;
    }
    
    UE_LOG(LogTemp, Error, TEXT("❌ SAVE FAILED: Could not save game to slot '%s'."), *GetSaveSlotName());
    return false;
}

// --- Load Game Data ---

bool UMyGameInstance::LoadGameData(APawn* TargetPawn)
{
    if (UGameplayStatics::DoesSaveGameExist(GetSaveSlotName(), GetUserIndex()))
    {
        USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(GetSaveSlotName(), GetUserIndex());
        UMySaveGame* LoadedMySaveGame = Cast<UMySaveGame>(LoadedGame);

        if (LoadedMySaveGame)
        {
            CurrentGameData = LoadedMySaveGame;
            UE_LOG(LogTemp, Log, TEXT("LOAD SUCCESS: Data loaded to GI memory from slot '%s'. BaseMaxHP=%.2f, Abilities=%d (Pending Apply)."), 
                *GetSaveSlotName(), 
                LoadedMySaveGame->BaseMaxHealth, 
                LoadedMySaveGame->GrantedAbilities.Num());
            // TargetPawn이 유효하면 GAS 데이터 적용
            if (TargetPawn)
            {
                UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetPawn);
                URPGAttributeSet* AS = ASC ? const_cast<URPGAttributeSet*>(ASC->GetSet<URPGAttributeSet>()) : nullptr;

                if (ASC && AS)
                {
                    // ----------------------------------------------------
                    // 1. 획득 능력 (Granted Abilities) 복원
                    // ----------------------------------------------------
                    // 기존 어빌리티 제거
                    TArray<FGameplayAbilitySpecHandle> HandlesToClear;
                    ASC->GetAllAbilities(HandlesToClear);
                    for (const FGameplayAbilitySpecHandle& Handle : HandlesToClear)
                    {
                        ASC->ClearAbility(Handle);
                    }
                    
                    // 저장된 목록 재부여
                    for (TSubclassOf<UGameplayAbility> AbilityClass : LoadedMySaveGame->GrantedAbilities)
                    {
                        if (AbilityClass)
                        {
                            ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0));
                        }
                    }

                    // ----------------------------------------------------
                    // 2. 영구 속성 (Attribute Base/Current Value) 복원
                    // ----------------------------------------------------
                    
                    // Max/Base 값 복원
                    AS->InitMaxHealth(LoadedMySaveGame->BaseMaxHealth);
                    AS->InitHealth(LoadedMySaveGame->BaseHealth); 
                    
                    // Current Health 복원 (클램핑)
                    float NewHealth = FMath::Min(LoadedMySaveGame->CurrentHealth, AS->GetMaxHealth());
                    AS->SetHealth(NewHealth);
                    
                    UE_LOG(LogTemp, Log, TEXT("✅ GAS Applied: Abilities Re-granted=%d. Final HP: %.2f / %.2f (Current/Max)."), 
                                        LoadedMySaveGame->GrantedAbilities.Num(), AS->GetHealth(), AS->GetMaxHealth());
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("LoadGameData: Target Pawn's ASC or AttributeSet is NULL. GAS data not applied."));
                }
            }
            return true;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ LOAD FAILED: Slot '%s' exists but failed to cast to UMySaveGame (Class Mismatch or Corrupted)."), *GetSaveSlotName());
            InitializeDefaultSaveData();
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadGameData: Save file does not exist in slot '%s'."), *GetSaveSlotName());
        return false;
    }
}

// --- Reset Save Data ---

bool UMyGameInstance::ResetSaveData()
{
    // 파일 삭제
    bool bFileDeleted = UGameplayStatics::DeleteGameInSlot(GetSaveSlotName(), GetUserIndex());
    if (bFileDeleted)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ SAVE RESET SUCCESS: File in slot '%s' has been deleted."), *GetSaveSlotName());
    }
    else
    {
        // 파일이 없었을 경우에도 성공으로 간주할 수 있음.
        UE_LOG(LogTemp, Warning, TEXT("SAVE RESET: File in slot '%s' did not exist or could not be deleted."), *GetSaveSlotName());
    }

    // 메모리 데이터 초기화
    CurrentGameData = nullptr;
    InitializeDefaultSaveData();

    return true;
}