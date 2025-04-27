// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GetCurrentHealth();
	CurrentHealth = MaxHealth;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ApplyDamage(int32 DamageAmount)
{
	if (CurrentHealth <= 0) return; // Already dead

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0, MaxHealth);

	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}



