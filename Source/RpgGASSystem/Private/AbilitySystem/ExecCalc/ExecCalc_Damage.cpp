// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/RPGGameplayTags.h"
#include "AbilitySystem/Attributes/RPGAttributeSet.h"

struct  RPGDamageStatics
{
	// Source Captures

	// Target Captures
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingHealthDamage);
	
	RPGDamageStatics()
	{
		// Source Defines

		// Target Defines
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, IncomingHealthDamage, Target, false);
	}
};

static const RPGDamageStatics& DamageStatics()
{
	static RPGDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// Source Captures

	// Target Catures
	RelevantAttributesToCapture.Add(DamageStatics().IncomingHealthDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// Get raw damage value
	float Damage = EffectSpec.GetSetByCallerMagnitude(RPGGameplayTags::Combat::Data_Damage);
	Damage = FMath::Max<float>(Damage, 0.f);

	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingHealthDamageProperty, EGameplayModOp::Additive, Damage));
	}
	
}
