// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Presets/MajulaZoneVolume_Deferred.h"

#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

bool FMajulaDeferredZoneDwellSet::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    Ar << Version;
    auto Count = LocalPawns.Num();
    Ar << Count;

    if (Ar.IsSaving())
    {
        for (auto&& LocalPawn : LocalPawns)
        {
            UObject* PawnPtr = LocalPawn.Get();
            bOutSuccess &= Map->SerializeObject(Ar, LocalPawn->GetClass(), PawnPtr);
        }
    }

    if (Ar.IsLoading())
    {
        LocalPawns.Reset();
        while (Count-- > 0)
        {
            UObject* PawnPtr = nullptr;
            bOutSuccess &= Map->SerializeObject(Ar, APawn::StaticClass(), PawnPtr);
            LocalPawns.Add(Cast<APawn>(PawnPtr));
        }
    }

    return bOutSuccess;
}

bool FMajulaDeferredZoneDwellSet::operator==(const FMajulaDeferredZoneDwellSet& Other) const
{
    return Version == Other.Version;
}

// Sets default values
AMajulaZoneVolume_Deferred::AMajulaZoneVolume_Deferred()
{
    PrimaryActorTick.bCanEverTick = false;
    ZoneSettings.GetMutable<FMajulaZoneContext>().bUnbound = false;
}

// Called when the game starts or when spawned
void AMajulaZoneVolume_Deferred::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        OnActorBeginOverlap.AddUniqueDynamic(this, &AMajulaZoneVolume_Deferred::HandleActorBeginOverlap);
        OnActorEndOverlap.AddUniqueDynamic(this, &AMajulaZoneVolume_Deferred::HandleActorEndOverlap);
    }
}

void AMajulaZoneVolume_Deferred::BeginDestroy()
{
    Super::BeginDestroy();
    for (auto&& TimerHandle : TimerHandles)
    {
        TimerHandle.Value.Invalidate();
    }
    TimerHandles.Empty();
}

void AMajulaZoneVolume_Deferred::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    constexpr FDoRepLifetimeParams Params{
        .Condition = COND_SimulatedOnly,
        .RepNotifyCondition = REPNOTIFY_OnChanged,
        .bIsPushBased = true
    };
    // DOREPLIFETIME_WITH_PARAMS(ThisClass, DwellSet, Params);
    DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, LocalPawns, Params);
}

bool AMajulaZoneVolume_Deferred::ValidTest_Implementation(const APawn* const Pawn) const
{
    // return DwellSet.LocalPawns.Contains(Pawn);
    return LocalPawns.Contains(Pawn);
}

void AMajulaZoneVolume_Deferred::HandleActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (HasAuthority())
    {
        auto* Pawn = Cast<APawn>(OtherActor);
        if (Pawn)
        {
            FTimerHandle TimerHandle = TimerHandles.FindOrAdd(Pawn);
            GetWorldTimerManager().SetTimer(TimerHandle, [this,Pawn]()
            {
                // DwellSet.LocalPawns.Add(Pawn);
                // DwellSet.Version++;
                // MARK_PROPERTY_DIRTY_FROM_NAME(AMajulaZoneVolume_Deferred, DwellSet, this);

                LocalPawns.AddUnique(Pawn);
                MARK_PROPERTY_DIRTY_FROM_NAME(AMajulaZoneVolume_Deferred, LocalPawns, this);
            }, DelayTime, false);
        }
    }
}

void AMajulaZoneVolume_Deferred::HandleActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (ensure(HasAuthority()))
    {
        auto* Pawn = Cast<APawn>(OtherActor);
        if (Pawn)
        {
            auto* PawnHandle = TimerHandles.Find(Pawn);
            if (PawnHandle)
            {
                PawnHandle->Invalidate();
                TimerHandles.Remove(Pawn);
            }
            // DwellSet.LocalPawns.Remove(Pawn);
            // DwellSet.Version++;
            // MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DwellSet, this);

            LocalPawns.Remove(Pawn);
            MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, LocalPawns, this);
        }
    }
}
