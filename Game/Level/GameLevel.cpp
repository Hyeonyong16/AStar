#include "GameLevel.h"

#include "Engine.h"
#include "Game/Game.h"
#include "Actor/GridMap.h"

#include <iostream>


GameLevel::GameLevel()
{
	grid = new GridMap(20, 20);
	AddActor(grid);

	playerCursor = Vector2(0, 0);

	startPos = Vector2(0, 0);
	goalPos = Vector2(0, 0);
}

GameLevel::~GameLevel()
{
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
}

void GameLevel::Tick(float _deltaTime)
{
	super::Tick(_deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().GoMenu();
	}

	// 키입력
	// Todo: GetKey 함수로 변경 및 타이머 추가해서 이동속도 조절
	if (Input::Get().GetKeyDown(VK_UP))
	{
		if(playerCursor.y > 0)
			--playerCursor.y;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		if(playerCursor.y < grid->GetHeight() - 1)
			++playerCursor.y;
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		if (playerCursor.x > 0)
			--playerCursor.x;
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		if (playerCursor.x < grid->GetWidth() - 1)
			++playerCursor.x;
	}

	// 벽 생성
	if (Input::Get().GetKeyDown('W'))
	{
		grid->SetWall(playerCursor);
	}

	// 벽 초기화
	if (Input::Get().GetKeyDown('R'))
	{
		grid->WallReset();
	}

	if (Input::Get().GetKeyDown('S'))
	{
		grid->SetPurposeNode(playerCursor);
	}
}

void GameLevel::Render()
{
	super::Render();

	for (int i = 0; i <= grid->GetHeight() + 1; ++i)
	{
		for (int j = 0; j <= grid->GetWidth() + 1; ++j)
		{
			if (i == 0 || i == grid->GetHeight() + 1 || j == 0 || j == grid->GetWidth() + 1)
			{
				Engine::Get().WriteToBuffer(Vector2(j, i), "#", Color::Blue);
			}
		}
	}

	if (isFindingPath == false && isMoveToPath == false)
	{
		Engine::Get().WriteToBuffer(Vector2(playerCursor.x + 1, playerCursor.y + 1), "O", Color::White);
	}
	
}

