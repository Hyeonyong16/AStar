#include "AStar.h"
#include "Node.h"

#include <Windows.h>

AStar::AStar()
{
}

AStar::~AStar()
{
	// 메모리 해제
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

std::vector<Node*> AStar::FindPath(Node* startNode, Node* goalNode, std::vector<std::vector<int>>& grid)
{
	// 시작 노드 / 목표 노드 저장
	this->startNode = startNode;
	this->goalNode = goalNode;

	// 시작 노드를 열린 목록(openlist)에 저장
	openList.emplace_back(startNode);

	// 상하좌우, 대각선 방향 및 비용
	std::vector<Direction> directions = {
		// 하상우좌 이동
		{ 0, 1, 1.0f }, { 0, -1, 1.0f },
		{ 1, 0, 1.0f }, { -1, 0, 1.0f },

		// 대각선 이동
		{ 1, 1, 1.414f }, { 1, -1, 1.414f },
		{ -1, 1, 1.414f }, {-1, -1, 1.414f },
	};

	// 방문
	while (!openList.empty())
	{
		// 가장 비용이 작은 노드 선택 (선형 탐색으로 검색)
		// 최적화 기능 -> 가장 작은 값을 빠르게 찾을 수 있도록
		Node* lowestNode = openList[0];

		for (Node* node : openList)
		{
			if (node->fCost < lowestNode->fCost)
			{
				lowestNode = node;
			}
		}

		// fCost 가 가장 낮은 노드를 현재 노드로 설정
		Node* currentNode = lowestNode;

		DisplayCurNode(currentNode);

		// 현재 노드가 목표 노드인지 확인
		if (IsDestination(currentNode))
		{
			// 목표 노드라면, 지금까지의 경로를 계산해서 반환
			return ConstructPath(currentNode);
		}

		// 닫힌 목록에 추가 (열린 목록에서는 제거)
		for (int i = 0; i < (int)openList.size(); ++i)
		{
			// 위치 비교
			if (*openList[i] == *currentNode)
			{
				// iterator 를 활용해 동적 배열에서 노드 제거
				openList.erase(openList.begin() + i);
				grid[currentNode->position.y][currentNode->position.x] = 6;
				break;
			}
		}

		// 현재 노드를 닫힌 목록에 추가
		// 이미 있으면 추가 안하고, 없으면 추가
		bool isNodeInList = false;
		for (Node* node : closedList)
		{
			// 위치 비교
			if (*node == *currentNode)
			{
				isNodeInList = true;
				break;
			}
		}

		// 방문 했으면 아래 단계 건너뛰기
		if (isNodeInList)
		{
			continue;
		}

		// 목록에 추가
		closedList.emplace_back(currentNode);

		// 이웃노드 방문
		for (const Direction& direction : directions)
		{
			// 다음에 이동할 위치 설정
			int newX = currentNode->position.x + direction.x;
			int newY = currentNode->position.y + direction.y;

			// 그리드 밖인지 확인
			if (!IsInRange(newX, newY, grid))
			{
				// 그리드 밖이면 무시
				continue;
			}

			// (옵션) 장애물인지 확인
			// 값이 1이면 장애물이라고 약속
			if (grid[newY][newX] == 1)
			{
				continue;
			}

			// 이미 방문 했어도 무시
			// 이미 방문했는지 확인하는 함수 호출
			float gCost = currentNode->gCost + direction.cost;
			if (HasVisited(newX, newY, gCost))
			{
				continue;
			}

			// 방문을 위한 노드 생성
			// 비용도 계산
			Node* neighborNode = new Node(newX, newY, currentNode);
			neighborNode->gCost = currentNode->gCost + direction.cost;

			// 휴리스틱 계산 함수 호출
			neighborNode->hCost = CalculateHeuristic(neighborNode, goalNode);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			// 이웃 노드가 열린 리스트에 있는지 확인
			Node* openListNode = nullptr;
			for (Node* node : openList)
			{
				if (*node == *neighborNode)
				{
					openListNode = node;
					break;
				}
			}

			// 노드가 목록에 없거나 비용이 싸면, 새 노드 추가
			if (openListNode == nullptr
				|| openListNode->gCost > neighborNode->gCost
				|| openListNode->fCost > neighborNode->fCost)
			{
				// 오픈리스트 = 5
				grid[newY][newX] = 5;

				openList.emplace_back(neighborNode);
			}
			else
			{
				SafeDelete(neighborNode);
			}

			DisplayGrid(grid);
			//DisplayClosedList();

			/*int delay = (int)(0.1f * 1000);
			Sleep(delay);*/
		}
	}

	return std::vector<Node*>();
}

void AStar::DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Node*>& path)
{
	static COORD position = { 0, 2 };
	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

	int white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	int green = FOREGROUND_GREEN;

	// 구분을 위해 설정했던 데이터 초기화.
	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].size(); ++x)
		{
			int& value = grid[y][x];
			if (value == 2 || value == 3 || value == 5 || value == 6)
			{
				value = 0;
			}
		}
	}

	// 경로를 제외한 맵 출력.
	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].size(); ++x)
		{
			// 장애물.
			if (grid[y][x] == 1)
			{
				SetConsoleTextAttribute(handle, white);
				std::cout << "1 ";
			}


			// 빈 공간.
			else if (grid[y][x] == 0)
			{
				SetConsoleTextAttribute(handle, white);
				std::cout << "0 ";
			}
		}

		std::cout << "\n";
	}

	// 경로 출력.
	for (const Node* node : path)
	{
		// 경로는 '2'로 표시.
		COORD position{ static_cast<short>(node->position.x * 2), static_cast<short>(node->position.y) + 2 };
		SetConsoleCursorPosition(handle, position);
		SetConsoleTextAttribute(handle, green);

		std::cout << "* ";
		/*int delay = static_cast<int>(0.05f * 1000);
		Sleep(delay);*/
	}
}

