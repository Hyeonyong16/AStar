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

	// 키입력
	// Todo: GetKey 함수로 변경 및 타이머 추가해서 이동속도 조절
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

		// 벽 생성
		if (Input::Get().GetKeyDown('W'))
		{
			grid->SetWall(playerCursor);
		}

		// 벽 초기화
		if (Input::Get().GetKeyDown('R'))
		{
			grid->WallReset();
		}

		if (Input::Get().GetKeyDown('S'))
		{
			grid->SetPurposeNode(playerCursor);
		}

		// 설정 초기화
		if (Input::Get().GetKeyDown(VK_DELETE))
		{
			ResetSettings();
		}

		// 길찾기 시작
		if (Input::Get().GetKeyDown('T'))
		{
			// 목표 노드가 2개 미만 (아예 없거나 시작점만 있음)
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
		// 플레이어 처음 활성화 시
		if (player->GetIsDraw() == false)
		{
			// Draw 활성화
			player->SetIsDraw(true);
			
			// 출발 위치로 이동 시키기
			player->Move(Vector2(movePath[0]->position.x, movePath[0]->position.y));
		}

		if (player->GetIsPlay())
		{
			for (int i = 0; i < movePath.size(); ++i)
			{
				// 현재 노드가 플레이어 위치랑 같을 시
				if (player->GetPosition().x == movePath[i]->position.x
					&& player->GetPosition().y == movePath[i]->position.y)
				{
					// 다음 노드로 플레이어를 이동 시킨다.
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
	// 목표 노드가 2개 미만 (아예 없거나 시작점만 있음)
	if (grid->GetPurposeNode().size() < 2)
	{
		return;
	}

	// 경로 탐색.
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
	// Grid 초기화
	grid->ResetSettings(aStar.GetIsFindDestination());

	// Astar 초기화
	aStar.ResetAStar();

	// 레벨 초기화
	isFindingPath = false;
	isDrawingPath = false;
	isMoveToPath = false;

	// aStar 에서 노드 지우면서 지워짐
	movePath.clear();

	renderTimer.Reset();
	renderTimer.SetTargetTime(renderTargetTime);

	player->ResetSettings();
}

