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

	// 시점 함수
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

private:
	// Astar 돌릴 필드
	// 0 : 빈칸
	// -1 : 벽
	// 1~10 : 목표 위치
	// 20 : 열린 리스트
	// 21 : 닫힌 리스트
	std::vector<std::vector<int*>> grid;

	// 필드 크기
	int width = 20;
	int height = 20;

	// 커서 위치
	Vector2 playerCursor;

	// 현재 길찾기 진행 여부
	bool isFindingPath = false;
	bool isMoveToPath = false;

};