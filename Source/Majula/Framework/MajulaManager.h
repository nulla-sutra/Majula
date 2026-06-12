// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif
#include "MajulaManager.generated.h"

class IMajulaZoneInterface;

UCLASS(NotPlaceable, BlueprintInternalUseOnly)
class MAJULA_API AMajulaManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMajulaManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PostNetReceive() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
#if UE_WITH_IRIS
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
	                                          UE::Net::EFragmentRegistrationFlags RegistrationFlags) override
	{
		UE::Net::FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
	};
#endif

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> UnboundZones;
};