std::vector<Node*> AStar::ConstructPath(Node* goalNode)
{
	// 경로 반환

	// 출력용 경로 배열 선언
	std::vector<Node*> path;
	Node* currentNode = goalNode;
	while (currentNode != nullptr)
	{
		path.emplace_back(currentNode);
		currentNode = currentNode->parent;
	}

	// 지금까지의 경로는 목표 -> 시작 노드 방향
	// 따라서 이 순서를 뒤집어야 함
	std::reverse(path.begin(), path.end());

	return path;
}

bool AStar::IsDestination(const Node* node)
{
	// 노드가 목표 노드와 위치가 같은지 비교
	return *node == *goalNode;
}

bool AStar::IsInRange(int x, int y, const std::vector<std::vector<int>>& grid)
{
	// x, y 범위가 벗어나면 false
	if (x < 0 || y < 0 || x >= (int)grid[0].size() || y >= (int)grid.size())
	{
		return false;
	}

	// 벗어나지 않았으면 true
	return true;
}

bool AStar::HasVisited(int x, int y, float gCost)
{
	// 열린 리스트나 닫힌 리스트에 이미 해당 노드가 있으면 방문한 것으로 판단
	for (int i = 0; i < (int)openList.size(); ++i)
	{
		Node* node = openList[i];
		if (node->position.x == x && node->position.y == y)
		{
			// 위치가 같고, 비용도 더 크면 방문할 이유 없음
			if (node->gCost < gCost)
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
			// 위치가 같고, 비용도 높으면 방문할 이유 없음
			if (node->gCost < gCost)
			{
				return true;
			}
			//위치는 같으나 비용이 작다면, 기존 노드 제거
			else if (node->gCost > gCost)
			{
				closedList.erase(closedList.begin() + i);
				SafeDelete(node);
			}
		}
	}

	// 목록에 없다고 판단되면 방문하지 않은 것으로 판단
	return false;
}

float AStar::CalculateHeuristic(Node* currentNode, Node* goalNode)
{
	// 단순 거리 계산으로 휴리스틱 비용으로 활용
	Position diff = *currentNode - *goalNode;

	// 대각선 길이 구하기
	return (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

void AStar::DisplayGrid(std::vector<std::vector<int>>& grid)
{
	static COORD position = { 0, 2 };
	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

	int white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	int wow = FOREGROUND_RED | FOREGROUND_GREEN;
	int green = FOREGROUND_GREEN;

	for (int y = 0; y < grid.size(); ++y)
	{
		for (int x = 0; x < grid[0].size(); ++x)
		{
			// 시작 위치.
			if (grid[y][x] == 2)
			{
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				std::cout << "S ";
			}

			// 목표 위치.
			if (grid[y][x] == 3)
			{
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				std::cout << "G ";
			}

			// 장애물.
			if (grid[y][x] == 1)
			{
				SetConsoleTextAttribute(handle, white);
				std::cout << "1 ";
			}

			// 열린 리스트
			else if (grid[y][x] == 5)
			{
				SetConsoleTextAttribute(handle, green);
				std::cout << "+ ";
			}

			// 닫힌 리스트
			else if (grid[y][x] == 6)
			{
				SetConsoleTextAttribute(handle, wow);
				std::cout << "+ ";
			}

			// 빈 공간.
			else if (grid[y][x] == 0)
			{
				SetConsoleTextAttribute(handle, white);
				std::cout << "0 ";
			}
		}

		std::cout << "\n";
	}
}

void AStar::DisplayCurNode(Node* currentNode)
{
	static COORD position = { 0, 0 };
	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);

	std::cout << "                            ";
	SetConsoleCursorPosition(handle, position);

	std::vector<Node*>::iterator iter = openList.begin();

	std::cout << "curNode: " << "{ "
		<< currentNode->position.x << ", "
		<< currentNode->position.y << " } ";

}

