#pragma once

#include "Actor/Actor.h"
#include <vector>
#include <list>

// A*
// 그리드 정보를 저장하고 맵을 그릴 Actor

class Node;
class GridMap : public Actor
{
	RTTI_DECLARATIONS(GridMap, Actor)

public:
	GridMap();
	GridMap(int width, int height);
	~GridMap();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;


public:
	// Getter
	inline std::vector<std::vector<int*>>& GetGridInfo() { return grid; }
	inline std::list<Node*>& GetPurposeNode() { return purposeNode; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	// 벽 설정
	void SetWall(Vector2 pos);

	// 벽 리셋
	void WallReset();

	// 목표 위치 설정
	void SetPurposeNode(Vector2 pos);

	// 목표 위치 리셋
	void PurPoseNodeReset();

	void ResetSettings(bool isSafeDelete);

public:
	void SetIsDraw(bool isDraw) { isDrawList = isDraw; }
	

private:
	// Astar 돌릴 필드
	// 0 : 빈칸
	// -1 : 벽
	// 1~10 : 목표 위치 (1은 시작 > 2부터 목적지)
	// 20 : 열린 리스트
	// 21 : 닫힌 리스트
	std::vector<std::vector<int*>> grid;

	// 그리드 크기
	int width = 20;
	int height = 20;

	// 최대 목표 노드 수
	int maxPurposeNode = 10;

	// 목표 노드 변경 여부
	bool isNodeChange = false;

	// 목표 노드들을 관리할 리스트
	// head 부터 이동
	std::list<Node*> purposeNode;

	bool isDrawList = false;
};
