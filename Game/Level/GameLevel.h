#pragma once
#include "Level/Level.h"
#include <vector>

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	GameLevel(int width, int height);
	~GameLevel();

	// ���� �Լ�
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

private:
	// Astar ���� �ʵ�
	// 0 : ��ĭ
	// -1 : ��
	// 1~10 : ��ǥ ��ġ
	// 20 : ���� ����Ʈ
	// 21 : ���� ����Ʈ
	std::vector<std::vector<int*>> grid;

	// �ʵ� ũ��
	int width = 20;
	int height = 20;

	// Ŀ�� ��ġ
	Vector2 playerCursor;

	// ���� ��ã�� ���� ����
	bool isFindingPath = false;
	bool isMoveToPath = false;

};