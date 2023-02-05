// Fill out your copyright notice in the Description page of Project Settings.

#include "ChallengeGameMode.h"
#include "UObject/ConstructorHelpers.h"


AChallengeGameMode::AChallengeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GoalCount = MAX_int32;
}


void AChallengeGameMode::InitChallenge_Implementation(int32 Count)
{
	CurrentCount = 0;

	GoalCount = Count;
}


void AChallengeGameMode::OnCollected(UStaticMeshComponent* StaticMeshComponent)
{ 
	if (StaticMeshComponent != nullptr)
	{
		if (++CurrentCount >= GoalCount)
		{
			OnGameOver.Broadcast(true);
		}
	}
}
