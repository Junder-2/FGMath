#include "StateComponent.h"

UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
}
