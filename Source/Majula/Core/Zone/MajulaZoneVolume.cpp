// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Core/Zone/MajulaZoneVolume.h"

#include "Components/BrushComponent.h"
#include "Engine/CollisionProfile.h"
#include "Majula/Framework/MajulaSubsystem.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMajulaZoneVolume::AMajulaZoneVolume()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    auto* CollisionShape = GetBrushComponent();
    CollisionShape->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
    CollisionShape->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
    CollisionShape->SetCollisionObjectType(ECC_WorldStatic);
    CollisionShape->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionShape->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionShape->Mobility = EComponentMobility::Static;

    bReplicates = true;
    bNetLoadOnClient = true;
    SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

// Called when the game starts or when spawned
void AMajulaZoneVolume::BeginPlay()
{
    Super::BeginPlay();


    if (HasAuthority())
    {
        if (ZoneSettings.Get<FMajulaZoneContext>().bUnbound)
        {
            UMajulaSubsystem::Get(this)->RegisterUnboundZone(this);
        }
    }
}


void AMajulaZoneVolume::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    constexpr FDoRepLifetimeParams Params{
        .Condition = COND_SimulatedOnly,
        .RepNotifyCondition = REPNOTIFY_OnChanged,
        .bIsPushBased = true,
    };
    DOREPLIFETIME_WITH_PARAMS(ThisClass, ZoneSettings, Params);
}

bool AMajulaZoneVolume::ValidTest_Implementation(const APawn* const Pawn) const
{
    return true;
}


#if WITH_EDITOR
void AMajulaZoneVolume::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
