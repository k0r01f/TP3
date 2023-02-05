// Fill out your copyright notice in the Description page of Project Settings.


#include "ChallengeCollectable.h"
#include "ChallengeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


AChallengeCollectable::AChallengeCollectable(const FObjectInitializer& ObjectInitializer)
{
	bReplicates = true;
}


void AChallengeCollectable::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (StageIndex == 0)
	{
		StartChallenge(0);
	}
	else
	{
		HideCollectable();
	}
}


void AChallengeCollectable::HideCollectable()
{
	bIsChallengeStarted = false;

	GetStaticMeshComponent()->SetVisibility(false); // wait challenge start event before to show collectable
}


void AChallengeCollectable::StartChallenge(int32 Stage)
{
	if (StageIndex == Stage)
	{
		bIsChallengeStarted = true;

		GetStaticMeshComponent()->SetVisibility(true);

		USphereComponent* CollisionComponent = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));
		if (CollisionComponent != nullptr)
		{
			CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AChallengeCollectable::OnSphereBeginOverlap);
		}

		AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode != nullptr)
		{
			GameMode->OnGameOver.AddDynamic(this, &AChallengeCollectable::StopChallenge);
		}
	}
	else if (StageIndex == 0)
	{
		HideCollectable();
	}
}


void AChallengeCollectable::CollectCoin_Implementation(APlayerState* PlayerState)
{
	AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		UStaticMeshComponent* StaticMeshComp = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if (StaticMeshComp != nullptr)
		{
			GameMode->OnChallengeCollected.Broadcast(StaticMeshComp);
		}
	}
}


void AChallengeCollectable::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsChallengeStarted)
	{
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		APlayerState* PlayerState = (PlayerPawn == nullptr ? nullptr : PlayerPawn->GetPlayerState());

		if(PlayerState != nullptr)
		{
			HideCollectable();

			CollectCoin(PlayerState);

			AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GameMode != nullptr)
			{
				GameMode->OnGameOver.RemoveDynamic(this, &AChallengeCollectable::StopChallenge);
			}
		}
	}
}


void AChallengeCollectable::StopChallenge(bool IsSucceed)
{
	if (bIsChallengeStarted)
	{
		HideCollectable();

		AChallengeGameMode* GameMode = Cast<AChallengeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode != nullptr)
		{
			GameMode->OnGameOver.RemoveDynamic(this, &AChallengeCollectable::StopChallenge);
		}
	}
}


void AChallengeCollectable::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChallengeCollectable, bIsChallengeStarted);
}


void AChallengeCollectable::ToggleCollectable_Implementation()
{
	GetStaticMeshComponent()->SetVisibility(bIsChallengeStarted);
}