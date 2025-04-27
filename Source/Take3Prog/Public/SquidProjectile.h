// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SquidProjectile.generated.h"

UCLASS()
class TAKE3PROG_API ASquidProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASquidProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* squidProjectileComponent;

	FVector TargetLocation;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTargetLocation(const FVector& TargetLoc);

};
