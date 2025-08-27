#pragma once

#include <vector>

// A* ��ã�� �˰����� ó���ϴ� Ŭ����
class Node;
class AStar
{
	// ���� ó���� ���� ����ü
	struct Direction
	{
		// ��ġ
		int x = 0;
		int y = 0;

		// �̵� ���
		float cost = 0.0f;
	};

public:
	AStar();
	~AStar();

	// ��� Ž�� �Լ�
	std::vector<Node*> FindPath(
		Node* startNode,
		Node* goalNode,
		std::vector<std::vector<int*>>& grid
	);

public:
	inline bool GetIsFindDestination() const { return isFindDestination; }

	void ResetAStar();
	void ResetOpenClosedList(std::vector<Node*> path);

private:

	// Ž���� ��ģ �Ŀ� ��θ� ������ ��ȯ�ϴ� �Լ�
	// ��ǥ ��忡�� �θ� ��带 ������ ���� ������ ������ �ϸ鼭 ��θ� ����
	std::vector<Node*> ConstructPath(Node* goalNode);

	// Ž���Ϸ��� ��尡 ��ǥ ������� Ȯ��
	bool IsDestination(const Node* node);

	// �׸��� �ȿ� �ִ��� Ȯ��
	bool IsInRange(
		int x,
		int y,
		const std::vector<std::vector<int*>>& grid
	);

	// �̹� �湮�ߴ��� Ȯ���ϴ� �Լ�
	bool HasVisited(int x, int y, float gCost);

	// ���� �������� ��ǥ ���������� ���� ��� ��� �Ϲ�
	float CalculateHeuristic(Node* currentNode, Node* goalNode);

private:
	// ���� ����Ʈ (�湮�� ����� ���)
	std::vector<Node*> openList;

	// ���� ����Ʈ (�湮�� ����� ���)
	std::vector<Node*> closedList;

	// ���� ���
	Node* startNode = nullptr;

	// ��ǥ ���
	Node* goalNode = nullptr;

	// ��� ã�� �� ó�� openlist �� ���� ��� ����
	bool isAddStartNode = false;
	bool isFindDestination = false;
};
