#pragma once
#include "Level/Level.h"
#include "Actor/Actor.h"
#include "Utils/AStar.h"
#include "Utils/Timer.h"
#include <vector>

class Player;
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

public:
	void StartFindPath();
	void ResetSettings();

private:
	// ���� ���� grid ����
	GridMap* grid;

	// AStar ��ü
	AStar aStar;

	// Ŀ�� ��ġ
	Vector2 playerCursor;

	// ���� ���, ���� ���
	Vector2 startPos;
	Vector2 goalPos;

	// ���� ��ã�� ���� ����
	bool isFindingPath = false;
	bool isDrawingPath = false;
	bool isMoveToPath = false;

	bool isPlayingAnim = false;

	// ��� ����
	// ���� ��θ� �����ϱ� ���� 2�� vector ���
	std::vector<std::vector<Node*>> movePath;

	Timer renderTimer;
	float renderTargetTime = 0.15f;

	// ������ �÷��̾� ����
	Player* player;

	// ���� ��� Ž�� Ƚ��
	// �ʿ��� Ž�� Ƚ���� grid ���� purposeNode size �����ͼ� -1 
	int findNum = 0;

	// �ִϸ��̼� ��� ã�ư��� Ƚ��
	int animNum = 0;
};