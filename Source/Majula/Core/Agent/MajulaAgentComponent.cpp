// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Core/Agent/MajulaAgentComponent.h"

#include "GameFramework/Controller.h"
#include "Majula/Core/Agent/MajulaAgentStrategy.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

UMajulaAgentComponent::UMajulaAgentComponent()
{
    SetIsReplicatedByDefault(true);
    bReplicateUsingRegisteredSubObjectList = true;
}

void UMajulaAgentComponent::BeginPlay()
{
    Super::BeginPlay();

    // Must Attach to Controller
    auto* Controller = Cast<AController>(GetOwner());
    if (ensure(Controller))
    {
        APawn* Pawn = Controller->GetPawn();
        if (Pawn)
        {
            HandlePawnChanged(nullptr, Pawn);
        }
        Controller->OnPossessedPawnChanged.AddUniqueDynamic(this, &UMajulaAgentComponent::HandlePawnChanged);
    }
}

void UMajulaAgentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    FDoRepLifetimeParams Params;
    Params.bIsPushBased = true;
    Params.Condition = COND_OwnerOnly;
    DOREPLIFETIME_WITH_PARAMS(ThisClass, TeamStrategy, Params);
}

void UMajulaAgentComponent::OnRep_TeamStrategy_Implementation(UMajulaAgentStrategy* OldTeamStrategy)
{
}

void UMajulaAgentComponent::SetTeamStrategy(UMajulaAgentStrategy* NewTeamStrategy)
{
    if (NewTeamStrategy->GetClass() == TeamStrategy.GetClass())
    {
        return;
    }

    AddReplicatedSubObject(NewTeamStrategy);
    COMPARE_ASSIGN_AND_MARK_PROPERTY_DIRTY(ThisClass, TeamStrategy, NewTeamStrategy, this);
    PostStrategyChanged.Broadcast();
}

void UMajulaAgentComponent::HandlePawnChanged(APawn* OldPawn, APawn* NewPawn)
{
    Avatar = NewPawn;
}
