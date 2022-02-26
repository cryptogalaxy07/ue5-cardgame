﻿#include "CardGameMode.h"

#include "CardGameCardPile.h"
#include "CardGameLogCategory.h"
#include "CardGamePlayerController.h"
#include "CardGamePlayerState.h"

ACardGameMode::ACardGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerStateClass = ACardGamePlayerState::StaticClass();
}

FCardGameModel& ACardGameMode::GetModel()
{
	return Model;
}

void ACardGameMode::SetPlayerReady(AController* Player)
{
	if (!IsValid(Player))
	{
		return;
	}

	ACardGamePlayerState* PlayerState = Player->GetPlayerState<ACardGamePlayerState>();

	if (!IsValid(PlayerState) || PlayerState->IsReady())
	{
		return;
	}

	PlayerState->SetReady();

	UE_LOG(LogCardGame, Log, TEXT("%s is ready."), *PlayerState->GetName());
	
	// Check if all players ready.
	int32 ReadyPlayers = 0;

	for (ACardGamePlayerState* P : Players)
	{
		if (IsValid(P) && P->IsReady())
		{
			++ReadyPlayers;
		}
	}

	if (ReadyPlayers >= NumPlayers)
	{
		UE_LOG(LogCardGame, Log, TEXT("All %d player(s) are ready, starting game."), NumPlayers);
		
		NotifyOnPreStartGame();

		// Start game.
		for (ACardGamePlayerState* P : Players)
		{
			ACardGamePlayerController* PC = Cast<ACardGamePlayerController>(P->GetOwner());

			if (!IsValid(PC))
			{
				continue;
			}

			PC->ClientGameStarted(Model);
		}
	}
}

FString ACardGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
                                     const FString& Options, const FString& Portal)
{
	FString ErrorMessage = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	
	if (IsValid(NewPlayerController))
	{
		ACardGamePlayerState* NewPlayer = NewPlayerController->GetPlayerState<ACardGamePlayerState>();

		if (IsValid(NewPlayer))
		{
			// Find first available player index.
			int32 NewPlayerIndex = 0;
			bool bPlayerIndexInUse = true;
			
			while (bPlayerIndexInUse)
			{
				bPlayerIndexInUse = IsPlayerIndexInUse(NewPlayerIndex);

				if (bPlayerIndexInUse)
				{
					++NewPlayerIndex;
				}
			}

			// Set player index.
			NewPlayer->SetPlayerIndex(NewPlayerIndex);

			UE_LOG(LogCardGame, Log, TEXT("Set player index of player %s to %d."), *NewPlayer->GetName(), NewPlayerIndex);
			
			// Store player reference.
			Players.Add(NewPlayer);

			// Add player to model.
			const TArray<UCardGameCardPile*> PlayerCardPileClasses = GetCardPileClassesByScope(ECardGameScope::Player);
			Model.AddPlayer(NewPlayerIndex, PlayerCardPileClasses);
		}
	}

	return ErrorMessage;
}

void ACardGameMode::NotifyOnPreStartGame()
{
	// Add global card piles.
	const TArray<UCardGameCardPile*> GlobalCardPileClasses = GetCardPileClassesByScope(ECardGameScope::Global);
	
	for (UCardGameCardPile* CardPileClass : GlobalCardPileClasses)
	{
		Model.AddGlobalCardPile(CardPileClass);
	}

	// Notify listeners.
	ReceiveOnPreStartGame();
}

TArray<UCardGameCardPile*> ACardGameMode::GetCardPileClassesByScope(ECardGameScope Scope) const
{
	TArray<UCardGameCardPile*> CardPileClassesByScope;

	for (UCardGameCardPile* CardPileClass : CardPileClasses)
	{
		if (CardPileClass->GetScope() == Scope)
		{
			CardPileClassesByScope.Add(CardPileClass);
		}
	}

	return CardPileClassesByScope;
}

bool ACardGameMode::IsPlayerIndexInUse(int32 PlayerIndex) const
{
	for (ACardGamePlayerState* ExistingPlayer : Players)
	{
		if (IsValid(ExistingPlayer) && ExistingPlayer->GetPlayerIndex() == PlayerIndex)
		{
			return true;
		}
	}

	return false;
}
