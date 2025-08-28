#include "Game.h"
#include "Level/GameLevel.h"
#include "Level/MenuLevel.h"
#include "Level/RedBlackTreeLevel.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;
	// ���� �߰�.
	menuLevel = new MenuLevel();
	astarLevel = new GameLevel();
	rbtLevel = new RedBlackTreeLevel();
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

void Game::GoLevel(LevelType type)
{
	// ȭ�� ����.
	// clear screen ��ɾ� ����.
	system("cls");

	// �޴� ������ ���� ������ ����.
	if (type == LevelType::AStar)
		mainLevel = astarLevel;

	else if (type == LevelType::RedBlackTree)
		mainLevel = rbtLevel;

	// ���� ���� ����
	isLevelChangePreviousFrame = true;
}

void Game::CleanUp()
{
	for (std::pair<const char*, Level*> level : loadedLevels)
	{
		SafeDelete(level.second);
	}

	SafeDelete(menuLevel);
	SafeDelete(astarLevel);
	SafeDelete(rbtLevel);
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
