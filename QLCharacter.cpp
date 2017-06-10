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
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLCharacter::AQLCharacter()
{
    Health = 25.0f;
    MaxHealth = 100.0f;
    Chi = 30.0f;
    MaxChi = 100.0f;
    bCanDoubleJump = false;
    bIsFalling = false;

    bIsSprinting = false;
    bWantToSprint = false;
    bAllWeaponAndSuperPowerUnlockable = true;

    bCanChangeCurrentWeapon = true;

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // movement
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.0f;
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
    MaxWalkSpeedCrouched = 100.0f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = MaxWalkSpeedCrouched;
    //GetCharacterMovement()->MaxStepHeight = 100.0f;

    // camera
    // note pawn has a member var: BaseEyeHeight
    QLCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    QLCameraComponent->SetupAttachment(GetCapsuleComponent());
    QLCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
    QLCameraComponent->bUsePawnControlRotation = true;

    // physics handle
    PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

    // creat all weapon slots
    WeaponList.Add("GravityGun", nullptr);
    WeaponList.Add("PortalGun", nullptr);
    WeaponList.Add("RecyclerGrenade", nullptr);
    CurrentWeapon = nullptr;
    LastWeapon = nullptr;

    // create all super power slots
    SuperPowerList.Add("TheWorld", nullptr);
    SuperPowerList.Add("MimicMatter", nullptr);
    CurrentSuperPower = nullptr;

    // sound
    //SoundNoAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundNoAttenuation"));
    //SoundNoAttenuation->Attenuation.bAttenuate = false;
    //SoundAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundAttenuation"));
    //SoundAttenuation->Attenuation.bAttenuate = true;
    //SoundAttenuation->Attenuation.bSpatialize = true;
    //SoundAttenuation->Attenuation.SpatializationAlgorithm = ESoundSpatializationAlgorithm::SPATIALIZATION_Default;

    // post-process
    static ConstructorHelpers::FObjectFinder<UMaterial> SuperPowerTheWorldMaterialObj(TEXT("/Game/Blueprints/SuperPower/TheWorld/Internal/M_QLTheWorldHaloPP"));
    if (SuperPowerTheWorldMaterialObj.Succeeded())
    {
        SuperPowerTheWorldMaterial = SuperPowerTheWorldMaterialObj.Object;
    }
    SuperPowerTheWorldDynamicMaterial = nullptr;

    ArmSkeletalMeshComponent = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter::~AQLCharacter()
{
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLCharacter::BeginPlay()
{
    Super::BeginPlay();
    //DebugHelper = GetWorld()->SpawnActor<AQLDebugHelper>(AQLDebugHelper::StaticClass());
    //DebugHelper->AttachToComponent(QLCameraComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    //DebugHelper->SetActorRelativeLocation(FVector(200.0f, 0.0f, 0.0f));

    SuperPowerTheWorldDynamicMaterial = UMaterialInstanceDynamic::Create(SuperPowerTheWorldMaterial, this);

    for (auto It = SoundComponentAssetList.CreateConstIterator(); It; ++It)
    {
        CreateSoundComponent(It->Key, It->Value);
    }

    for (auto It = FireAndForgetSoundAssetList.CreateConstIterator(); It; ++It)
    {
        CreateFireAndForgetSound(It->Key, It->Value);
    }
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLCharacter::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
// Called to bind functionality to input
//------------------------------------------------------------
void AQLCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent_ext)
{
    Super::SetupPlayerInputComponent(InputComponent_ext);

    // Set up "action" bindings.
    InputComponent_ext->BindAction("Jump", EInputEvent::IE_Pressed, this, &AQLCharacter::StartJump);
    InputComponent_ext->BindAction("Jump", EInputEvent::IE_Released, this, &AQLCharacter::StopJump);
    InputComponent_ext->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AQLCharacter::ToggleCrouch);
    InputComponent_ext->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AQLCharacter::ToggleSprint);
    InputComponent_ext->BindAction("Fire", EInputEvent::IE_Pressed, this, &AQLCharacter::Fire);
    InputComponent_ext->BindAction("Fire", EInputEvent::IE_Released, this, &AQLCharacter::FireReleased);
    InputComponent_ext->BindAction("AltFire", EInputEvent::IE_Pressed, this, &AQLCharacter::AltFire);
    InputComponent_ext->BindAction("AltFire", EInputEvent::IE_Released, this, &AQLCharacter::AltFireReleased);
    InputComponent_ext->BindAction("SwitchToGravityGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToGravityGun);
    InputComponent_ext->BindAction("SwitchToPortalGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToPortalGun);
    InputComponent_ext->BindAction("SwitchToRecyclerGrenade", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToRecyclerGrenade);
    InputComponent_ext->BindAction("SwitchToLastWeapon", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToLastWeapon);
    InputComponent_ext->BindAction("Test", EInputEvent::IE_Pressed, this, &AQLCharacter::Test);
    InputComponent_ext->BindAction("Inventory", EInputEvent::IE_Pressed, this, &AQLCharacter::ShowInventory);
    InputComponent_ext->BindAction("Zoom", EInputEvent::IE_Pressed, this, &AQLCharacter::Zoom);
    InputComponent_ext->BindAction("ExecuteSuperPower", EInputEvent::IE_Pressed, this, &AQLCharacter::ExecuteSuperPower);
    InputComponent_ext->BindAction("ReplayCurrentLevel", EInputEvent::IE_Pressed, this, &AQLCharacter::ReplayCurrentLevel);

    // Set up "axis" bindings.
    InputComponent_ext->BindAxis("MoveForward", this, &AQLCharacter::MoveForward);
    InputComponent_ext->BindAxis("MoveRight", this, &AQLCharacter::MoveRight);
    InputComponent_ext->BindAxis("Turn", this, &AQLCharacter::AddControllerYawInput);
    InputComponent_ext->BindAxis("LookUp", this, &AQLCharacter::AddControllerPitchInput);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // Find out which way is "forward" and record that the player wants to move that way.
        FRotator Rotation = Controller->GetControlRotation();
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        // Find out which way is "right" and record that the player wants to move that way.
        FRotator Rotation = Controller->GetControlRotation();
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::StartJump()
{
    // determine whether to do regular jump or double-jump

    // if double-jump is allowed, do it
    if (bCanDoubleJump)
    {
        // perform double jump
        LaunchCharacter({0 , 0, GetCharacterMovement()->JumpZVelocity}, false, true);

        PlaySoundComponent("DoubleJump");
    }
    else
    {
        bPressedJump = true;
    }

    bCanDoubleJump = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::StopJump()
{
    bPressedJump = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Landed(const FHitResult& Hit)
{
    bCanDoubleJump = false;
    bIsFalling = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Falling()
{
    bCanDoubleJump = true;
    bIsFalling = true;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ToggleCrouch()
{
    // If we are crouching then CanCrouch will return false. If we cannot crouch then calling Crouch() wont do anything
    if (CanCrouch() == true)
    {
        Crouch();
    }
    else
    {
        UnCrouch();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ToggleSprint()
{
    // toggle logic
    bWantToSprint = !bWantToSprint;

    if (CanSprint() == true)
    {
        Sprint();
    }
    else
    {
        UnSprint();
    }
}

//------------------------------------------------------------
// only true when the player wants to sprint and is not sprinting
// and is not falling
//------------------------------------------------------------
bool AQLCharacter::CanSprint() const
{
    if (bWantToSprint == true && bIsSprinting == false && bIsFalling == false)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Sprint()
{
    bWantToSprint = true;
    GetCharacterMovement()->MaxWalkSpeed = 3.0f * MaxWalkSpeed;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::UnSprint()
{
    bWantToSprint = false;
    GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

//------------------------------------------------------------
// delegate to weapon member function
//------------------------------------------------------------
void AQLCharacter::Fire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Fire();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::FireReleased()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->FireReleased();
    }
}

//------------------------------------------------------------
// delegate to weapon member function
//------------------------------------------------------------
void AQLCharacter::AltFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->AltFire();
    }
}

//------------------------------------------------------------
// delegate to weapon member function
//------------------------------------------------------------
void AQLCharacter::AltFireReleased()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->AltFireReleased();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToGravityGun()
{
    SwitchToWeapon("GravityGun");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToPortalGun()
{
    SwitchToWeapon("PortalGun");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToRecyclerGrenade()
{
    SwitchToWeapon("RecyclerGrenade");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToWeapon(const FName& Name)
{
    // check if weapon can be switched at this moment
    if (bCanChangeCurrentWeapon)
    {
        // check if the selected weapon is equipped
        if (IsWeaponEquipped(Name))
        {
            AQLWeapon* SelectedWeapon = WeaponList[Name];
            // first, unset all other equipped weapon except the selected weapon
            for (auto It = WeaponList.CreateIterator(); It; ++It)
            {
                if (It.Value() && It.Value() != SelectedWeapon)
                {
                    It.Value()->ResetWeapon();
                    It.Value()->SetActorHiddenInGame(true);
                    It.Value()->SetActorEnableCollision(false);
                    It.Value()->SetActorTickEnabled(false);
                }
            }

            // second, switch to the selected weapon
            ChangeCurrentWeapon(SelectedWeapon);
            SelectedWeapon->SetActorHiddenInGame(false);
            SelectedWeapon->SetActorEnableCollision(true);
            SelectedWeapon->SetActorTickEnabled(true);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToLastWeapon()
{
    if (LastWeapon)
    {
        SwitchToWeapon(LastWeapon->GetWeaponName());
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ExecuteSuperPower()
{
    if (CurrentSuperPower)
    {
        CurrentSuperPower->ExecuteSuperPower();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
FHitResult AQLCharacter::RayTraceFromCharacterPOV()
{
    const float rayTraceRange = 100000.0f;
    return RayTraceFromCharacterPOV(rayTraceRange);
}

//------------------------------------------------------------
//------------------------------------------------------------
FHitResult AQLCharacter::RayTraceFromCharacterPOV(float rayTraceRange)
{
    FCollisionQueryParams lineTraceParams = FCollisionQueryParams(FName(TEXT("lineTrace")), true, this);
    lineTraceParams.bTraceComplex = true;
    lineTraceParams.bTraceAsyncScene = true;
    lineTraceParams.bReturnPhysicalMaterial = false;

    APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    FVector start = cm->GetCameraLocation();
    FVector end = cm->GetActorForwardVector() * rayTraceRange + start;

    FHitResult hit(ForceInit);
    GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, lineTraceParams);

    // hit.bBlockingHit  // did ray hit something? (bool)
    // hit.GetActor();   // the hit actor if there is one
    // hit.ImpactPoint;  // FVector
    // hit.ImpactNormal; // FVector

    // for debugging purpose
    // DrawDebugLine(GetWorld(), start, hit.ImpactPoint, FColor(255, 0, 0), true, -1, 0, 10);

    return hit;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::IsObjectNextToCharacter(AQLGravityGunCompatibleActor* ggcActor)
{
    if (ggcActor)
    {
        if (GetDistanceTo(ggcActor) <= nextToPlayerThreshold)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------
// bAllWeaponUnlockable makes sure UnlockAllWeapon()
// can only be called once per character life cycle
//------------------------------------------------------------
void AQLCharacter::UnlockAllWeaponAndSuperPower()
{
    if (bAllWeaponAndSuperPowerUnlockable)
    {
        // weapon
        if (!IsWeaponEquipped("GravityGun"))
        {
            PickUpWeapon(GetWorld()->SpawnActor<AQLWeaponGravityGun>(AQLWeaponGravityGun::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!IsWeaponEquipped("PortalGun"))
        {
            PickUpWeapon(GetWorld()->SpawnActor<AQLWeaponPortalGun>(AQLWeaponPortalGun::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!IsWeaponEquipped("RecyclerGrenade"))
        {
            PickUpWeapon(GetWorld()->SpawnActor<AQLWeaponRecyclerGrenade>(AQLWeaponRecyclerGrenade::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!CurrentWeapon)
        {
            CurrentWeapon = WeaponList["GravityGun"];
        }

        // super power
        if (!IsSuperPowerEquipped("TheWorld"))
        {
            PickUpSuperPower(GetWorld()->SpawnActor<AQLSuperPowerTheWorld>(AQLSuperPowerTheWorld::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!IsSuperPowerEquipped("MimicMatter"))
        {
            PickUpSuperPower(GetWorld()->SpawnActor<AQLSuperPowerMimicMatter>(AQLSuperPowerMimicMatter::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!CurrentSuperPower)
        {
            //CurrentSuperPower = SuperPowerList["TheWorld"];
            CurrentSuperPower = SuperPowerList["MimicMatter"];
        }

        bAllWeaponAndSuperPowerUnlockable = false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PickUpWeapon(AQLWeapon* Weapon)
{
    // if the weapon exists
    if (Weapon)
    {
        // if the player does not have this weapon yet
        if (!IsWeaponEquipped(Weapon->GetWeaponName()))
        {
            WeaponList[Weapon->GetWeaponName()] = Weapon;
            SwitchToWeapon(Weapon->GetWeaponName());

            // set logical ownership
            Weapon->SetQLOwner(this);
            AddToInventory(Weapon);

            PlaySoundComponent("EquipWeapon");
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::DropOffWeapon(AQLWeapon* Weapon)
{
    // if the weapon exists
    if (Weapon)
    {
        if (IsWeaponEquipped(Weapon->GetWeaponName()))
        {
            WeaponList[Weapon->GetWeaponName()] = nullptr;
            CurrentWeapon = nullptr;

            // unset logical ownership
            Weapon->UnSetQLOwner();
            RemoveFromInventory(Weapon);

            // physical detachment
            Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeapon* AQLCharacter::GetCurrentWeapon() const
{
    return CurrentWeapon;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PickUpSuperPower(AQLSuperPower* SuperPower)
{
    // if the SuperPower exists
    if (SuperPower)
    {
        // if the player does not have this SuperPower yet
        if (!IsSuperPowerEquipped(SuperPower->GetSuperPowerName()))
        {
            SuperPowerList[SuperPower->GetSuperPowerName()] = SuperPower;
            ChangeCurrentSuperPower(SuperPower);

            // set logical ownership
            SuperPower->SetQLOwner(this);
            AddToInventory(SuperPower);

            // physical attachment
            SuperPower->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLSuperPower* AQLCharacter::GetCurrentSuperPower() const
{
    return CurrentSuperPower;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::AddToInventory(AQLActor* QLActor)
{
    // if key exists, overwrite the value
    Inventory.Add(QLActor->GetName(), QLActor);
    QLActor->SetQLOwner(this);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::RemoveFromInventory(AQLActor* QLActor)
{
    // if key does not exist, throw a warning
    AActor* Temp;
    bool bKeyExist = Inventory.RemoveAndCopyValue(QLActor->GetName(), Temp);
    QLActor->UnSetQLOwner();

    if (!bKeyExist)
    {
        QLUtility::QLSay(FString("AQLCharacter::RemoveFromInventory(): key does not exist. ") + FString(QLActor->GetName()));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ShowInventory()
{
    for (auto It = Inventory.CreateConstIterator(); It; ++It)
    {
        QLUtility::QLSay(FString("--> character inventory item = ") + It.Value()->GetName() + "     location = " + It.Value()->GetActorLocation().ToString());
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Zoom()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Zoom();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ChangeCurrentWeapon(AQLWeapon* Weapon)
{
    // if the target weapon exists
    // note that the current weapon is allowed to be nonexistent
    if (Weapon != nullptr)
    {
        // if the target weapon is not the same with the current weapon
        if (CurrentWeapon != Weapon)
        {
            LastWeapon = CurrentWeapon;
            CurrentWeapon = Weapon;

            PlaySoundComponent("SwitchWeapon");
        }
    }
}

//------------------------------------------------------------
// check if the specified type of weapon (identified by name)
// is already equipped by the player
//------------------------------------------------------------
bool AQLCharacter::IsWeaponEquipped(const FName& Name)
{
    // if the given name exists in the preset list
    if (WeaponList.Contains(Name))
    {
        // if the player has that type of weapon
        if (WeaponList[Name] != nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        QLUtility::QLSay("AQLCharacter::IsWeaponEquipped(): unknown weapon type.");
        return false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ChangeCurrentSuperPower(AQLSuperPower* SuperPower)
{
    // if the target weapon exists
    // note that the current weapon is allowed to be nonexistent
    if (SuperPower != nullptr)
    {
        // if the target weapon is not the same with the current weapon
        if (CurrentSuperPower != SuperPower)
        {
            CurrentSuperPower = SuperPower;
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::IsSuperPowerEquipped(const FName& Name)
{
    // if the given name exists in the preset list
    if (SuperPowerList.Contains(Name))
    {
        // if the player has that type of weapon
        if (SuperPowerList[Name] != nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        QLUtility::QLSay("AQLCharacter::IsSuperPowerEquipped(): unknown weapon type.");
        return false;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PlaySoundComponent(const FName& SoundName)
{
    QLUtility::PlaySoundComponent(SoundComponentList, SoundName);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::PlaySoundFireAndForget(const FName& SoundName)
{
    QLUtility::PlaySoundFireAndForget(this->GetWorld(),
        FireAndForgetSoundList,
        SoundName,
        GetActorLocation(),
        SoundAttenuation);
}

//------------------------------------------------------------
//------------------------------------------------------------
UAudioComponent* AQLCharacter::CreateSoundComponent(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset)
{
    FSoundResult Result;
    UQLGameInstance* QLGameInstance = Cast<UQLGameInstance>(GetGameInstance());
    if (QLGameInstance)
    {
        Result = QLGameInstance->AddToSoundWaveList(SoundWaveAsset);
    }

    UAudioComponent* SoundComp = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass());

    if (Result.SoundWave && SoundComp)
    {
        SoundComp->SetSound(Result.SoundWave);
        SoundComp->SetupAttachment(RootComponent);
        SoundComp->SetRelativeLocation(FVector(0.0f));
        SoundComp->bAutoActivate = false;
        //SoundComp->AdjustAttenuation(SoundAttenuation->Attenuation);
        SoundComp->RegisterComponent(); // must occur after audio component is properly set up

        SoundComponentList.Add(SoundName, SoundComp);
    }

    if (!Result.bNew)
    {
        QLUtility::QLSay(SoundName.ToString() + " is not new.");
    }

    return SoundComp;
}

//------------------------------------------------------------
//------------------------------------------------------------
USoundWave* AQLCharacter::CreateFireAndForgetSound(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset)
{
    FSoundResult Result;
    UQLGameInstance* QLGameInstance = Cast<UQLGameInstance>(GetGameInstance());
    if (QLGameInstance)
    {
        Result = QLGameInstance->AddToSoundWaveList(SoundWaveAsset);
    }

    if (Result.SoundWave)
    {
        FireAndForgetSoundList.Add(SoundName, Result.SoundWave);
    }

    if (!Result.bNew)
    {
        QLUtility::QLSay(SoundName.ToString() + " is not new.");
    }

    return Result.SoundWave;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::Test()
{
    //QLUtility::QLSayLong("act rot = " + this->GetActorRotation().ToString()
    //    + "     loc = " + this->GetActorLocation().ToString()
    //    + "     forward = " + this->GetActorForwardVector().ToString());

    //QLUtility::QLSayLong("capsule rot = " + GetCapsuleComponent()->GetComponentRotation().ToString()
    //    + "     loc = " + GetCapsuleComponent()->GetComponentLocation().ToString()
    //    + "     forward = " + GetCapsuleComponent()->GetForwardVector().ToString());

    //QLUtility::QLSayLong("controller rot = " + GetController()->GetControlRotation().ToString()
    //    + "     forward = " + GetController()->GetActorForwardVector().ToString());
    //APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

    //QLUtility::QLSayLong("camera rot = " + cm->GetCameraRotation().ToString()
    //    + "     loc = " + cm->GetCameraLocation().ToString()
    //    + "     forward = " + cm->GetActorForwardVector().ToString());

    //FVector Location;
    //FRotator Rotation;
    //GetController()->GetPlayerViewPoint(Location, Rotation);
    //QLUtility::QLSayLong("player view rot = " + Rotation.ToString() + "     loc = " + Location.ToString());

    //QLUtility::QLSayLong("world = " + this->GetWorld()->GetName());
    //QLUtility::QLSayLong("level = " + this->GetWorld()->GetCurrentLevel()->GetName());
    //QLUtility::QLSayLong("game mode = " + this->GetWorld()->GetAuthGameMode()->GetName());
    //QLUtility::QLSayLong("character = " + this->GetName());
    //QLUtility::QLSayLong("play controller = " + this->GetController()->GetName());

    UnlockAllWeaponAndSuperPower();
}

//------------------------------------------------------------
//------------------------------------------------------------
UMaterialInstanceDynamic* AQLCharacter::GetSuperPowerTheWorldDynamicMaterial()
{
    return SuperPowerTheWorldDynamicMaterial;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLCharacter::GetHealth() const
{
    return Health;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLCharacter::GetChi() const
{
    return Chi;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::IncrementHealth_Implementation(float increment)
{
    // if the player has already had full health
    // or if the player is dead, do not change health
    if (Health >= MaxHealth || Health <= 0.0f)
    {
        return false;
    }
    else
    {
        Health += increment;

        if (Health > MaxHealth)
        {
            Health = MaxHealth;
        }

        if (Health <= 0.0f)
        {
            // to do: dead
        }

        return true;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLCharacter::IncrementChi_Implementation(float increment)
{
    // if the player has already had full Chi
    if (Chi >= MaxChi || Chi < 0.0f)
    {
        return false;
    }
    else
    {
        Chi += increment;

        if (Chi > MaxChi)
        {
            Chi = MaxChi;
        }

        if (Chi <= 0.0f)
        {
            Chi = 0.0f;
        }

        return true;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
USkeletalMeshComponent* AQLCharacter::GetArm() const
{
    return ArmSkeletalMeshComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
UCameraComponent* AQLCharacter::GetQLCameraComponent() const
{
    return QLCameraComponent;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::ReplayCurrentLevel()
{
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}