#pragma once

#include "Engine.h"
#include <vector>
#include <utility>

class Level;
class Game : public Engine
{
public:
	Game();
	~Game();

	// �޴� ��ȯ �Լ�.
	void GoMenu();

	// �� �����͸� ������ �ش� ������ �̵�
	void GoLevel();

	virtual void CleanUp() override;
	static Game& Get();

public:
	
private:
	void AddLevelToVector(Level* _level, const char* _mapFile);

private:
	Level* menuLevel = nullptr;
	Level* astarLevel = nullptr;

	bool isLevelChangePreviousFrame = false;

	static Game* instance;

	std::vector<std::pair<const char*, Level*>> loadedLevels;
};