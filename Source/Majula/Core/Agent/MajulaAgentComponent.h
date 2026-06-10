// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/Object.h"
#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif
#include "MajulaAgentComponent.generated.h"

class UMajulaAgentStrategy;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMajulaAgentStrategyChanged);

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent), ClassGroup="Majula")
class MAJULA_API UMajulaAgentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMajulaAgentComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
#if UE_WITH_IRIS
    virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
                                              const UE::Net::EFragmentRegistrationFlags RegistrationFlags) override
    {
        UE::Net::FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
    };
#endif

    //
    UPROPERTY(BlueprintReadOnly, EditAnywhere,
        ReplicatedUsing="OnRep_TeamStrategy", Category="Majula", Instanced, NoClear)
    TObjectPtr<UMajulaAgentStrategy> TeamStrategy;

    UFUNCTION(BlueprintNativeEvent)
    void OnRep_TeamStrategy(UMajulaAgentStrategy* OldTeamStrategy);

    UFUNCTION(BlueprintCallable, Category = "Majula", BlueprintAuthorityOnly)
    void SetTeamStrategy(UMajulaAgentStrategy* NewTeamStrategy);

    UPROPERTY(BlueprintAssignable, Category="Majula", BlueprintAuthorityOnly)
    FMajulaAgentStrategyChanged PostStrategyChanged;

protected:
    UFUNCTION()
    void HandlePawnChanged(APawn* OldPawn, APawn* NewPawn);

    UPROPERTY(Transient)
    TObjectPtr<APawn> Avatar;
};
