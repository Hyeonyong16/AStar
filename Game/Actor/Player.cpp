#include "Player.h"
#include "Core/Engine.h"

Player::Player()
	:Actor("P", Color::YellowIntensity)
{
	moveTimer.SetTargetTime(moveTime);
}

Player::~Player()
{
}

void Player::BeginPlay()
{
	super::BeginPlay();
}

void Player::Tick(float _deltaTime)
{
	super::Tick(_deltaTime);

	if (this->isDraw)
	{
		moveTimer.Tick(_deltaTime);
		if (moveTimer.IsTimeout())
		{
			isPlay = true;
		}
	}
}

void Player::Render()
{
	if (isDraw)
	{
		Engine::Get().WriteToBuffer(
			Vector2(position.x + 1, position.y + 1), 
			image, 
			color
		);
	}
	
}

// 이동 및 타이머 초기화
void Player::Move(Vector2 pos)
{
	SetPosition(pos);
	
	isPlay = false;
	moveTimer.Reset();
	moveTimer.SetTargetTime(moveTime);
}

void Player::ResetSettings()
{
	isDraw = false;
	isPlay = false;

	moveTimer.Reset();
	moveTimer.SetTargetTime(moveTime);
}