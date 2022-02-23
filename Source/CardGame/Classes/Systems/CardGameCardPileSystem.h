﻿#pragma once

#include "CoreMinimal.h"

#include "Model/CardGameCardModel.h"

#include "CardGameCardPileSystem.generated.h"

class UCardGameCard;
class FCardGameCardInstanceIdPool;
class UCardGameCardPile;
struct FCardGameCardPileModel;
struct FCardGameModel;
struct FCardGamePlayerModel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCardGameCardPileSystemCardAddedToGlobalCardPileSignature, UCardGameCardPile*, CardPileClass, FCardGameCardModel, Card);

class CARDGAME_API FCardGameCardPileSystem
{
public:
	explicit FCardGameCardPileSystem(FCardGameCardInstanceIdPool& CardInstanceIdPool, FRandomStream& RandomStream);
	
	void AddGlobalCardPile(FCardGameModel& Model, UCardGameCardPile* CardPileClass) const;
	void AddCardToGlobalCardPile(FCardGameModel& Model, UCardGameCardPile* CardPileClass,
		UCardGameCard* CardClass) const;
	void AddCardToPlayerCardPile(FCardGameModel& Model, int32 PlayerIndex, UCardGameCardPile* CardPileClass,
		UCardGameCard* CardClass) const;
	void ShuffleGlobalCardPile(FCardGameModel& Model, UCardGameCardPile* CardPileClass) const;
	void ShufflePlayerCardPile(FCardGameModel& Model, int32 PlayerIndex, UCardGameCardPile* CardPileClass) const;
	void MoveCardBetweenGlobalCardPiles(FCardGameModel& Model, UCardGameCardPile* From, UCardGameCardPile* To,
		int32 CardIndex) const;
	void MoveCardBetweenPlayerCardPiles(FCardGameModel& Model, int32 PlayerIndex, UCardGameCardPile* From,
		UCardGameCardPile* To, int32 CardIndex) const;

	FCardGameCardPileSystemCardAddedToGlobalCardPileSignature OnCardAddedToGlobalCardPile;

private:
	FCardGameCardInstanceIdPool& CardInstanceIdPool;
	FRandomStream& RandomStream;
	
	FCardGameCardPileModel* GetGlobalCardPile(FCardGameModel& Model, UCardGameCardPile* CardPileClass) const;
	FCardGameCardPileModel* GetPlayerCardPile(FCardGamePlayerModel& Player, UCardGameCardPile* CardPileClass) const;
	FCardGameCardModel AddCard(FCardGameCardPileModel& CardPile, int64 InstanceId, UCardGameCard* CardClass) const;
	void Shuffle(FCardGameCardPileModel& CardPile) const;
	FCardGameCardModel MoveCardBetweenPiles(FCardGameCardPileModel& From, FCardGameCardPileModel& To,
		int32 CardIndex) const;
};
