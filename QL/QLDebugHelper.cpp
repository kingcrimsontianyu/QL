//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLDebugHelper.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLDebugHelper::AQLDebugHelper()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(1.0f, 1.0f, 1.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    XAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("XAxis"));
    static const ConstructorHelpers::FObjectFinder<UStaticMesh> XAxisObj(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
    XAxis->SetStaticMesh(XAxisObj.Object);
    XAxis->SetSimulatePhysics(false);
    XAxis->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    XAxis->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    XAxis->SetupAttachment(RootComponent);
    XAxis->SetWorldScale3D(FVector(0.2f, 0.2f, 1.0f));
    XAxis->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    XAxis->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
    static ConstructorHelpers::FObjectFinder<UMaterial> XAxisMaterialObj(TEXT("/Game/Materials/DebugHelper/M_QLDebugHelperRed"));
    if (XAxisMaterialObj.Succeeded())
    {
        UMaterial* XAxiMaterial = XAxisMaterialObj.Object;
        XAxis->SetMaterial(0, XAxiMaterial);
    }
    XAxisLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("XAxisLabel"));
    XAxisLabel->SetupAttachment(XAxis);
    XAxisLabel->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    XAxisLabel->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
    XAxisLabel->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    XAxisLabel->SetText(FText::FromString("X"));
    XAxisLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);

    YAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YAxis"));
    static const ConstructorHelpers::FObjectFinder<UStaticMesh> YAxisObj(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
    YAxis->SetStaticMesh(YAxisObj.Object);
    YAxis->SetSimulatePhysics(false);
    YAxis->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    YAxis->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    YAxis->SetupAttachment(RootComponent);
    YAxis->SetWorldScale3D(FVector(0.2f, 0.2f, 1.0f));
    YAxis->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    YAxis->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
    static ConstructorHelpers::FObjectFinder<UMaterial> YAxisMaterialObj(TEXT("/Game/Materials/DebugHelper/M_QLDebugHelperGreen"));
    if (YAxisMaterialObj.Succeeded())
    {
        UMaterial* XAxiMaterial = YAxisMaterialObj.Object;
        YAxis->SetMaterial(0, XAxiMaterial);
    }
    YAxisLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("YAxisLabel"));
    YAxisLabel->SetupAttachment(YAxis);
    YAxisLabel->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    YAxisLabel->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
    YAxisLabel->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    YAxisLabel->SetText(FText::FromString("Y"));
    YAxisLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);

    ZAxis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZAxis"));
    static const ConstructorHelpers::FObjectFinder<UStaticMesh> ZAxisObj(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
    ZAxis->SetStaticMesh(ZAxisObj.Object);
    ZAxis->SetSimulatePhysics(false);
    ZAxis->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ZAxis->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    ZAxis->SetupAttachment(RootComponent);
    ZAxis->SetWorldScale3D(FVector(0.1f, 0.1f, 0.5f));
    ZAxis->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    ZAxis->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    static ConstructorHelpers::FObjectFinder<UMaterial> ZAxisMaterialObj(TEXT("/Game/Materials/DebugHelper/M_QLDebugHelperBlue"));
    if (ZAxisMaterialObj.Succeeded())
    {
        UMaterial* XAxiMaterial = ZAxisMaterialObj.Object;
        ZAxis->SetMaterial(0, XAxiMaterial);
    }
    ZAxisLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ZAxisLabel"));
    ZAxisLabel->SetupAttachment(ZAxis);
    ZAxisLabel->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    ZAxisLabel->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
    ZAxisLabel->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    ZAxisLabel->SetText(FText::FromString("Z"));
    ZAxisLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLDebugHelper::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLDebugHelper::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}