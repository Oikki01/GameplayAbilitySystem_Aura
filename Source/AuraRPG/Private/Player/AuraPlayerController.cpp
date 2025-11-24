// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	//用于控制一个对象是否需要在网络中进行复制（多人联机时使用）
	bReplicates = true;
}

//在函数中执行追踪，并对碰到的实现了敌人接口的角色进行高亮处理
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Subsystem);
	//注册输入映射
	Subsystem->AddMappingContext(AuraContext, 0);

	//设置鼠标显示
	bShowMouseCursor = true;
	//设置鼠标光标,默认设置为默认光标
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	///鼠标不会锁定到视角
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	///鼠标指针被捕获到视口时，鼠标指针不会隐藏
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ContorllerPawn = GetPawn<APawn>())
	{
		ContorllerPawn->AddMovementInput(ForwardDirection * InputAxisVector.Y);
		ContorllerPawn->AddMovementInput(RightDirection * InputAxisVector.X);
	}
}

/// <summary>
/// 在光标下获取命中结果
/// </summary>
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	///如果没有命中，则返回
	//if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	* 光标进行线性追踪的几种情况：
	* A.上一个角色和这个角色都是空的
	*		- 什么都不做
	* B.LastActor是Null && CurrentActor是Enemy
	*		- 高亮显示CurrentActor
	* C.LastActor是Enemy && CurrentActor是Null
	*		- 取消高亮显示上一个角色
	* D.LastActor和CurrentActor都是Enemy && LastActor != CurrentActor
	*		- 高亮显示当前角色，取消高亮显示上一个角色
	* E.LastActor和CurrentActor都是Enemy && LastActor == CurrentActor
	* 		- 什么都不做
	*/

	if (LastActor == nullptr)
	{
		if (CurrentActor != nullptr)
		{
			//case B
			CurrentActor->HighlightActor();
		}
		else
		{
			//case A
			//什么都不做
		}
	}
	else
	{
		if (CurrentActor == nullptr)
		{
			//case C
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != CurrentActor)
			{
				//case D
				LastActor->UnHighlightActor();
				CurrentActor->HighlightActor();
			}
			else
			{
				//case E
				//什么都不做
			}
		}
	}
}