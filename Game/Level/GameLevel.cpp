#include "GameLevel.h"

#include "Engine.h"
#include "Game/Game.h"
#include "Utils/AStar.h"
#include "Utils/Node.h"
#include "Actor/GridMap.h"
#include "Actor/Player.h"

#include <string>

#include <iostream>


GameLevel::GameLevel()
{
	grid = new GridMap(50, 50);
	AddActor(grid);

	playerCursor = Vector2(0, 0);

	startPos = Vector2(0, 0);
	goalPos = Vector2(0, 0);

	renderTargetTime = 0.05f;
	renderTimer.SetTargetTime(renderTargetTime);

	player = new Player();
	AddActor(player);

	keyMoveSpeed = 5.0f;
	keyMoveDelay = 0.3f;

	keyInputTime = 0.5f;

	for (int i = 0; i < 4; ++i)
	{
		keyInputTimer[i].Reset();
		keyInputTimer[i].SetTargetTime(keyInputTime);
		keyMoveCurDelay[i] = 0.f;
	}
}

GameLevel::~GameLevel()
{
	for(std::vector<Node*> pathNode : movePath)
	{
		for (Node* node : pathNode)
		{
			SafeDelete(node);
		}
		pathNode.clear();
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
		if(isPlayingAnim == false)
		{
			// 방향키 눌렀을 때=======================================
			if (Input::Get().GetKeyDown(VK_UP))
			{
				keyInputTimer[0].Tick(_deltaTime);
				if (playerCursor.y > 0)
					--playerCursor.y;
			}

			if (Input::Get().GetKeyDown(VK_DOWN))
			{
				keyInputTimer[1].Tick(_deltaTime);
				if (playerCursor.y < grid->GetHeight() - 1)
					++playerCursor.y;
			}

			if (Input::Get().GetKeyDown(VK_LEFT))
			{
				keyInputTimer[2].Tick(_deltaTime);
				if (playerCursor.x > 0)
					--playerCursor.x;
			}

			if (Input::Get().GetKeyDown(VK_RIGHT))
			{
				keyInputTimer[3].Tick(_deltaTime);
				if (playerCursor.x < grid->GetWidth() - 1)
					++playerCursor.x;
			}

			// 방향키 입력 중========================================
			if (Input::Get().GetKey(VK_UP))
			{
				if (!keyInputTimer[0].IsTimeout())
				{
					keyInputTimer[0].Tick(_deltaTime);
				}
				else
				{
					keyMoveCurDelay[0] += keyMoveSpeed * _deltaTime;
					if (keyMoveCurDelay[0] > keyMoveDelay)
					{
						if (playerCursor.y > 0.f)
							--playerCursor.y;

						keyMoveCurDelay[0] = 0.f;
					}
				}
			}

			if (Input::Get().GetKey(VK_DOWN))
			{
				if (!keyInputTimer[1].IsTimeout())
				{
					keyInputTimer[1].Tick(_deltaTime);
				}
				else
				{
					keyMoveCurDelay[1] += keyMoveSpeed * _deltaTime;
					if (keyMoveCurDelay[1] > keyMoveDelay)
					{
						if (playerCursor.y < grid->GetHeight() - 1)
							++playerCursor.y;

						keyMoveCurDelay[1] = 0.f;
					}
				}
			}

			if (Input::Get().GetKey(VK_LEFT))
			{
				if (!keyInputTimer[2].IsTimeout())
				{
					keyInputTimer[2].Tick(_deltaTime);
				}
				else
				{
					keyMoveCurDelay[2] += keyMoveSpeed * _deltaTime;
					if (keyMoveCurDelay[2] > keyMoveDelay)
					{
						if (playerCursor.x > 0)
							--playerCursor.x;

						keyMoveCurDelay[2] = 0.f;
					}
				}
			}

			if (Input::Get().GetKey(VK_RIGHT))
			{
				if (!keyInputTimer[3].IsTimeout())
				{
					keyInputTimer[3].Tick(_deltaTime);
				}
				else
				{
					keyMoveCurDelay[3] += keyMoveSpeed * _deltaTime;
					if (keyMoveCurDelay[3] > keyMoveDelay)
					{
						if (playerCursor.x < grid->GetWidth() - 1)
							++playerCursor.x;

						keyMoveCurDelay[3] = 0.f;
					}
				}
			}

			// 방향키 땠을 때========================================
			if (Input::Get().GetKeyUp(VK_UP))
			{
				keyInputTimer[0].Reset();
				keyInputTimer[0].SetTargetTime(keyInputTime);
				keyMoveCurDelay[0] = 0.f;
			}

			if (Input::Get().GetKeyUp(VK_DOWN))
			{
				keyInputTimer[1].Reset();
				keyInputTimer[1].SetTargetTime(keyInputTime);
				keyMoveCurDelay[1] = 0.f;
			}

			if (Input::Get().GetKeyUp(VK_LEFT))
			{
				keyInputTimer[2].Reset();
				keyInputTimer[2].SetTargetTime(keyInputTime);
				keyMoveCurDelay[2] = 0.f;
			}

			if (Input::Get().GetKeyUp(VK_RIGHT))
			{
				keyInputTimer[3].Reset();
				keyInputTimer[3].SetTargetTime(keyInputTime);
				keyMoveCurDelay[3] = 0.f;
			}



			// 벽 생성
			if (Input::Get().GetKeyDown('W'))
			{
				grid->SetWall(playerCursor);
				int curWall = *(grid->GetGridInfo()[playerCursor.y][playerCursor.x]);
				if (curWall == 0) isSetWall = false;
				else isSetWall = true;
			}
			// 벽 생성
			if (Input::Get().GetKey('W'))
			{
				grid->SetWall(playerCursor, isSetWall);
			}

			// 벽 초기화
			if (Input::Get().GetKeyDown('R'))
			{
				grid->WallReset();
			}

			if (Input::Get().GetKeyDown('S'))
			{
				if(isDrawingPath == false)
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

			if (isPlayingAnim == false) isPlayingAnim = true;
			
			// 출발 위치로 이동 시키기
			player->Move(Vector2(movePath[0][0]->position.x, movePath[0][0]->position.y));
		}

		if (player->GetIsPlay())
		{
			//for (int i = 0; i < movePath.size(); ++i)

			while(animNum < movePath.size())
			{
				bool isBreak = false;
				for(int j = 0; j < movePath[animNum].size(); ++j)
				{
					// 현재 노드가 플레이어 위치랑 같을 시
					if (player->GetPosition().x == movePath[animNum][j]->position.x
						&& player->GetPosition().y == movePath[animNum][j]->position.y)
					{
						// 다음 노드로 플레이어를 이동 시킨다.
						if (j != movePath[animNum].size() - 1)
						{
							player->Move(Vector2(movePath[animNum][j+1]->position.x, movePath[animNum][j+1]->position.y));
							isBreak = true;
							break;
						}
						else
						{
							++animNum;

							if (animNum == movePath.size())
							{
								isPlayingAnim = false;
							}

							break;
						}
					}
				}

				if (isBreak) break;
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
		for(int i = 0; i < movePath.size(); ++i)
		{
			if(i == animNum || animNum >= movePath.size())
			{
				for (int j = 0; j < movePath[i].size(); ++j)
				{
					if (movePath[i][j]->position.x == player->GetPosition().x
						&& movePath[i][j]->position.y == player->GetPosition().y)
						continue;

					// 만약 목적노드여서 루트 대신 숫자 적을지 여부
					bool isDrawPass = false;
					for (Node* purposeNode : grid->GetPurposeNode())
					{
						if (movePath[i][j]->position.x == purposeNode->position.x
							&& movePath[i][j]->position.y == purposeNode->position.y)
						{
							isDrawPass = true;
							break;
						}
					}
					if (isDrawPass == true) continue;

					Engine::Get().WriteToBuffer(Vector2(movePath[i][j]->position.x + 1, movePath[i][j]->position.y + 1), "*", Color::Yellow);
				}
			}
		}
	}

	if (isFindingPath == false && isMoveToPath == false)
	{
		Engine::Get().WriteToBuffer(Vector2(playerCursor.x + 1, playerCursor.y + 1), "O", Color::White);
	}

	// 키 알림용 텍스트
	std::string str = "방향키: 이동, W: 벽, R: 벽 초기화, S: 노드, T: 시작(노드2개 이상), DEL: 초기화";
	Engine::Get().WriteToBuffer(
		Vector2(0, grid->GetHeight() + 2),
		str.c_str(),
		Color::White
	);
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
	// 탐색해야할 노드 설정
	if(findNum < grid->GetPurposeNode().size() - 1)
	{
		for (int i = 0; i < findNum; ++i)
		{
			++iter;
		}
	}

	Node* startNode = *iter;
	++iter;
	Node* goalNode = *iter;

	std::vector<Node*> path = aStar.FindPath(startNode, goalNode, grid->GetGridInfo());

	// 순회 한번 완료 시
	if (aStar.GetIsFindDestination())
	{
		// 현재 경로를 저장
		movePath.emplace_back(path);

		// astar 내 열린 리스트, 닫힌 리스트 초기화하기
		aStar.ResetOpenClosedList(path);

		// grid 내 열린/닫힌 리스트 초기화
		grid->ResetOpenClosedList();


		// 순회 횟수 추가
		findNum += 1;

		// 순회가 마무리되었으면
		if(findNum >= grid->GetPurposeNode().size() - 1)
		{
			isDrawingPath = true;
			isFindingPath = false;

			grid->SetIsDraw(false);
		}

		grid->SetIsNodeChange(true);
	}
}

void GameLevel::ResetSettings()
{
	// Grid 초기화
	grid->ResetSettings(aStar.GetIsFindDestination());

	// Astar 초기화
	aStar.ResetAStar();

	// 위에서 Start 노드만 openlist 에서 바로 삭제
	// 나머지는 new node로 만든거
	// 0번만 제외하고 지울것
	for (std::vector<Node*> pathNode : movePath)
	{
		for (Node* node : pathNode)
		{
			if(node != pathNode[0])
				SafeDelete(node);
		}
		pathNode.clear();
	}
	movePath.clear();



	// 레벨 초기화
	isFindingPath = false;
	isDrawingPath = false;
	isMoveToPath = false;

	// aStar 에서 노드 지우면서 지워짐
	movePath.clear();

	renderTimer.Reset();
	renderTimer.SetTargetTime(renderTargetTime);

	player->ResetSettings();

	findNum = 0;
	animNum = 0;
}

