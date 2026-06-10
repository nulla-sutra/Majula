// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Core/Agent/MajulaTeamAgentInterface.h"

#include "GameFramework/Controller.h"
#include "Majula/Core/Agent/MajulaAgentComponent.h"
#include "Majula/Core/Agent/MajulaAgentStrategy.h"


UMajulaAgentComponent* IMajulaTeamAgentInterface::GetTeamAgentComponent() const
{
    const auto Controller = Cast<AController>(_getUObject());
    if (Controller == nullptr)
    {
        checkNoEntry();
        return nullptr;
    }
    return Controller->GetComponentByClass<UMajulaAgentComponent>();
}

UMajulaAgentStrategy* IMajulaTeamAgentInterface::GetAgentStrategy() const
{
    const auto* Comp = GetTeamAgentComponent();
    if (Comp == nullptr)
    {
        return nullptr;
    }
    return Comp->TeamStrategy;
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void IMajulaTeamAgentInterface::SetAgentStrategy(UMajulaAgentStrategy* AgentStrategy)
{
    auto* Comp = GetTeamAgentComponent();
    if (Comp == nullptr)
    {
        return;
    }
    Comp->SetTeamStrategy(AgentStrategy);
}

