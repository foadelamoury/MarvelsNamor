// Fill out your copyright notice in the Description page of Project Settings.


#include "Take3Prog/MyActors/HealthArea.h"
#include "Components/SphereComponent.h"
#include "Take3Prog/Public/MyComponents/HealthComponent.h"

// Sets default values
AHealthArea::AHealthArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereFirst"));


}

// Called when the game starts or when spawned
void AHealthArea::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthArea::OnComponentBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AHealthArea::OnComponentEndOverlap);

	
}

// Called every frame
void AHealthArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (UHealthComponent* component : ComponentsInRange)
	{
		component->CurrentHealth -= 10;
	}

}

void AHealthArea::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;
	UHealthComponent* healthComponent = OtherActor->GetComponentByClass<UHealthComponent>();		
	if (!healthComponent)
		return;


	ComponentsInRange.Add(healthComponent);	
}

void AHealthArea::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (!OtherActor)
		return;
	UHealthComponent* healthComponent = OtherActor->GetComponentByClass<UHealthComponent>();
	if (!healthComponent)
		return;

	ComponentsInRange.Remove(healthComponent);


}

