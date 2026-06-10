// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "Majula/Helpers/MajulaLibrary.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Majula/Core/Agent/MajulaAgentStrategy.h"
#include "Majula/Core/Agent/MajulaTeamAgentInterface.h"

FMajulaTeamId UMajulaLibrary::GetMajulaTeamId(const APawn* const Pawn)
{
    const auto Controller = Cast<IMajulaTeamAgentInterface>(Pawn->GetController());

    if (!Controller)
    {
        return FMajulaTeamId::NoTeam;
    }

    return Controller->GetAgentStrategy()->ResolveTeamId(Pawn->GetController());
}

FMajulaTeamId UMajulaLibrary::MakeMajulaTeamIdFromPlayerController(const APlayerController* const PC)
{
    const auto PlayerState = PC->GetPlayerState<APlayerState>();
    if (!PlayerState)
    {
        return FMajulaTeamId::NoTeam;
    }
    return FMajulaTeamId(PC->GetPlayerState<APlayerState>()->GetPlayerId());
}

AActor* UMajulaLibrary::GetZoneActor(const TScriptInterface<IMajulaZoneInterface>& Interface)
{
    return Cast<AActor>(Interface.GetObject());
}
