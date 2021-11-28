
#pragma once

#include "CoreMinimal.h"
#include "K_Support.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FK_FloatParameter_OnParameterChanged, float, OldFloatData, float, NewFloatData);


USTRUCT(BlueprintType)
struct FK_FloatParameter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "FloatParameterEvent")
	FK_FloatParameter_OnParameterChanged OnParameterChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FloatParameter")
	float Data;

public:
	FK_FloatParameter() { Data = 0; }
	FK_FloatParameter(const float BaseData) { Data = BaseData; }

	void SetData(const float NewData);
	float GetData() const {	return Data; }
};
