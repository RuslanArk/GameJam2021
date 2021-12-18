// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "K_ActivateAbilityAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifyFiredSignature);

UCLASS()
class PINGLEGAMEJAM_API UK_ActivateAbilityAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FOnNotifyFiredSignature OnNotified;
	
};