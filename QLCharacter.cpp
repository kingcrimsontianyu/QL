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
    bCanDoubleJump = false;
    bIsFalling = false;

    bIsSprinting = false;
    bWantToSprint = false;
    bAllWeaponAndSuperPowerUnlockable = true;

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // movement
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 1.0f;
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
    WeaponList.Add("NeutronAWP", nullptr);
    CurrentWeapon = nullptr;
    LastWeapon = nullptr;
    CurrentSuperPower = nullptr;

    // create all super power slots
    SuperPowerList.Add("TheWorld", nullptr);

    // sound
    SoundNoAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundNoAttenuation"));
    SoundNoAttenuation->Attenuation.bAttenuate = false;
    SoundAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundAttenuation"));
    SoundAttenuation->Attenuation.bAttenuate = true;
    SoundAttenuation->Attenuation.DistanceAlgorithm = ESoundDistanceModel::ATTENUATION_Linear;
    SoundComponentList.Add("EquipWeapon", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/medshot4_from_hl2"), TEXT("SoundEquipWeaponComp")));
    SoundComponentList.Add("SwitchWeapon", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/swords_collide"), TEXT("SoundSwitchWeaponComp")));
    SoundComponentList.Add("DoubleJump", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/quake_jump"), TEXT("SoundDoubleJumpComp")));

    // post-process
    static ConstructorHelpers::FObjectFinder<UMaterial> SuperPowerTheWorldMaterialObj(TEXT("/Game/Materials/TheWorld/M_QLTheWorldHaloPP"));
    if (SuperPowerTheWorldMaterialObj.Succeeded())
    {
        SuperPowerTheWorldMaterial = SuperPowerTheWorldMaterialObj.Object;
    }
    SuperPowerTheWorldDynamicMaterial = nullptr;
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
void AQLCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    // Set up "action" bindings.
    InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AQLCharacter::StartJump);
    InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AQLCharacter::StopJump);
    InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AQLCharacter::ToggleCrouch);
    InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AQLCharacter::ToggleSprint);
    InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AQLCharacter::Fire);
    InputComponent->BindAction("AltFire", EInputEvent::IE_Pressed, this, &AQLCharacter::AltFire);
    InputComponent->BindAction("AltFire", EInputEvent::IE_Released, this, &AQLCharacter::AltFireReleased);
    InputComponent->BindAction("SwitchToGravityGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToGravityGun);
    InputComponent->BindAction("SwitchToPortalGun", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToPortalGun);
    InputComponent->BindAction("SwitchToNeutronAWP", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToNeutronAWP);
    InputComponent->BindAction("SwitchToLastWeapon", EInputEvent::IE_Pressed, this, &AQLCharacter::SwitchToLastWeapon);
    InputComponent->BindAction("Test", EInputEvent::IE_Pressed, this, &AQLCharacter::Test);
    InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &AQLCharacter::ShowInventory);
    InputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &AQLCharacter::Zoom);
    InputComponent->BindAction("ExecuteSuperPower", EInputEvent::IE_Pressed, this, &AQLCharacter::ExecuteSuperPower);

    // Set up "axis" bindings.
    InputComponent->BindAxis("MoveForward", this, &AQLCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AQLCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AQLCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AQLCharacter::AddControllerPitchInput);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
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
void AQLCharacter::SwitchToNeutronAWP()
{
    SwitchToWeapon("NeutronAWP");
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToWeapon(const FName& Name)
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
            }
        }

        // second, switch to the selected weapon
        ChangeCurrentWeapon(SelectedWeapon);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCharacter::SwitchToLastWeapon()
{
    ChangeCurrentWeapon(LastWeapon);
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
        if (!CurrentWeapon)
        {
            CurrentWeapon = WeaponList["GravityGun"];
        }

        // super power
        if (!IsSuperPowerEquipped("TheWorld"))
        {
            PickUpSuperPower(GetWorld()->SpawnActor<AQLSuperPowerTheWorld>(AQLSuperPowerTheWorld::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator));
        }
        if (!CurrentSuperPower)
        {
            CurrentSuperPower = SuperPowerList["TheWorld"];
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
            ChangeCurrentWeapon(Weapon);

            // set logical ownership
            Weapon->SetQLOwner(this);
            AddToInventory(Weapon);

            // physical attachment
            Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

            PlaySoundComponent("EquipWeapon");
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
void AQLCharacter::PlaySoundFireAndForget(const FName& SoundName, const FVector& Location)
{
    QLUtility::PlaySoundFireAndForget(this->GetWorld(),
        FireAndForgetSoundWaveList,
        SoundName,
        Location,
        SoundAttenuation);
}

//------------------------------------------------------------
// note: ConstructorHelpers::FObjectFinder<T> and
// CreateDefaultSubobject<T> can only be used inside ctor!!!
//------------------------------------------------------------
UAudioComponent* AQLCharacter::CreateSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName)
{
    ConstructorHelpers::FObjectFinder<USoundWave> soundWave(soundPath);
    UAudioComponent* soundComp = CreateDefaultSubobject<UAudioComponent>(soundName);

    bool success = false;
    if (soundWave.Object->IsValidLowLevel() && soundComp)
    {
        soundComp->SetSound(soundWave.Object);
        soundComp->SetupAttachment(RootComponent);
        soundComp->SetRelativeLocation(FVector(0.0f));
        soundComp->bAutoActivate = false;
        soundComp->AdjustAttenuation(SoundAttenuation->Attenuation);
        success = true;
    }

    if (!success)
    {
        QLUtility::QLSay(TEXT("AQLActor::CreateSoundComponent() failed."));
        soundComp = nullptr;
    }

    return soundComp;
}

//------------------------------------------------------------
//------------------------------------------------------------
USoundWave* AQLCharacter::CreateFireAndForgetSoundWave(const TCHAR* SoundPath, const TCHAR* SoundName)
{
    ConstructorHelpers::FObjectFinder<USoundWave> SoundWaveObj(SoundPath);

    if (SoundWaveObj.Object->IsValidLowLevel())
    {
        return SoundWaveObj.Object;
    }
    else
    {
        QLUtility::QLSay(TEXT("CreateFireAndForgetSoundWave() failed."));
        return nullptr;
    }
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