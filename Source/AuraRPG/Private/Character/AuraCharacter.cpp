// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbiilitySystem/AuraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"


AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 750.0f;
	CameraBoom->bUsePawnControlRotation = false;
	//防止摄像机跟着人物视角旋转
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom);

	GetCharacterMovement()->bOrientRotationToMovement = true; // 允许角色的旋转方向与移动方向一致
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 设置旋转速度，这里仅设置yaw（左右）方向的旋转速度为540度/秒
	GetCharacterMovement()->bConstrainToPlane = true; // 限制角色移动在平面上
	GetCharacterMovement()->bSnapToPlaneAtStart = true; // 在角色开始移动时立即将其对齐到平面上

	bUseControllerRotationPitch = false; // 禁止角色的控制器pitch旋转
	bUseControllerRotationYaw = false; // 允许角色的控制器yaw旋转
	bUseControllerRotationRoll = false; // 禁止角色的控制器roll旋转
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init Ability Actor Info for Server
	InitAbilityActorInfo();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init Ability Actor Info for Clinet
	InitAbilityActorInfo();
	
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(PlayerController,AuraPlayerState,AbilitySystemComponent, AttributeSet);
		}
	}
}
