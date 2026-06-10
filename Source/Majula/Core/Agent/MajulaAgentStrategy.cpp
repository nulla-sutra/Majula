// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Core/Agent/MajulaAgentStrategy.h"

#include "Engine/BlueprintGeneratedClass.h"

void UMajulaAgentStrategy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

    if (const auto BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
    {
        BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
    }
}

FMajulaTeamId UMajulaAgentStrategy::ResolveTeamId_Implementation(const AController* const Controller) const
{
    return FMajulaTeamId::NoTeam;
}
