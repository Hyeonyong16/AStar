#pragma once

#include <vector>

template<typename T>
void SafeDelete(T*& pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer = nullptr;
	}
}

// A* 길찾기 알고리즘을 처리하는 클래스
class Node;
class AStar
{
	// 방향 처리를 위한 구조체
	struct Direction
	{
		// 위치
		int x = 0;
		int y = 0;

		// 이동 비용
		float cost = 0.0f;
	};

public:
	AStar();
	~AStar();

	// 경로 탐색 함수
	std::vector<Node*> FindPath(
		Node* startNode,
		Node* goalNode,
		std::vector<std::vector<int>>& grid
	);

	// 그리드 출력 함수.
	void DisplayGridWithPath(
		std::vector<std::vector<int>>& grid,
		const std::vector<Node*>& path
	);

private:

	// 탐색을 마친 후에 경로를 조립해 반환하는 함수
	// 목표 노드에서 부모 노드를 참조해 시작 노드까지 역추적 하면서 경로를 구함
	std::vector<Node*> ConstructPath(Node* goalNode);

	// 탐색하려는 노드가 목표 노드인지 확인
	bool IsDestination(const Node* node);

	// 그리드 안에 있는지 확인
	bool IsInRange(
		int x,
		int y,
		const std::vector<std::vector<int>>& grid
	);

	// 이미 방문했는지 확인하는 함수
	bool HasVisited(int x, int y, float gCost);

	// 현재 지점에서 목표 지점까지의 추정 비용 계산 하무
	float CalculateHeuristic(Node* currentNode, Node* goalNode);

	void DisplayGrid(std::vector<std::vector<int>>& grid);

	void DisplayOpenList();
	void DisplayCurNode(Node* currentNode);

private:
	// 열린 리스트 (방문할 노드의 목록)
	std::vector<Node*> openList;

	// 닫힌 리스트 (방문한 노드의 목록)
	std::vector<Node*> closedList;

	// 시작 노드
	Node* startNode = nullptr;

	// 목표 노드
	Node* goalNode = nullptr;
};
