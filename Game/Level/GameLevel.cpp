#include "GameLevel.h"

#include "Engine.h"
#include "Game/Game.h"

#include <iostream>


GameLevel::GameLevel()
{
	for (int i = 0; i < height; ++i)
	{
		std::vector<int> temp;
		for (int j = 0; j < width; ++j)
		{
			temp.emplace_back(new int(0));
		}
		grid.emplace_back(temp);
	}
}

GameLevel::GameLevel(int width, int height)
	: width(width), height(height)
{
	for (int i = 0; i < height; ++i)
	{
		std::vector<int> temp;
		for (int j = 0; j < width; ++j)
		{
			temp.emplace_back(new int(0));
		}
		grid.emplace_back(temp);
	}
}

GameLevel::~GameLevel()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			SafeDelete(grid[i][j]);
		}
	}
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
}

void GameLevel::Tick(float _deltaTime)
{
	super::Tick(_deltaTime);

	if (Input::Get().GetKeyDown(VK_UP))


}

void GameLevel::Render()
{
	super::Render();

	
}

