#pragma once

#include "Engine.h"
#include <vector>
#include <utility>

enum class LevelType
{
	Menu,
	AStar,
	RedBlackTree,
};

class Level;
class Game : public Engine
{
public:
	Game();
	~Game();

	// 메뉴 전환 함수.
	void GoMenu();

	// 맵 데이터를 넣으면 해당 레벨로 이동
	void GoLevel(LevelType type);

	virtual void CleanUp() override;
	static Game& Get();

public:
	
private:
	void AddLevelToVector(Level* _level, const char* _mapFile);

private:
	Level* menuLevel = nullptr;
	Level* astarLevel = nullptr;
	Level* rbtLevel = nullptr;

	bool isLevelChangePreviousFrame = false;

	static Game* instance;

	std::vector<std::pair<const char*, Level*>> loadedLevels;
};