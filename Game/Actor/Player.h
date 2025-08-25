#pragma once

#include "Actor/Actor.h"
#include "Utils/AStar.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

private:


};