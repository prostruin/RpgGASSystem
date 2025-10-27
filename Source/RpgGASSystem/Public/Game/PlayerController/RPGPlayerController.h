// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/InventoryInterface.h"
#include "Interfaces/RPGAbilitySystemInterface.h"
#include "RPGPlayerController.generated.h"

class URPGAbilitySystemComponent;
class URPGInputConfig;
class URPGSystemWidget;
class UInventoryWidgetController;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class RPGGASSYSTEM_API ARPGPlayerController : public APlayerController, public IAbilitySystemInterface, public IInventoryInterface, public IRPGAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARPGPlayerController();

	virtual void SetupInputComponent() override;

	/* Implement Inventroy Interface */
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override;

	/* Implement RPGAbilitySystemInterface Interface */
	virtual void SetDynamicProjectile_Implementation(const FGameplayTag& ProjectileTag) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UInventoryWidgetController* GetInventoryWidgetController();

	UFUNCTION(BlueprintCallable)
	void CreateInventoryWidget();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	
private:

	UPROPERTY()
	TObjectPtr<URPGAbilitySystemComponent> RPGAbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, Category="Custom Values|Input")
	TObjectPtr<URPGInputConfig> RPGInputConfig;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = true),Replicated)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Widgets")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<URPGSystemWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Widgets")
	TSubclassOf<URPGSystemWidget> InventoryWidgetClass;


	URPGAbilitySystemComponent* GetRPGAbilitySystemComponent();
	
	
};

