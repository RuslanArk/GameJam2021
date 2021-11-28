
#include "K_Support.h"


void FK_FloatParameter::SetData(const float NewData)
{
	if (OnParameterChanged.IsBound())
	{
		OnParameterChanged.Broadcast(Data, NewData);
	}
	Data = NewData;
}
