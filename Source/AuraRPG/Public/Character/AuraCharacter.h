// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 *
 */
UCLASS()
class AURARPG_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	//服务器上，当 Pawn 被某个 Controller 接管（Possess）时触发
	virtual void PossessedBy(AController* NewController) override;
	//客户端：当 PlayerState 变量（带有 Replicated 标记）从服务器同步过来时触发。
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;
	//初始化技能角色信息
	virtual void InitAbilityActorInfo() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

};
