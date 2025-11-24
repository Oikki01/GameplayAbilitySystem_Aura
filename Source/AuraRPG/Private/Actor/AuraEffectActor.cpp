


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ShaderPrintParameters.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::AddEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//获取目标Actor的ASC 使用asc的静态库获取
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetAsc == nullptr) return;

	check(GameplayEffectClass);
	
	const FGameplayEffectContextHandle GameplayEffectContext = TargetAsc->MakeEffectContext();
	//ASC中有查找FGameplayEffectSpec的方法 第三个参数为FGameplayEffectContextHandle
	const FGameplayEffectSpecHandle GameplayEffectSpec = TargetAsc->MakeOutgoingSpec(GameplayEffectClass, ActorLevel,GameplayEffectContext);
	
	//为ASC应用GameplayEffect 参数为FGameplayEffectSpec;  GameplayEffectSpec.Data是一个TSharedPtr的指针需要使用.Get()来获取它的裸指针 参数只需要一个引用 但是目前为裸指针 所以需要在参数前加*变为引用类型
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpec.Data.Get());
	
	//获取游戏策略
	bool bIsTargetDurationType = GameplayEffectSpec.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsTargetDurationType && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//存储当前游戏ASC对象(用于移除游戏效果)和激活的handle(用于ASC移除游戏效果)
		ActiveEffectHandle.Add(ActiveGameplayEffectHandle, TargetAsc);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		AddEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DuartionEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		AddEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		AddEffectToTarget(TargetActor, InfiniteGamepalyEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		AddEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DuartionEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		AddEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//获取当前正在应用的游戏效果并移除它
		//根据UabilitySystemComponent查找当前激活的effct
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		//检查ASC是否有效
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRomove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandle)
		{
			if (HandlePair.Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);//1：单一效果移除 -1：移除全部效果
				//需要将移除的键存储起来，用于在映射中移除它们
				HandlesToRomove.Add(HandlePair.Key);
			}
		}
		for	(FActiveGameplayEffectHandle Handle : HandlesToRomove)
		{
			ActiveEffectHandle.FindAndRemoveChecked(Handle);
		}
	}
}

