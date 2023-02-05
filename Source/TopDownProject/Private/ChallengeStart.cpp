// Fill out your copyright notice in the Description page of Project Settings.


#include "ChallengeStart.h"
#include "ChallengeGameMode.h"
#include "ChallengeCollectable.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/SceneViewport.h"
#include "Slate/WidgetRenderer.h"


AChallengeStart::AChallengeStart(const FObjectInitializer& ObjectInitializer) 
	: AStaticMeshActor(ObjectInitializer)
{
	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ChallengeSphereCollision"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetupAttachment(RootComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AChallengeStart::OnSphereBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AChallengeStart::OnSphereEndOverlap);
}


void AChallengeStart::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	bIsChallengeStarted = false;
}


void AChallengeStart::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->OnChallengeRequested.AddDynamic(this, &AChallengeStart::OnChallengeRequested);
	}
}


void AChallengeStart::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->OnChallengeRequested.RemoveDynamic(this, &AChallengeStart::OnChallengeRequested);
	}
}


void AChallengeStart::OnChallengeRequested_Implementation()
{
	if (bIsChallengeStarted)
	{
		return;
	}

	bIsChallengeStarted = true;

	AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode != nullptr)
	{
		GameMode->OnChallengeRequested.RemoveDynamic(this, &AChallengeStart::OnChallengeRequested);

		GameMode->OnGameOver.AddDynamic(this, &AChallengeStart::OnChallengeOver_Internal);
	}

	CurrentStage = StageMinimun;

	TriggerChallenge(CurrentStage);
}


void AChallengeStart::OnChallengeOver_Implementation(bool IsSucceed)
{
	// TODO prompt game over dialog
}


void AChallengeStart::OnChallengeOver_Internal(bool IsSucceed)
{
	AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		if (IsSucceed)
		{
			if (++CurrentStage > StageMaximun)
			{
				GameMode->OnGameOver.RemoveDynamic(this, &AChallengeStart::OnChallengeOver_Internal);

				bIsChallengeStarted = false;

				OnChallengeOver.Broadcast(true); // you win the challenge

				TriggerChallenge(0); // reset initial state of the level
			}
			else
			{
				TriggerChallenge(CurrentStage);
			}
		}
		else
		{
			GameMode->OnGameOver.RemoveDynamic(this, &AChallengeStart::OnChallengeOver_Internal);

			bIsChallengeStarted = false;

			OnChallengeOver.Broadcast(false); // you lose the challenge

			TriggerChallenge(0); // reset initial state of the level
		}
	}
}


void AChallengeStart::TriggerChallenge_Implementation(int32 Stage)
{
	int32 CollectableCount = 0; // count the number of collectable during that stage

	TArray<AActor*> CollectableActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChallengeCollectable::StaticClass(), CollectableActors);
	for (AActor* Actor : CollectableActors)
	{
		AChallengeCollectable* CollectableActor = Cast<AChallengeCollectable>(Actor);

		CollectableActor->StartChallenge(Stage);

		if (CollectableActor->IsChallengeStarted())
		{
			++CollectableCount;
		}
	}

	AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->InitChallenge(CollectableCount);
	}
}
