//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLWeaponGravityGun.h"
#include "QLCharacter.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponGravityGun::AQLWeaponGravityGun()
{
    PrimaryActorTick.bCanEverTick = true;
    Name = "GravityGun";
    bIsGravityGunCompatibleActorHeld = false;
    bIsAltFireHeldDown = false;
    RunningTimeAltFirePressed = 0.0f;
    FixedIntervalAltFirePressed = 0.2f;
    RunningTimeAltFireHeldDown = 0.0f;
    FixedIntervalAltFireHeldDown = 0.2f;
    ggcActor = nullptr;

    CrosshairTextureList.Add("Regular", CreateCrosshairTexture(TEXT("/Game/Blueprints/Weapon/GravityGun/Internal/gravity_gun_crosshair")));
    InitializeCurrentCrosshairTexture("Regular");

    // sound
    //SoundComponentList.Add("None", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/bottle"), TEXT("SoundNoneComp")));
    //SoundComponentList.Add("Hold", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/zoom_in"), TEXT("SoundHoldComp")));
    //SoundComponentList.Add("Fire", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/gravity_gun_fire"), TEXT("SoundFireComp")));
}

//------------------------------------------------------------
// If the actor that is compatible with gravity gun is within range but not being
// held by the character, or if that actor is being held by the character,
// the apply impulse to it.
//------------------------------------------------------------
void AQLWeaponGravityGun::Fire()
{
    // the actor is being held by the player
    if (bIsGravityGunCompatibleActorHeld)
    {
        // ggcActor is supposed to be non-null, but we check it just in case
        if (ggcActor)
        {
            UStaticMeshComponent* comp = ggcActor->GetStaticMeshComponent();

            // by design, the gravity gun compatible actor must have a component
            // here the component is checked for the sake of safety
            if (comp)
            {
                // character releases the actor
                bIsGravityGunCompatibleActorHeld = false;

                // reenable collision between character and component
                // enum members are shown in engine source code: EngineTypes.h
                comp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

                // physical handle releases the component
                WeaponOwner->PhysicsHandle->ReleaseComponent();
                // avoid comp stuck in sleep mode when at rest
                comp->WakeRigidBody();

                // unset logical ownership
                WeaponOwner->RemoveFromInventory(ggcActor);

                // apply repulsive force to the component
                APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(WeaponOwner->GetWorld(), 0);
                FVector Impulse = repulsionInstantSpeed * comp->GetMass() * cm->GetActorForwardVector();
                comp->AddImpulse(Impulse);

                // apply sound
                PlaySoundComponent("Fire");
            }
        }
    }
    // the actor is not being held by the player
    else
    {
        Hit = WeaponOwner->RayTraceFromCharacterPOV();
        // if hit occurs
        if (Hit.bBlockingHit)
        {
            // check if the hit actor compatible with (i.e. responsive to) gravity gun
            // if it is not, the result of cast is nullptr
            // this is a convenient runtime polymorphic type check
            AActor* hitActor = Hit.GetActor();
            ggcActor = Cast<AQLGravityGunCompatibleActor>(hitActor);
            if (ggcActor)
            {
                UStaticMeshComponent* comp = ggcActor->GetStaticMeshComponent();

                // by design, the gravity gun compatible actor must have a component
                // here the component is checked for the sake of safety
                if (comp)
                {
                    // if the compatible actor is considered next to the actor
                    if (WeaponOwner->IsObjectNextToCharacter(ggcActor))
                    {
                        // apply repulsive force to the component
                        APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(WeaponOwner->GetWorld(), 0);
                        FVector Impulse = repulsionInstantSpeed * comp->GetMass() * cm->GetActorForwardVector();
                        comp->AddImpulse(Impulse);

                        // apply sound
                        PlaySoundComponent("Fire");
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------
// If the actor that is compatible with gravity gun is far away,
// AltFire() will exert attractive force on it, until it becomes
// next to the player, when the player starts to hold it.
// When the actor is being held by the player, AltFire() will cause
// it to be released.
//
// Note that:
// bool bIsAltFireHeldDown; // key/button is held down for some time
// bool bIsAltFirePressed; // key/button is pressed, regardless if
// it is subsequently released or held down
//------------------------------------------------------------
void AQLWeaponGravityGun::AltFire()
{
    bIsAltFirePressed = true;
    WeaponOwner->bCanChangeCurrentWeapon = false;

    // the actor is being held by the player
    if (bIsGravityGunCompatibleActorHeld)
    {
        // only release the actor when the player is making a single click instead of
        // holding down alt fire
        if (!bIsAltFireHeldDown)
        {
            // ggcActor is supposed to be non-null, but we check it just in case
            if (ggcActor)
            {
                UStaticMeshComponent* comp = ggcActor->GetStaticMeshComponent();

                // by design, the gravity gun compatible actor must have a component
                // here the component is checked for the sake of safety
                if (comp)
                {
                    // character releases the actor
                    bIsGravityGunCompatibleActorHeld = false;

                    // reenable collision between character and component
                    // enum members are shown in engine source code: EngineTypes.h
                    comp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

                    // physical handle releases the component
                    WeaponOwner->PhysicsHandle->ReleaseComponent();
                    // avoid comp stuck in sleep mode when at rest
                    comp->WakeRigidBody();

                    // unset logical ownership
                    WeaponOwner->RemoveFromInventory(ggcActor);
                }
            }
        }
    }
    // the actor is not being held by the player
    else
    {
        Hit = WeaponOwner->RayTraceFromCharacterPOV();
        // if hit occurs
        if (Hit.bBlockingHit)
        {
            // check if the hit actor compatible with (i.e. responsive to) gravity gun
            // if it is not, the result of cast is nullptr
            // note: this is a convenient runtime polymorphic type check!!
            AActor* hitActor = Hit.GetActor();
            ggcActor = Cast<AQLGravityGunCompatibleActor>(hitActor);
            if (ggcActor)
            {
                UStaticMeshComponent* comp = ggcActor->GetStaticMeshComponent();

                // by design, the gravity gun compatible actor must have a component
                // here the component is checked for the sake of safety
                if (comp)
                {
                    // if the compatible actor is considered next to the actor
                    if (WeaponOwner->IsObjectNextToCharacter(ggcActor))
                    {
                        // character holds the actor
                        bIsGravityGunCompatibleActorHeld = true;

                        // get rotation delta between character and component
                        FRotator ggcActorRotation = comp->GetComponentRotation();
                        APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(WeaponOwner->GetWorld(), 0);
                        FRotator cameraRotation = cm->GetCameraRotation();
                        DeltaRotation = ggcActorRotation - cameraRotation;
                        DeltaRotation.Normalize();

                        // physical handle grabs the component
                        WeaponOwner->PhysicsHandle->GrabComponent(comp, Hit.BoneName, Hit.Location, true);

                        // set logical ownership
                        WeaponOwner->AddToInventory(ggcActor);

                        // disable collision between character and component
                        // enum members are shown in engine source code: EngineTypes.h
                        comp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

                        // apply sound
                        PlaySoundComponent("Hold");

                        // consequently player then holds the actor for every tick
                        // refer to Tick()
                    }
                    else
                    {
                        // apply attractive force to the component
                        APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(WeaponOwner->GetWorld(), 0);
                        FVector Impulse = -attractionInstantSpeed * comp->GetMass() * cm->GetActorForwardVector();
                        comp->AddImpulse(Impulse);
                    }
                }
            }
            // the ggcActor is not gravity gun compatible
            else
            {
                // apply sound
                PlaySoundComponent("None");
            }
        }
        else
        {
            // apply sound
            PlaySoundComponent("None");
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGravityGun::AltFireReleased()
{
    bIsAltFireHeldDown = false;
    bIsAltFirePressed = false;
    WeaponOwner->bCanChangeCurrentWeapon = true;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGravityGun::AltFireRepeat()
{
    AltFire();
}

//------------------------------------------------------------
// In Tick() several things are done:
// --- if alt fire is pressed, check if it is being held down
// --- if alt fire is being held down, call AltFireRepeat() at a constant interval
// --- if gravity gun compatible actor is being held, move it along with the character
//------------------------------------------------------------
void AQLWeaponGravityGun::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // if the gravity gun has been owned by character
    if (WeaponOwner)
    {
        if (bIsAltFirePressed)
        {
            RunningTimeAltFirePressed += DeltaSeconds;
            if (RunningTimeAltFirePressed >= FixedIntervalAltFirePressed)
            {
                bIsAltFireHeldDown = true;
                RunningTimeAltFirePressed = 0.0f;
            }
        }

        if (bIsAltFireHeldDown)
        {
            // If AltFireRepeat() is called for every tick, the attracted object will gain
            // momentum in a riduculous rapid fashion. As a workaround, AltFireRepeat() is
            // called every FixedInterval.
            RunningTimeAltFireHeldDown += DeltaSeconds;
            if (RunningTimeAltFireHeldDown >= FixedIntervalAltFireHeldDown)
            {
                AltFireRepeat();
                RunningTimeAltFireHeldDown = 0.0f;
            }
        }

        if (bIsGravityGunCompatibleActorHeld)
        {
            if (ggcActor)
            {
                // note:
                // --> ggcActor->SetActorLocation(newLocation);
                //     ggcActor->SetActorRotation(newRotation);
                //     will force actor to be in front of the character, in which case
                //     the actor may be embedded into the ground when the character is looking down
                // --> the actor may deviate from the camera center when rotating it fast.
                //     this is because full physics simulation is still enabled and the actor cannot be
                //     moved instantly with the camera.
                APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
                FVector newLocation = cm->GetCameraLocation() + cm->GetActorForwardVector() * distanceFromCharacterToActorWhenHold;
                FRotator newRotation = cm->GetCameraRotation() + DeltaRotation;
                newRotation.Normalize();
                WeaponOwner->PhysicsHandle->SetTargetLocation(newLocation);
                WeaponOwner->PhysicsHandle->SetTargetRotation(newRotation);
            }
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponGravityGun::ResetWeapon()
{
    // if the weapon is being owned
    if (WeaponOwner)
    {
        // if ggcActor exists and it is being held
        if (ggcActor && bIsGravityGunCompatibleActorHeld)
        {
            UStaticMeshComponent* comp = ggcActor->GetStaticMeshComponent();

            // by design, the gravity gun compatible actor must have a component
            // here the component is checked for the sake of safety
            if (comp)
            {
                // reenable collision between character and component
                // enum members are shown in engine source code: EngineTypes.h
                comp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

                // physical handle releases the component
                WeaponOwner->PhysicsHandle->ReleaseComponent();
                // avoid comp stuck in sleep mode when at rest
                comp->WakeRigidBody();

                // unset logical ownership
                WeaponOwner->RemoveFromInventory(ggcActor);
            }
        }

        bIsGravityGunCompatibleActorHeld = false;
        bIsAltFireHeldDown = false;
        bIsAltFirePressed = false;
        RunningTimeAltFirePressed = 0.0f;
        FixedIntervalAltFirePressed = 0.2f;
        RunningTimeAltFireHeldDown = 0.0f;
        FixedIntervalAltFireHeldDown = 0.2f;
        ggcActor = nullptr;
    }
}