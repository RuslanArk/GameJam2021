// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PingleGameJam/Player/K_BaseCharacter.h"
#include "K_WerewolfCharacter.generated.h"

class USphereComponent;

UCLASS()
class PINGLEGAMEJAM_API AK_WerewolfCharacter : public AK_BaseCharacter
{
	GENERATED_BODY()

public:
	AK_WerewolfCharacter();

protected:
	virtual void BeginPlay() override;


};
