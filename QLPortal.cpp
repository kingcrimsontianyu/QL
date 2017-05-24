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
#include "QLPortal.h"
#include "QLCharacter.h"
#include "QLWeaponPortalGun.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortal::AQLPortal()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Spouse = nullptr;

    // sound
    FireAndForgetSoundWaveList.Add("Teleport", CreateFireAndForgetSoundWave(TEXT("/Game/Sounds/teleport"), TEXT("SoundTeleportFireAndForget")));

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(50.0f, 120.0f, 150.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
    StaticMeshComponent->SetStaticMesh(StaticMeshObj.Object);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetWorldScale3D(FVector(0.01f, 2.4f, 3.0f));
    float xDim = BoxComponent->Bounds.BoxExtent.X; // note: extent refers to half of the side
    float zDim = StaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    StaticMeshComponent->SetRelativeLocation(FVector(-xDim + 0.1f, 0.0f, -zDim));
    StaticMeshComponent->bCastStaticShadow = false;
    StaticMeshComponent->bCastDynamicShadow = false;

    PortalCameraComp = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalCameraComp"));
    PortalCameraComp->InitBoxExtent(FVector(1.0f, 1.0f, 1.0f));
    PortalCameraComp->SetSimulatePhysics(false);
    PortalCameraComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PortalCameraComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    PortalCameraComp->SetupAttachment(RootComponent);
    PortalCameraComp->SetRelativeLocation(FVector(-xDim + 0.2f, 0.0f, 0.0f));

    // debugging purpose
    //HelperStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelperStaticMeshComponent"));
    //const ConstructorHelpers::FObjectFinder<UStaticMesh> HelperStaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
    //HelperStaticMeshComponent->SetStaticMesh(HelperStaticMeshObj.Object);
    //HelperStaticMeshComponent->SetSimulatePhysics(false);
    //HelperStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //HelperStaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    //HelperStaticMeshComponent->SetupAttachment(PortalCameraComp);
    //HelperStaticMeshComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 2.0f));
    //HelperStaticMeshComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 100.0f));
    //HelperStaticMeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // In order for scene capture and material instance dynamic
    // to work properly, it is critical to:
    // --- create a blueprint material and render target respectively
    // --- feed that material a texture sample
    // --- set the name of the texture sample as PortalTexture
    // --- set the texture sample's texture to render target
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    static ConstructorHelpers::FObjectFinder<UMaterial> BluePortalDefaultMaterialObj(TEXT("/Game/Blueprints/Weapon/PortalGun/Portal/M_QLBluePortalInActive"));
    static ConstructorHelpers::FObjectFinder<UMaterial> OrangePortalDefaultMaterialObj(TEXT("/Game/Blueprints/Weapon/PortalGun/Portal/M_QLOrangePortalInActive"));
    if (BluePortalDefaultMaterialObj.Succeeded())
    {
        BluePortalDefaultMaterial = BluePortalDefaultMaterialObj.Object;
    }
    if (OrangePortalDefaultMaterialObj.Succeeded())
    {
        OrangePortalDefaultMaterial = OrangePortalDefaultMaterialObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> BluePortalMaterialObj(TEXT("/Game/Blueprints/Weapon/PortalGun/Portal/M_QLBluePortalActive"));
    static ConstructorHelpers::FObjectFinder<UMaterial> OrangePortalMaterialObj(TEXT("/Game/Blueprints/Weapon/PortalGun/Portal/M_QLOrangePortalActive"));
    if (BluePortalMaterialObj.Succeeded())
    {
        BluePortalMaterial = BluePortalMaterialObj.Object;
    }
    if (OrangePortalMaterialObj.Succeeded())
    {
        OrangePortalMaterial = OrangePortalMaterialObj.Object;
    }

    // set up camera, texture at compile time
    PortalCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalCamera"));
    PortalCamera->SetupAttachment(PortalCameraComp);
    PortalCamera->bCaptureEveryFrame = true;
    PortalCamera->TextureTarget = nullptr;

    PortalRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("PortalRenderTarget"));
    PortalRenderTarget->InitAutoFormat(512, 640);
    PortalRenderTarget->AddressX = TextureAddress::TA_Wrap;
    PortalRenderTarget->AddressY = TextureAddress::TA_Wrap;

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLPortal::OnOverlapBeginForActor);
    this->OnActorEndOverlap.AddDynamic(this, &AQLPortal::OnOverlapEndForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPortal::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPortal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Spouse)
    {
        // set up myself: use spouse's camera to feed my material
        APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetPortalOwner()->GetWeaponOwner()->GetWorld(), 0);
        FVector CameraForward = this->GetActorLocation() - cm->GetCameraLocation();
        FRotator Inv = this->GetActorRotation().GetInverse();
        CameraForward = Inv.RotateVector(CameraForward);
        CameraForward.X = -CameraForward.X;
        CameraForward.Y = -CameraForward.Y;
        CameraForward = Spouse->GetActorRotation().RotateVector(CameraForward);

        FRotator NewRotation = UKismetMathLibrary::MakeRotFromXZ(CameraForward, Spouse->GetActorUpVector());
        Spouse->PortalCameraComp->SetWorldRotation(NewRotation);
    }
}

