#include "Game.h"
#include "Level/GameLevel.h"
#include "Level/MenuLevel.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;
	// 레벨 추가.
	menuLevel = new MenuLevel();

	AddLevel(menuLevel);
}

Game::~Game()
{
	CleanUp();
}

void Game::GoMenu()
{
	// 화면 정리.
	// clear screen 명령어 실행.
	system("cls");

	// 메뉴 레벨을 메인 레벨로 설정.
	mainLevel = menuLevel;

	// 기존 레벨 제거
	isLevelChangePreviousFrame = true;
}

void Game::GoLevel(const char* _map)
{
	// 화면 정리.
	// clear screen 명령어 실행.
	system("cls");

	for (int i = 0; i < loadedLevels.size(); ++i)
	{
		if (mainLevel != loadedLevels[i].second)
		{
			if (strcmp(loadedLevels[i].first, _map) == 0)
			{
				mainLevel = loadedLevels[i].second;
				curStage = i + 1;
				return;
			}
		}
	}
}

void Game::CleanUp()
{
	for (std::pair<const char*, Level*> level : loadedLevels)
	{
		SafeDelete(level.second);
	}

	SafeDelete(menuLevel);
	mainLevel = nullptr;

	loadedLevels.clear();



	Engine::CleanUp();
}

Game& Game::Get()
{
	return *instance;
}


// 레벨들을 관리하기 위한 벡터
void Game::AddLevelToVector(Level* _level, const char* _mapFile)
{
	loadedLevels.emplace_back(std::make_pair(_mapFile, _level));
}
