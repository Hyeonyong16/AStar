#include "Game.h"
#include "Level/GameLevel.h"
#include "Level/MenuLevel.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;
	// ���� �߰�.
	menuLevel = new MenuLevel();

	AddLevel(menuLevel);
}

Game::~Game()
{
	CleanUp();
}

void Game::GoMenu()
{
	// ȭ�� ����.
	// clear screen ��ɾ� ����.
	system("cls");

	// �޴� ������ ���� ������ ����.
	mainLevel = menuLevel;

	// ���� ���� ����
	isLevelChangePreviousFrame = true;
}

void Game::GoLevel(const char* _map)
{
	// ȭ�� ����.
	// clear screen ��ɾ� ����.
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


// �������� �����ϱ� ���� ����
void Game::AddLevelToVector(Level* _level, const char* _mapFile)
{
	loadedLevels.emplace_back(std::make_pair(_mapFile, _level));
}