//------------------------------------------------------------
// --- When a portal happens to overlap another actor upon
//     spawn, the overlap event seems to be triggered inside of SpawnActor() method
//     before the portal owner is manually set, which means at that time
//     PortalOwner is still nullptr !!!
//     To guarantee overlap is called after the actor is spawned, deferred
//     actor spawn method is used. See AQLWeaponPortalGun::CreatePortal().
// --- If an actor has 2 components, both having overlap event, then
//     upon creation by SpawnActor(), the 2 components will trigger
//     their overlap event against each other, resulting in 2
//     overlap calls, in name of the actor itself. So here we always only
//     specify physics/collision in RootComponent and set the children
//     components to NoCollision and Ignore.
// --- Fun fact: Apparently, character getting teleported precedes the attaching actors
//     triggering the overlap event.
// --- One critical issue in teleport implementation is repeated
//     teleport back and forth between 2 portals. In fact, this
//     directly causes the game to crash (stack overflow maybe?). The solution here
//     is to use a roll (i.e. register list) to keep track of the overlapping
//     actors. Specifically, when an actor BEGINS to overlap a portal, the portal adds
//     that actor to its spouse's roll. After the actor is teleported to the spouse
//     and ENDS to overlap the spouse, the spouse removes the actor from its own roll.
//     Teleport upon overlap is only permitted if the actor does not appear in the
//     overlapped portal's roll.
// --- There is an annoying limitation: AActor only has velocity getter, not setter.
//------------------------------------------------------------
void AQLPortal::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    bool bCanTeleport = false;
    AQLPortal* AnotherPortal = Cast<AQLPortal>(OtherActor);
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
    AQLActor* QLActor = Cast<AQLActor>(OtherActor);

    // if the other portal exists, teleport can be performed
    if (Spouse)
    {
        // if the actor is not currently appear in my roll, teleport can be performed
        if (!IsInMyRoll(OtherActor))
        {
            // if the overlapping actor is not a portal
            if (!AnotherPortal)
            {
                // if OtherActor is character, teleport can be performed
                if (QLCharacter)
                {
                    bCanTeleport = true;
                }
                else
                {
                    // if OtherActor is AQLActor, teleport can be performed
                    if (QLActor)
                    {
                        // if OtherActor is not owned by character, teleport can be performed
                        if (QLActor->GetQLOwner() != PortalOwner->GetWeaponOwner())
                        {
                            bCanTeleport = true;
                        }
                    }
                }
            }
        }
    }

    // time to teleport!
    if (bCanTeleport)
    {
        // add actor to spouse's roll
        Spouse->AddToRoll(OtherActor);

        // update actor phase space
        // step 0: get portal delta rotation
        FRotator PortalRotation = this->GetActorRotation();
        FRotator PortalRotationInverse = this->GetActorRotation().GetInverse();
        FRotator PortalNewRotation = Spouse->GetActorRotation();

        // step 1: change velocity rotation
        FVector Velocity = OtherActor->GetVelocity();
        float Speed = Velocity.Size();
        Velocity = PortalRotationInverse.RotateVector(Velocity);
        Velocity.X = -Velocity.X;
        Velocity.Y = -Velocity.Y;
        FVector NewVelocity = PortalNewRotation.RotateVector(Velocity);

        // step 2: change actor rotation
        FVector ActorDirection = OtherActor->GetActorForwardVector();
        ActorDirection = PortalRotationInverse.RotateVector(ActorDirection);
        ActorDirection.X = -ActorDirection.X;
        ActorDirection.Y = -ActorDirection.Y;
        FVector NewActorDirection = PortalNewRotation.RotateVector(ActorDirection);
        FRotator NewActorRotation = UKismetMathLibrary::MakeRotFromXZ(NewActorDirection, OtherActor->GetActorUpVector());

        // step 3: change location
        // --> Here NewLocation cannot be set to Spouse->GetActorLocation(), in which case
        //     the actor overlap the portal upon transport and the engine automatically budge
        //     the actor but the actor's NewVelocity becomes 0. The character by default
        //     has a half height of 88 (including two radii) and a radius of 34.
        //     The character should be spawned at least 88 away from the portal in order to retain velocity.
        //     To prettify the teleport result, if the portal resides on a nearly vertical wall, the character
        //     is spawned 34 away from the portal.
        // --> If the portal thickness does not cover the distance between actor and portal after teleport,
        //     end overlap event cannot be triggered. So the portal must be made thick, 50 in here.
        FVector BudgeDir = Spouse->GetActorForwardVector();
        BudgeDir.Normalize();
        FVector Origin;
        FVector Bounds;
        OtherActor->GetActorBounds(true, Origin, Bounds);
        FVector NewLocation = Spouse->GetActorLocation() + BudgeDir * (Bounds.GetMax() + 1.0f);

        // if OtherActor is character, teleport can be performed
        if (QLCharacter)
        {
            // step 2 cont'd: change actor rotation
            FRotator NewActorRotationYawOnly = FRotator(0.0f, NewActorRotation.Yaw, 0.0f);

            // step 3 cont'd: change location
            // reduce budge distance if portal is spawned on a (nearly) vertical wall
            FVector SpouseDir = Spouse->GetActorUpVector();
            SpouseDir.Normalize();
            if (FGenericPlatformMath::Abs(SpouseDir.Z - 1.0f) < 0.1)
            {
                NewLocation = Spouse->GetActorLocation() + BudgeDir * (QLCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() + 1.0f);
            }

            // step 4: change controller/camera direction
            FVector CameraForwardDirection = QLCharacter->GetController()->GetActorForwardVector();
            CameraForwardDirection = PortalRotationInverse.RotateVector(CameraForwardDirection);
            CameraForwardDirection.X = -CameraForwardDirection.X;
            CameraForwardDirection.Y = -CameraForwardDirection.Y;
            FVector NewCameraForwardDirection = PortalNewRotation.RotateVector(CameraForwardDirection);

            FVector CameraUpDirection = QLCharacter->GetController()->GetActorUpVector();
            CameraUpDirection = PortalRotationInverse.RotateVector(CameraUpDirection);
            CameraUpDirection.X = -CameraUpDirection.X;
            CameraUpDirection.Y = -CameraUpDirection.Y;
            FVector NewCameraUpDirection = PortalNewRotation.RotateVector(CameraUpDirection);

            FRotator NewCameraRotation = UKismetMathLibrary::MakeRotFromXZ(NewCameraForwardDirection, NewCameraUpDirection);
            FRotator NewCameraRotationYawPitchOnly = FRotator(NewCameraRotation.Pitch, NewCameraRotation.Yaw, 0.0f);

            // teleport
            QLCharacter->TeleportTo(NewLocation, NewActorRotationYawOnly);
            QLCharacter->GetController()->SetControlRotation(NewCameraRotationYawPitchOnly);
            QLCharacter->GetMovementComponent()->Velocity = NewVelocity;
        }
        else
        {
            // if OtherActor is AQLActor, teleport can be performed
            if (QLActor)
            {
                // if OtherActor is not owned by character, teleport can be performed
                if (QLActor->GetQLOwner() != PortalOwner->GetWeaponOwner())
                {
                    // step 1 cont'd: change velocity
                    // impulse (vector) = mass (scalar) x velocity change (vector)
                    // ps: AddImpulse(velocity change, NAME_None, false) does not seem to work as expected
                    FVector Impulse = QLActor->GetStaticMeshComponent()->GetMass() * (NewVelocity - Velocity);

                    // teleport
                    QLActor->TeleportTo(NewLocation, NewActorRotation);

                    QLActor->GetStaticMeshComponent()->AddImpulse(Impulse);
                }
            }
        }

        //PlaySoundFireAndForget("Teleport", this->GetActorLocation());
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::OnOverlapEndForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    // remove actor from my own roll if any
    RemoveFromRoll(OtherActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetQLOwner(AActor* QLOwner)
{
    Super::SetQLOwner(QLOwner);
    this->PortalOwner = Cast<AQLWeaponPortalGun>(QLOwner);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::UnSetQLOwner()
{
    Super::SetQLOwner(nullptr);
    this->PortalOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponPortalGun* AQLPortal::GetPortalOwner()
{
    return PortalOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetSpouse(AQLPortal* Spouse)
{
    // no self-marriage
    if (this != Spouse)
    {
        this->Spouse = Spouse;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortal* AQLPortal::GetSpouse()
{
    return Spouse;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::QueryPortal()
{
    if (Roll.Num() > 0)
    {
        for (auto It = Roll.CreateConstIterator(); It; ++It)
        {
            QLUtility::QLSayLong("--> roll = " + It.Value()->GetName() + ". source = " + this->GetName());
        }
    }
    else
    {
        QLUtility::QLSayLong("--> empty roll. source = " + this->GetName());
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::AddToRoll(AActor* Actor)
{
    // if key exists, overwrite the value
    Roll.Add(Actor->GetName(), Actor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::RemoveFromRoll(AActor* Actor)
{
    // key does not have to exist
    AActor* Temp;
    bool found = Roll.RemoveAndCopyValue(Actor->GetName(), Temp);
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLPortal::IsInMyRoll(AActor* Actor)
{
    return Roll.Contains(Actor->GetName());
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetPortal(EPortalType PortalType, AQLPortal* Spouse)
{
    this->PortalType = PortalType;

    // set up crosshair
    if (PortalType == EPortalType::Blue && PortalOwner)
    {
        PortalOwner->CurrentCrosshairTextureList[0] = PortalOwner->CrosshairTextureList["BlueFilled"];
    }
    else if (PortalType == EPortalType::Orange && PortalOwner)
    {
        PortalOwner->CurrentCrosshairTextureList[1] = PortalOwner->CrosshairTextureList["OrangeFilled"];
    }

    // tell myself that I have a wife (existent or non-existent)
    SetSpouse(Spouse);

    // if my wife exists, tell her that she has a husband: me
    if (Spouse)
    {
        Spouse->SetSpouse(this);

        UMaterial* PortalMaterial = (PortalType == EPortalType::Blue) ? BluePortalMaterial : OrangePortalMaterial;

         // set up myself: use spouse's camera to feed my material
        Spouse->PortalCamera->TextureTarget = PortalRenderTarget;
        Spouse->PortalCamera->UpdateContent();
        PortalDynamicMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, PortalMaterial);
        PortalDynamicMaterial->SetTextureParameterValue("PortalTexture", PortalRenderTarget);

        // help set up my spouse if she/he is not set up yet
        if (!PortalCamera->TextureTarget)
        {
            UMaterial* SpousePortalMaterial = (PortalType == EPortalType::Blue) ? OrangePortalMaterial : BluePortalMaterial;
            PortalCamera->TextureTarget = Spouse->PortalRenderTarget;
            PortalCamera->UpdateContent();
            Spouse->PortalDynamicMaterial = Spouse->StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, SpousePortalMaterial);
            Spouse->PortalDynamicMaterial->SetTextureParameterValue("PortalTexture", Spouse->PortalRenderTarget);
        }
    }
    else
    {
        UMaterial* PortalMaterial = (PortalType == EPortalType::Blue) ? BluePortalDefaultMaterial : OrangePortalDefaultMaterial;
        PortalDynamicMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, PortalMaterial);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::UnsetPortal()
{
    // set up crosshair
    if (PortalType == EPortalType::Blue && PortalOwner)
    {
        PortalOwner->CurrentCrosshairTextureList[0] = PortalOwner->CrosshairTextureList["BlueEmpty"];
    }
    else if (PortalType == EPortalType::Orange && PortalOwner)
    {
        PortalOwner->CurrentCrosshairTextureList[1] = PortalOwner->CrosshairTextureList["OrangeEmpty"];
    }

    if (Spouse)
    {
        UMaterial* SpousePortalMaterial = (PortalType == EPortalType::Blue) ? OrangePortalMaterial : BluePortalMaterial;
        Spouse->PortalDynamicMaterial = Spouse->StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, SpousePortalMaterial);
        Spouse->PortalDynamicMaterial->SetTextureParameterValue("PortalTexture", Spouse->PortalRenderTarget);
        Spouse->SetSpouse(nullptr);
    }
    else
    {
        UMaterial* PortalMaterial = (PortalType == EPortalType::Blue) ? BluePortalDefaultMaterial : OrangePortalDefaultMaterial;
        PortalDynamicMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, PortalMaterial);
    }

    SetSpouse(nullptr);
}

//------------------------------------------------------------
//------------------------------------------------------------
UBoxComponent*& AQLPortal::GetBoxComponent()
{
    return BoxComponent;
}