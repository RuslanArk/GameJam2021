// Fill out your copyright notice in the Description page of Project Settings.


#include "K_EndAbilityAnimNotify.h"

void UK_EndAbilityAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast();
	Super::Notify(MeshComp, Animation);	
}
