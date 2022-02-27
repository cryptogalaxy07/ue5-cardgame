﻿#pragma once

#include "CoreMinimal.h"
#include "Assets/CardGameConfiguration.h"
#include "GameFramework/GameModeBase.h"

#include "Model/CardGameModel.h"
#include "Model/CardGameScope.h"

#include "CardGameMode.generated.h"

class AController;

class UCardGameCard;
class UCardGameCardPile;
class ACardGamePlayerState;

UCLASS()
class CARDGAME_API ACardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACardGameMode(const FObjectInitializer& ObjectInitializer);

	FCardGameModel& GetModel();

	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
		const FString& Options, const FString& Portal) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UCardGameConfiguration* Configuration;
	
	UPROPERTY()
	FCardGameModel Model;
	
	UPROPERTY()
	TArray<ACardGamePlayerState*> Players;
};
