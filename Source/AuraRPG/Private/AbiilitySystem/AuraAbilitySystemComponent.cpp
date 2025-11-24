// Fill out your copyright notice in the Description page of Project Settings.


#include "AbiilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent*, const FGameplayEffectSpec&,
                                                    FActiveGameplayEffectHandle)
{
}
