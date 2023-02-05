// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerState.h"
#include "ChallengeCollectable.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNPROJECT_API AChallengeCollectable : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AChallengeCollectable(const FObjectInitializer& ObjectInitializer);

	/** Last stage index of the challenge */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Challenge")
	int32 StageIndex;

	UFUNCTION(BlueprintCallable)
	void StartChallenge(int32 Stage);

	UFUNCTION(BlueprintCallable)
	void StopChallenge(bool IsSucceed);

	bool IsChallengeStarted() const { return bIsChallengeStarted; }

	UFUNCTION(BlueprintNativeEvent)
	void CollectCoin(APlayerState* PlayerState);

	void CollectCoin_Implementation(APlayerState* PlayerState);

protected:
	virtual void BeginPlay() override;

	void HideCollectable();

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = ToggleCollectable)
	bool bIsChallengeStarted;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void ToggleCollectable();
};
