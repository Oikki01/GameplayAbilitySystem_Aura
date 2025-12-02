// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbiilitySystem/AuraAbilitySystemComponent.h"
#include "AbiilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	//网络更新频率：这是服务器尝试更新客户端的频率，单位是秒。当玩家状态在服务器上变化时，服务器会向所有客户端发送更新，以便他们同步到最新版本。所以任何需要同步的变量都会更新，服务器会尽量达到设定的网络更新频率。
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	//设置复制模式（本模式游戏效果会复制到需要的客户端，游戏玩法提示和标签会复制给各个客户端）
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
