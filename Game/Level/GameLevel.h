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

	// 시점 함수
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

private:
	// 현재 레벨 grid 정보
	GridMap* grid;

	// 커서 위치
	Vector2 playerCursor;

	// 시작 노드, 도착 노드
	Vector2 startPos;
	Vector2 goalPos;

	// 현재 길찾기 진행 여부
	bool isFindingPath = false;
	bool isMoveToPath = false;


};