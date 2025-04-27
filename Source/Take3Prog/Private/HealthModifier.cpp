#include "HealthModifier.h"
#include "mycomponents/HealthComponent.h"
#include "SquidProjectile.h"
#include "Kismet/KismetMathLibrary.h"

AHealthModifier::AHealthModifier()
{
    PrimaryActorTick.bCanEverTick = true;
    bCanLookAt = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;



    SquidModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquidModel"));
    SquidModel->SetupAttachment(RootComponent);
    SquidModel->SetUsingAbsoluteRotation(true);
}

void AHealthModifier::BeginPlay()
{
    Super::BeginPlay();

    

    GetWorld()->GetTimerManager().SetTimer(invincibiliityDelay, this, &AHealthModifier::DamageEnemy, damageCooldown, true);
}

void AHealthModifier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bCanLookAt)
    {
        UpdateLookAtTarget();

    }
}
void AHealthModifier::UpdateLookAtTarget()

{

    if (SquidModel || FollowTarget)

    {



        const FVector Start = SquidModel->GetComponentLocation();

        TArray<AActor*> OverlappingActors;

        SphereComponent->GetOverlappingActors(OverlappingActors);

        FVector End;

        for (AActor* actor : OverlappingActors)

        {

            const FVector EndTemp = actor->GetTargetLocation();

            End = EndTemp;

        }



        const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

        SquidModel->SetWorldRotation(LookAtRotation);

    }

}


void AHealthModifier::DamageEnemy()
{
    TArray<AActor*> OverlappingActors;
    SphereComponent->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        UHealthComponent* HealthComp = UHealthComponent::GetHealthComponent(Actor);
        if (HealthComp)
        {
            HealthComp->ApplyDamage(5);

            // Spawn projectile and set its target
            if (ProjectileActor)
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

                // Spawn projectile at the squid's location
                FVector SpawnLocation = SquidModel->GetComponentLocation();
                FRotator SpawnRotation = SquidModel->GetComponentRotation();

                ASquidProjectile* Projectile = GetWorld()->SpawnActor<ASquidProjectile>(
                    ProjectileActor,
                    SpawnLocation,
                    SpawnRotation,
                    SpawnParams
                );

                if (Projectile)
                {
                    // Set the target location to the actor's position
                    Projectile->SetTargetLocation(Actor->GetActorLocation());
                }
            }

            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(HealthComp->CurrentHealth));
        }
    }
}