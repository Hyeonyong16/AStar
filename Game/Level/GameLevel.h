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

	// 시점 함수
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

public:
	void StartFindPath();
	void ResetSettings();

private:
	// 현재 레벨 grid 정보
	GridMap* grid;

	// AStar 객체
	AStar aStar;

	// 커서 위치
	Vector2 playerCursor;

	// 시작 노드, 도착 노드
	Vector2 startPos;
	Vector2 goalPos;

	// 현재 길찾기 진행 여부
	bool isFindingPath = false;
	bool isDrawingPath = false;
	bool isMoveToPath = false;

	bool isPlayingAnim = false;

	// 경로 저장
	// 여러 경로를 저장하기 위해 2중 vector 사용
	std::vector<std::vector<Node*>> movePath;

	// 화면에 탐색 애니메이션 재생될 타이머 
	Timer renderTimer;
	float renderTargetTime = 0.05f;

	// 방향 키 길게 입력 시 인식될 시간 타이머
	// 상 하 좌 우 순서
	Timer keyInputTimer[4];
	float keyInputTime = 0.5f;
	// 키 입력 시간
	float keyMoveCurDelay[4] = { };
	float keyMoveSpeed = 5.0f;
	float keyMoveDelay = 0.3f;

	// 움직일 플레이어 정보
	Player* player;

	// 목적 노드 탐색 횟수
	// 필요한 탐색 횟수는 grid 에서 purposeNode size 가져와서 -1 
	int findNum = 0;

	// 애니메이션 노드 찾아가는 횟수
	int animNum = 0;

	// 벽 설치 여부 확인용 bool
	bool isSetWall = false;

	bool isFailed= false;
};