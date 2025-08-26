#include "GridMap.h"
#include "Core/Engine.h"
#include "Utils/Node.h"

#include <string>


GridMap::GridMap()
{
	for (int i = 0; i < height; ++i)
	{
		std::vector<int*> temp;
		for (int j = 0; j < width; ++j)
		{
			temp.emplace_back(new int(0));
		}
		grid.emplace_back(temp);
	}
}

GridMap::GridMap(int width, int height)
	: width(width), height(height)
{
	for (int i = 0; i < height; ++i)
	{
		std::vector<int*> temp;
		for (int j = 0; j < width; ++j)
		{
			temp.emplace_back(new int(0));
		}
		grid.emplace_back(temp);
	}
}

GridMap::~GridMap()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			SafeDelete(grid[i][j]);
		}
	}
}

void GridMap::BeginPlay()
{
	super::BeginPlay();
}

void GridMap::Tick(float _deltaTime)
{
	super::Tick(_deltaTime);

	if (isNodeChange)
	{
		std::list<Node*>::iterator iter = purposeNode.begin();
		int i = 1;
		for (; iter != purposeNode.end(); ++iter)
		{
			// grid 에서 Node 위치에 list 순서를 입력 (1 부터 시작)
			*(grid[(*iter)->position.y][(*iter)->position.x]) = i;

			// 삽입 후 i 증가
			++i;
		}

		isNodeChange = false;
	}
}

void GridMap::Render()
{
	// 엔진이 관리하는 이미지 버퍼에 액터의 문자열/색상 기록.
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			// 벽
			if (*(grid[i][j]) == -1)
			{
				Engine::Get().WriteToBuffer(Vector2(j + 1, i + 1), "#", Color::Blue);
			}

			// 빈 칸
			else if (*(grid[i][j]) == 0)
			{
				Engine::Get().WriteToBuffer(Vector2(j + 1, i + 1), " ", Color::Blue);
			}

			// 목표 노드들 
			else if (*(grid[i][j]) >= 1 && *(grid[i][j]) <= 10)
			{
				Engine::Get().WriteToBuffer(Vector2(j + 1, i + 1)
					, std::to_string(*(grid[i][j])).c_str()
					, Color::Red
				);
			}
		}
	}
}

void GridMap::SetWall(Vector2 pos)
{
	// 입력받은 좌표가 목표노드이면 그냥 반환
	if (*(grid[pos.y][pos.x]) != 0 && *(grid[pos.y][pos.x]) != -1)
	{
		return;
	}

	// 입력받은 좌표가 벽이면 벽 제거
	if (*(grid[pos.y][pos.x]) == -1)
	{
		*(grid[pos.y][pos.x]) = 0;
		return;
	}

	// 입력받은 좌표가 벽이 아니면 벽 설정
	*(grid[pos.y][pos.x]) = -1;
}

// 현재 그리드의 벽 제거
void GridMap::WallReset()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (*(grid[i][j]) == -1)
			{
				*(grid[i][j]) = 0;
			}
		}
	}
}

void GridMap::SetPurposeNode(Vector2 pos)
{
	// 입력받은 좌표가 벽이면 그냥 반환
	if (*(grid[pos.y][pos.x]) == -1)
	{
		return;
	}

	// 이미 목표 노드이면 해당 노드 제거
	if (*(grid[pos.y][pos.x]) >= 1 && *(grid[pos.y][pos.x]) <= 10)
	{
		std::list<Node*>::iterator iter = purposeNode.begin();
		int i = 1;
		for (; iter != purposeNode.end(); ++iter)
		{
			// 현재 위치 노드 순서 값과 노드 비교
			if (i == *(grid[pos.y][pos.x]))
			{
				// 기존 그리드 값을 빈 값으로 만들어 줌
				*(grid[pos.y][pos.x]) = 0;

				// 목표 노드 제거
				purposeNode.erase(iter);
				break;
			}

			// 아니면 i값 증가
			++i;
		}

		// 노드 순서가 바뀌었으므로 다시 그림
		isNodeChange = true;
		return;
	}


	// 설정할려는 최대 목표 노드수보다 많으면 반환
	if (purposeNode.size() >= maxPurposeNode)
	{
		return;
	}


	// 현재 커서 위치로 노드 생성
	Node* curNode = new Node(pos.x, pos.y);

	// 목표 노드 리스트에 생성한 노드 추가
	purposeNode.emplace_back(curNode);

	// 노드 변경 여부 설정
	isNodeChange = true;
}

void GridMap::PurPoseNodeReset()
{

}
