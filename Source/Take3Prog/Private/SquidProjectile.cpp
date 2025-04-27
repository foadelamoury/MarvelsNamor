// Fill out your copyright notice in the Description page of Project Settings.


#include "SquidProjectile.h"

// Sets default values
ASquidProjectile::ASquidProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	squidProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Squid Projectile"));
	squidProjectileComponent->ProjectileGravityScale = 0;

	squidProjectileComponent->InitialSpeed = 800;
	squidProjectileComponent->MaxSpeed = 8500;

}

// Called when the game starts or when spawned
void ASquidProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3.0f);
	
}

// Called every frame
void ASquidProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TargetLocation.IsZero())
	{
		// Calculate direction to target
		FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		// Update velocity to move towards target
		squidProjectileComponent->Velocity = Direction * squidProjectileComponent->InitialSpeed;
	}

}
void ASquidProjectile::SetTargetLocation(const FVector& TargetLoc)
{
	TargetLocation = TargetLoc;
}

