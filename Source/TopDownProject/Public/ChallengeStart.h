// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/StaticMeshActor.h"
#include "Components/SphereComponent.h"
#include "ChallengeStart.generated.h"

#pragma once

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChallengeOver, bool, IsSuccess);

/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWNPROJECT_API AChallengeStart : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AChallengeStart(const FObjectInitializer& ObjectInitializer);

	/** First stage index of the challenge */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Challenge")
	int32 StageMinimun;

	/** Last stage index of the challenge */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Challenge")
	int32 StageMaximun;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(BlueprintAssignable, Category = "Challenge")
	FOnChallengeOver OnChallengeOver;

protected:

	UFUNCTION(Server, Reliable)
	void OnChallengeRequested();

	UFUNCTION(Client, Reliable)
	void TriggerChallenge(int32 Stage);

	UFUNCTION()
	void OnChallengeOver_Internal(bool IsSucceed);

	/** Native implementation of the blueprint event */
	void OnChallengeOver_Implementation(bool IsSucceed);

	virtual void BeginPlay() override;

	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

private:
	void OnChallengeRequested_Implementation();

	void TriggerChallenge_Implementation(int32 Stage);

	bool bIsChallengeStarted;

	int32 CurrentStage;
};
