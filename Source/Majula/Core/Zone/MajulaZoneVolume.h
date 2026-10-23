// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Majula/Core/Zone/MajulaZoneInterface.h"
#include "GameFramework/Volume.h"
#include "StructUtils/InstancedStruct.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif

#include "MajulaZoneVolume.generated.h"

class UMajulaZoneRule;

UCLASS(Blueprintable)
class MAJULA_API AMajulaZoneVolume : public AVolume, public IMajulaZoneInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMajulaZoneVolume();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
#if UE_WITH_IRIS
    virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
                                              const UE::Net::EFragmentRegistrationFlags RegistrationFlags) override
    {
        UE::Net::FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
    };
#endif

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="MajulaZone", Replicated,
        meta=(ExposeOnSpawn, BaseStruct="/Script/Majula.MajulaZoneContext"))
    FInstancedStruct ZoneSettings = FInstancedStruct::Make(FMajulaZoneContext());

    virtual bool ValidTest_Implementation(const APawn* const Pawn) const override;
    virtual FMajulaZoneContext GetZoneContext_Implementation() const override;
};

inline FMajulaZoneContext AMajulaZoneVolume::GetZoneContext_Implementation() const
{
    return ZoneSettings.Get<FMajulaZoneContext>();
}
