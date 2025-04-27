// Copyright Epic Games, Inc. All Rights Reserved.

#include "Take3ProgCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "HealthModifier.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATake3ProgCharacter

ATake3ProgCharacter::ATake3ProgCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

    HealthComponent->OnDeath.AddDynamic(this, &ATake3ProgCharacter::HandleDeath);

}

void ATake3ProgCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CoolDownTimer > 0.0f)
        CoolDownTimer -= DeltaTime;

    FHitResult hitResult;

    const bool isHit = UKismetSystemLibrary::SphereTraceSingle(
        GetWorld(), FollowCamera->GetComponentLocation(),
        FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 5000.0f,
        50.0f, ChannelToTrace, false,
        {}, EDrawDebugTrace::ForOneFrame, hitResult, true
    );

    if (isHit)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, hitResult.GetActor()->GetName());

        HitSpawn = FTransform(hitResult.ImpactPoint);

    }

    else
    {
        HitSpawn.Reset();
    }
}

void ATake3ProgCharacter::StartAiming()
{
    IsFocusing = true;
    APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (CamManager)
    {
        CamManager->SetFOV(60.f);
    }
}

void ATake3ProgCharacter::StopAiming()
{
    APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (CamManager)
    {
        CamManager->SetFOV(90.f);
    }
}



void ATake3ProgCharacter::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ATake3ProgCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATake3ProgCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATake3ProgCharacter::Look);
        EnhancedInputComponent->BindAction(CreateHealthModifierAction, ETriggerEvent::Triggered, this, &ATake3ProgCharacter::CreateHealthModifier);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ATake3ProgCharacter::StartAiming);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ATake3ProgCharacter::StopAiming);


    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
    }

    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATake3ProgCharacter::StartAiming);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATake3ProgCharacter::StopAiming);

    //PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    //PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ATake3ProgCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ATake3ProgCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ATake3ProgCharacter::CreateHealthModifier(const FInputActionValue& Value)
{
    if (CoolDownTimer > 0.0f)
    {
        return;
    }

    CoolDownTimer = CoolDownTime;
    if (HealthModifierActor)
    {
        HealthModifierActor->Destroy();
    }
    /*   if (HealthModifierClass && GetWorld())
       {
           FActorSpawnParameters SpawnParams;
           SpawnParams.Owner = this;
           SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

           FTransform SpawnTransform = GetActorTransform();
           GetWorld()->SpawnActor<AHealthModifier>(HealthModifierClass, SpawnTransform, SpawnParams);
       }*/
    if (HitSpawn.IsSet() && IsFocusing)
    {
        GetWorld()->SpawnActor<AHealthModifier>(HealthModifierClass, HitSpawn.GetValue());
        FVector location = HitSpawn.GetValue().GetLocation();
        OnHealthModifier(location);
    }
}

void ATake3ProgCharacter::HandleDeath()
{
    // Enable physics simulation
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // Disable character movement
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->StopMovementImmediately();

    // Disable input
    DisableInput(GetWorld()->GetFirstPlayerController());

    // Detach camera so it doesn't follow the ragdoll
    CameraBoom->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}
