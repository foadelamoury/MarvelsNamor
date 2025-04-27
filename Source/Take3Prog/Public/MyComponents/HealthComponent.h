// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAKE3PROG_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	uint32 MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Health")
	uint32 CurrentHealth;

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

public:

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(int32 DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	static UHealthComponent* GetHealthComponent(AActor* TargetActor)
	{
		if (!TargetActor) return nullptr;
		return TargetActor->FindComponentByClass<UHealthComponent>();
	}


};
