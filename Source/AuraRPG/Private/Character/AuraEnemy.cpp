// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbiilitySystem/AuraAbilitySystemComponent.h"
#include "AuraRPG/AuraRPG.h"

AAuraEnemy::AAuraEnemy()
{
	//设置角色的碰撞网格（Mesh）对可见性碰撞通道（ECC_Visibility）的响应为阻挡（ECR_Block）。
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	//设置对象在网络环境中可以被复制
	AbilitySystemComponent->SetIsReplicated(true);
	//设置复制模式(本模式游戏效果不会复制，但是游戏玩法提示和标签会复制给所有客户端)
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEnemy::InitAbilityActorInfo()
{
	check(AbilitySystemComponent);
	//初始化能力角色信息（所有者和具体控制都为本身）
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
