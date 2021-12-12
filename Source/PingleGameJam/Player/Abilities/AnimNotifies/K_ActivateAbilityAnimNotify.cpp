// Fill out your copyright notice in the Description page of Project Settings.


#include "K_ActivateAbilityAnimNotify.h"

void UK_ActivateAbilityAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast();
	Super::Notify(MeshComp, Animation);	
}
