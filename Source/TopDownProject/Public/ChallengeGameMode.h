// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "ChallengeGameMode.generated.h"

// Declaration of the delegate that will be called when a challenge is requested
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChallengeRequested);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollected, UStaticMeshComponent*, StaticMeshComponent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, IsSuccess);

/**
 * 
 */
UCLASS()
class TOPDOWNPROJECT_API AChallengeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AChallengeGameMode();

	/** Event sent when a challenge start */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Challenge")
	FOnChallengeRequested OnChallengeRequested;

	UPROPERTY(BlueprintAssignable, Category = "Challenge")
    FOnCollected OnChallengeCollected;

	UPROPERTY(BlueprintAssignable, Category = "Challenge")
	FOnGameOver OnGameOver;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) //peu importe le nombre de fois que je build il faut toujours le refaire...
	void InitChallenge(int32 Count);

	void InitChallenge_Implementation(int32 Count);

protected:

	UFUNCTION()
	void OnCollected(UStaticMeshComponent* StaticMeshComponent);

	int32 CurrentCount;
	int32 GoalCount;
};
