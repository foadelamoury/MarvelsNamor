#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "HealthModifier.generated.h"

UCLASS()
class TAKE3PROG_API AHealthModifier : public AActor
{
    GENERATED_BODY()

public:
    AHealthModifier();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* SphereComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USceneComponent* FollowTarget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SquidModel;

    UPROPERTY(EditAnywhere, Category = "Damage")

    TSubclassOf<class ASquidProjectile> ProjectileActor;

    UPROPERTY(EditAnywhere, Category = "Behavior")
    bool bCanLookAt;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TSet<class UHealthComponent*> ComponentsInRange;

    UPROPERTY(EditAnywhere, Category = "Damage")
	float damageCooldown;


    FTimerHandle invincibiliityDelay;
  
public:

    UFUNCTION(BlueprintCallable, Category = "Behavior")
    void UpdateLookAtTarget();

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void DamageEnemy();

};