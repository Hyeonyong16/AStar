#include "GameLevel.h"

#include "Engine.h"
#include "Game/Game.h"
#include "Utils/AStar.h"
#include "Utils/Node.h"
#include "Actor/GridMap.h"
#include "Actor/Player.h"

#include <iostream>


GameLevel::GameLevel()
{
	grid = new GridMap(20, 20);
	AddActor(grid);

	playerCursor = Vector2(0, 0);

	startPos = Vector2(0, 0);
	goalPos = Vector2(0, 0);

	renderTargetTime = 0.05f;
	renderTimer.SetTargetTime(renderTargetTime);

	player = new Player();
	AddActor(player);
}

GameLevel::~GameLevel()
{
	for (Node* node : movePath)
	{
		//SafeDeleteArray(node);
	}
	movePath.clear();
}

void GameLevel::BeginPlay()
{
	super::BeginPlay();
}

void GameLevel::Tick(float _deltaTime)
{
	super::Tick(_deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().GoMenu();
	}

	// Ű�Է�
	// Todo: GetKey �Լ��� ���� �� Ÿ�̸� �߰��ؼ� �̵��ӵ� ����
	if(isFindingPath == false)
	{
		if (Input::Get().GetKeyDown(VK_UP))
		{
			if (playerCursor.y > 0)
				--playerCursor.y;
		}

		if (Input::Get().GetKeyDown(VK_DOWN))
		{
			if (playerCursor.y < grid->GetHeight() - 1)
				++playerCursor.y;
		}

		if (Input::Get().GetKeyDown(VK_LEFT))
		{
			if (playerCursor.x > 0)
				--playerCursor.x;
		}

		if (Input::Get().GetKeyDown(VK_RIGHT))
		{
			if (playerCursor.x < grid->GetWidth() - 1)
				++playerCursor.x;
		}

		// �� ����
		if (Input::Get().GetKeyDown('W'))
		{
			grid->SetWall(playerCursor);
		}

		// �� �ʱ�ȭ
		if (Input::Get().GetKeyDown('R'))
		{
			grid->WallReset();
		}

		if (Input::Get().GetKeyDown('S'))
		{
			grid->SetPurposeNode(playerCursor);
		}

		// ���� �ʱ�ȭ
		if (Input::Get().GetKeyDown(VK_DELETE))
		{
			ResetSettings();
		}

		// ��ã�� ����
		if (Input::Get().GetKeyDown('T'))
		{
			// ��ǥ ��尡 2�� �̸� (�ƿ� ���ų� �������� ����)
			if (grid->GetPurposeNode().size() >= 2)
			{
				isFindingPath = true;
				grid->SetIsDraw(true);
			}
		}
	}

	else// (isFindingPath == true)
	{
		renderTimer.Tick(_deltaTime);
		if(renderTimer.IsTimeout())
		{
			StartFindPath();
			renderTimer.Reset();
			renderTimer.SetTargetTime(renderTargetTime);
		}
	}

	if (isDrawingPath)
	{
		// �÷��̾� ó�� Ȱ��ȭ ��
		if (player->GetIsDraw() == false)
		{
			// Draw Ȱ��ȭ
			player->SetIsDraw(true);
			
			// ��� ��ġ�� �̵� ��Ű��
			player->Move(Vector2(movePath[0]->position.x, movePath[0]->position.y));
		}

		if (player->GetIsPlay())
		{
			for (int i = 0; i < movePath.size(); ++i)
			{
				// ���� ��尡 �÷��̾� ��ġ�� ���� ��
				if (player->GetPosition().x == movePath[i]->position.x
					&& player->GetPosition().y == movePath[i]->position.y)
				{
					// ���� ���� �÷��̾ �̵� ��Ų��.
					if(i != movePath.size() - 1)
					{
						player->Move(Vector2(movePath[i + 1]->position.x, movePath[i + 1]->position.y));
						break;
					}
				}
			}

			player->SetIsPlay(false);
		}
	}
}

void GameLevel::Render()
{
	super::Render();

	for (int i = 0; i <= grid->GetHeight() + 1; ++i)
	{
		for (int j = 0; j <= grid->GetWidth() + 1; ++j)
		{
			if (i == 0 || i == grid->GetHeight() + 1 || j == 0 || j == grid->GetWidth() + 1)
			{
				Engine::Get().WriteToBuffer(Vector2(j, i), "#", Color::Blue);
			}
		}
	}

	if (isDrawingPath == true)
	{
		for (const Node* node : movePath)
		{
			if (node->position.x == player->GetPosition().x
				&& node->position.y == player->GetPosition().y)
				continue;
			Engine::Get().WriteToBuffer(Vector2(node->position.x + 1, node->position.y + 1), "*", Color::Yellow);
		}
	}

	if (isFindingPath == false && isMoveToPath == false)
	{
		Engine::Get().WriteToBuffer(Vector2(playerCursor.x + 1, playerCursor.y + 1), "O", Color::White);
	}
}

void GameLevel::StartFindPath()
{
	// ��ǥ ��尡 2�� �̸� (�ƿ� ���ų� �������� ����)
	if (grid->GetPurposeNode().size() < 2)
	{
		return;
	}

	// ��� Ž��.
	std::list<Node*>::iterator iter = grid->GetPurposeNode().begin();
	Node* startNode = *iter;
	++iter;
	Node* goalNode = *iter;

	std::vector<Node*> path = aStar.FindPath(startNode, goalNode, grid->GetGridInfo());

	if (aStar.GetIsFindDestination())
	{
		movePath = path;
		isDrawingPath = true;
		isFindingPath = false;

		grid->SetIsDraw(false);
	}
}

void GameLevel::ResetSettings()
{
	// Grid �ʱ�ȭ
	grid->ResetSettings(aStar.GetIsFindDestination());

	// Astar �ʱ�ȭ
	aStar.ResetAStar();

	// ���� �ʱ�ȭ
	isFindingPath = false;
	isDrawingPath = false;
	isMoveToPath = false;

	// aStar ���� ��� ����鼭 ������
	movePath.clear();

	renderTimer.Reset();
	renderTimer.SetTargetTime(renderTargetTime);

	player->ResetSettings();
}

