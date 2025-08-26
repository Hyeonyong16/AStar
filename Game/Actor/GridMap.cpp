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
			// grid ���� Node ��ġ�� list ������ �Է� (1 ���� ����)
			*(grid[(*iter)->position.y][(*iter)->position.x]) = i;

			// ���� �� i ����
			++i;
		}

		isNodeChange = false;
	}
}

void GridMap::Render()
{
	// ������ �����ϴ� �̹��� ���ۿ� ������ ���ڿ�/���� ���.
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			// ��
			if (*(grid[i][j]) == -1)
			{
				Engine::Get().WriteToBuffer(Vector2(j + 1, i + 1), "#", Color::Blue);
			}

			// �� ĭ
			else if (*(grid[i][j]) == 0)
			{
				Engine::Get().WriteToBuffer(Vector2(j + 1, i + 1), " ", Color::Blue);
			}

			// ��ǥ ���� 
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
	// �Է¹��� ��ǥ�� ��ǥ����̸� �׳� ��ȯ
	if (*(grid[pos.y][pos.x]) != 0 && *(grid[pos.y][pos.x]) != -1)
	{
		return;
	}

	// �Է¹��� ��ǥ�� ���̸� �� ����
	if (*(grid[pos.y][pos.x]) == -1)
	{
		*(grid[pos.y][pos.x]) = 0;
		return;
	}

	// �Է¹��� ��ǥ�� ���� �ƴϸ� �� ����
	*(grid[pos.y][pos.x]) = -1;
}

// ���� �׸����� �� ����
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
	// �Է¹��� ��ǥ�� ���̸� �׳� ��ȯ
	if (*(grid[pos.y][pos.x]) == -1)
	{
		return;
	}

	// �̹� ��ǥ ����̸� �ش� ��� ����
	if (*(grid[pos.y][pos.x]) >= 1 && *(grid[pos.y][pos.x]) <= 10)
	{
		std::list<Node*>::iterator iter = purposeNode.begin();
		int i = 1;
		for (; iter != purposeNode.end(); ++iter)
		{
			// ���� ��ġ ��� ���� ���� ��� ��
			if (i == *(grid[pos.y][pos.x]))
			{
				// ���� �׸��� ���� �� ������ ����� ��
				*(grid[pos.y][pos.x]) = 0;

				// ��ǥ ��� ����
				purposeNode.erase(iter);
				break;
			}

			// �ƴϸ� i�� ����
			++i;
		}

		// ��� ������ �ٲ�����Ƿ� �ٽ� �׸�
		isNodeChange = true;
		return;
	}


	// �����ҷ��� �ִ� ��ǥ �������� ������ ��ȯ
	if (purposeNode.size() >= maxPurposeNode)
	{
		return;
	}


	// ���� Ŀ�� ��ġ�� ��� ����
	Node* curNode = new Node(pos.x, pos.y);

	// ��ǥ ��� ����Ʈ�� ������ ��� �߰�
	purposeNode.emplace_back(curNode);

	// ��� ���� ���� ����
	isNodeChange = true;
}

void GridMap::PurPoseNodeReset()
{

}
