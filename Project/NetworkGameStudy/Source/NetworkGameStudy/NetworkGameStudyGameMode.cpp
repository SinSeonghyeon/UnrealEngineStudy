// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkGameStudyGameMode.h"
#include "NetworkGameStudyCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANetworkGameStudyGameMode::ANetworkGameStudyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
