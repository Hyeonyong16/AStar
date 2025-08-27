#include "AStar.h"
#include "Node.h"
#include "Core/Engine.h"

#include <Windows.h>

AStar::AStar()
{
}

AStar::~AStar()
{
	//CheckDebug();
	// �޸� ����
	for (Node* node : openList)
	{
		SafeDelete(node);
	}
	openList.clear();

	for (Node* node : closedList)
	{
		SafeDelete(node);
	}
	closedList.clear();
}

std::vector<Node*> AStar::FindPath(Node* startNode, Node* goalNode, std::vector<std::vector<int*>>& grid)
{
	// ���� ��� / ��ǥ ��� ����
	this->startNode = startNode;
	this->goalNode = goalNode;

	// ���� ��带 ���� ���(openlist)�� ����
	if(isAddStartNode == false)
	{
		openList.emplace_back(startNode);
		isAddStartNode = true;
	}

	// �����¿�, �밢�� ���� �� ���
	std::vector<Direction> directions = {
		// �ϻ���� �̵�
		{ 0, 1, 1.0f }, { 0, -1, 1.0f },
		{ 1, 0, 1.0f }, { -1, 0, 1.0f },

		// �밢�� �̵�
		{ 1, 1, 1.414f }, { 1, -1, 1.414f },
		{ -1, 1, 1.414f }, {-1, -1, 1.414f },
	};

	// �湮
	//while (!openList.empty())
	if(!openList.empty())
	{
		// ���� ����� ���� ��� ���� (���� Ž������ �˻�)
		// ����ȭ ��� -> ���� ���� ���� ������ ã�� �� �ֵ���
		Node* lowestNode = openList[0];

		for (Node* node : openList)
		{
			if (node->fCost < lowestNode->fCost)
			{
				lowestNode = node;
			}
		}

		// fCost �� ���� ���� ��带 ���� ���� ����
		Node* currentNode = lowestNode;

		// ���� ��尡 ��ǥ ������� Ȯ��
		if (IsDestination(currentNode))
		{
			// ��ǥ �����, ���ݱ����� ��θ� ����ؼ� ��ȯ
			isFindDestination = true;
			// ��θ� ã�� ���¿��� �ٽ� ��� ��
			// ������� �ѹ� �� �ִ� �� ����
			if(closedList[closedList.size() - 1] != goalNode)
				closedList.emplace_back(currentNode);
			return ConstructPath(currentNode);
		}

		// ���� ��Ͽ� �߰� (���� ��Ͽ����� ����)
		for (int i = 0; i < (int)openList.size(); ++i)
		{
			// ��ġ ��
			if (*openList[i] == *currentNode)
			{
				// iterator �� Ȱ���� ���� �迭���� ��� ����
				openList.erase(openList.begin() + i);

				// ���� ����Ʈ == 21
				*grid[currentNode->position.y][currentNode->position.x] = 21;
				break;
			}
		}

		// ���� ��带 ���� ��Ͽ� �߰�
		// �̹� ������ �߰� ���ϰ�, ������ �߰�
		bool isNodeInList = false;
		for (Node* node : closedList)
		{
			// ��ġ ��
			if (*node == *currentNode)
			{
				//if (node != currentNode)
				//{
					//SafeDelete(currentNode);
				//}
				isNodeInList = true;
				break;
			}
		}

		// �湮 ������ �Ʒ� �ܰ� �ǳʶٱ�
		if (isNodeInList)
		{
			return std::vector<Node*>();
		}

		// ��Ͽ� �߰�
		closedList.emplace_back(currentNode);

		// �̿���� �湮
		for (const Direction& direction : directions)
		{
			// ������ �̵��� ��ġ ����
			int newX = currentNode->position.x + direction.x;
			int newY = currentNode->position.y + direction.y;

			// �׸��� ������ Ȯ��
			if (!IsInRange(newX, newY, grid))
			{
				// �׸��� ���̸� ����
				continue;
			}

			// (�ɼ�) ��ֹ����� Ȯ��
			// ���� -1�̸� ��ֹ��̶�� ���
			if (*grid[newY][newX] == -1)
			{
				continue;
			}

			// �̹� �湮 �߾ ����
			// �̹� �湮�ߴ��� Ȯ���ϴ� �Լ� ȣ��
			float gCost = currentNode->gCost + direction.cost;
			if (HasVisited(newX, newY, gCost))
			{
				continue;
			}

			// �湮�� ���� ��� ����
			// ��뵵 ���
			Node* neighborNode = new Node(newX, newY, currentNode);
			//debugList.push_back(neighborNode);
			neighborNode->gCost = currentNode->gCost + direction.cost;

			/*if (newX == 5 && newY == 1)
			{
				int a = 0;
			}
			if (newX == 6 && newY == 1)
			{
				int a = 0;
			}
			if (newX == 7 && newY == 1)
			{
				int a = 0;
			}*/


			// �޸���ƽ ��� �Լ� ȣ��
			neighborNode->hCost = CalculateHeuristic(neighborNode, goalNode);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			// �̿� ��尡 ���� ����Ʈ�� �ִ��� Ȯ��
			Node* openListNode = nullptr;
			for (Node* node : openList)
			{
				if (*node == *neighborNode)
				{
					openListNode = node;
					break;
				}
			}

			// ��尡 ��Ͽ� ���ų� ����� �θ�, �� ��� �߰�
			if (openListNode == nullptr
				|| openListNode->gCost > neighborNode->gCost
				|| openListNode->fCost > neighborNode->fCost)
			{
				// ���¸���Ʈ = 5
				*grid[newY][newX] = 20;

				openList.emplace_back(neighborNode);
			}
			else
			{
				SafeDelete(neighborNode);
			}
		}
	}

	return std::vector<Node*>();
}

