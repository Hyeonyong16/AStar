#pragma once
#include "Level/Level.h"
#include "Actor/Actor.h"
#include <vector>

class GridMap;
class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

	// ���� �Լ�
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

private:
	// ���� ���� grid ����
	GridMap* grid;

	// Ŀ�� ��ġ
	Vector2 playerCursor;

	// ���� ���, ���� ���
	Vector2 startPos;
	Vector2 goalPos;

	// ���� ��ã�� ���� ����
	bool isFindingPath = false;
	bool isMoveToPath = false;


};