void AStar::ResetAStar(bool isFirstDelete)
{
	for (Node* node : openList)
	{
		SafeDelete(node);
	}
	openList.clear();

	int i = 0;
	for (Node* node : closedList)
	{
		if (isFirstDelete && i == 0)
		{
			++i;
			continue;
		}
		SafeDelete(node);
	}
	closedList.clear();

	isAddStartNode = false;
	isFindDestination = false;

	startNode = nullptr;
	goalNode = nullptr;
}

void AStar::ResetOpenClosedList(std::vector<Node*> path)
{
	for (Node* node : openList)
	{
		bool isDelete = true;
		for(int i = 0; i < path.size(); ++i)
		{
			if (path[i] == node)
			{
				isDelete = false;
				break;
			}
		}
		if(isDelete == true)
			SafeDelete(node);
	}
	openList.clear();

	for (Node* node : closedList)
	{
		bool isDelete = true;
		for (int i = 0; i < path.size(); ++i)
		{
			if (path[i] == node)
			{
				isDelete = false;
				break;
			}
		}
		if (isDelete == true)
			SafeDelete(node);
	}
	closedList.clear();

	isAddStartNode = false;
	isFindDestination = false;
}

//void AStar::CheckDebug()
//{
//	int a = debugList.size();
//}

std::vector<Node*> AStar::ConstructPath(Node* goalNode)
{
	// ��� ��ȯ

	// ��¿� ��� �迭 ����
	std::vector<Node*> path;
	Node* currentNode = goalNode;
	while (currentNode != nullptr)
	{
		path.emplace_back(currentNode);
		currentNode = currentNode->parent;
	}

	// ���ݱ����� ��δ� ��ǥ -> ���� ��� ����
	// ���� �� ������ ������� ��
	std::reverse(path.begin(), path.end());

	return path;
}

bool AStar::IsDestination(const Node* node)
{
	// ��尡 ��ǥ ���� ��ġ�� ������ ��
	return *node == *goalNode;
}

bool AStar::IsInRange(int x, int y, const std::vector<std::vector<int*>>& grid)
{
	// x, y ������ ����� false
	if (x < 0 || y < 0 || x >= (int)grid[0].size() || y >= (int)grid.size())
	{
		return false;
	}

	// ����� �ʾ����� true
	return true;
}

bool AStar::HasVisited(int x, int y, float gCost)
{
	// ���� ����Ʈ�� ���� ����Ʈ�� �̹� �ش� ��尡 ������ �湮�� ������ �Ǵ�
	for (int i = 0; i < (int)openList.size(); ++i)
	{
		Node* node = openList[i];
		if (node->position.x == x && node->position.y == y)
		{
			// ��ġ�� ����, ��뵵 �� ũ�� �湮�� ���� ����
			if (node->gCost <= gCost)
			{
				return true;
			}
			else if (node->gCost > gCost)
			{
				openList.erase(openList.begin() + i);
				SafeDelete(node);
			}
		}
	}

	for (int i = 0; i < (int)closedList.size(); ++i)
	{
		Node* node = closedList[i];
		if (node->position.x == x && node->position.y == y)
		{
			// ��ġ�� ����, ��뵵 ������ �湮�� ���� ����
			if (node->gCost <= gCost)
			{
				return true;
			}
			//��ġ�� ������ ����� �۴ٸ�, ���� ��� ����
			else if (node->gCost > gCost)
			{
				closedList.erase(closedList.begin() + i);
				SafeDelete(node);
			}
		}
	}

	// ��Ͽ� ���ٰ� �ǴܵǸ� �湮���� ���� ������ �Ǵ�
	return false;
}

float AStar::CalculateHeuristic(Node* currentNode, Node* goalNode)
{
	// �ܼ� �Ÿ� ������� �޸���ƽ ������� Ȱ��
	Position diff = *currentNode - *goalNode;

	// �밢�� ���� ���ϱ�
	return (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
}
