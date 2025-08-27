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

	// Ű�Է�
	// Todo: GetKey �Լ��� ���� �� Ÿ�̸� �߰��ؼ� �̵��ӵ� ����
	if(isFindingPath == false)
	{
		if(isPlayingAnim == false)
		{
			// ����Ű ������ ��=======================================
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

			// ����Ű �Է� ��========================================
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

			// ����Ű ���� ��========================================
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



			// �� ����
			if (Input::Get().GetKeyDown('W'))
			{
				grid->SetWall(playerCursor);
				int curWall = *(grid->GetGridInfo()[playerCursor.y][playerCursor.x]);
				if (curWall == 0) isSetWall = false;
				else isSetWall = true;
			}
			// �� ����
			if (Input::Get().GetKey('W'))
			{
				grid->SetWall(playerCursor, isSetWall);
			}

			// �� �ʱ�ȭ
			if (Input::Get().GetKeyDown('R'))
			{
				grid->WallReset();
			}

			if (Input::Get().GetKeyDown('S'))
			{
				if(isDrawingPath == false)
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

			if (isPlayingAnim == false) isPlayingAnim = true;
			
			// ��� ��ġ�� �̵� ��Ű��
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
					// ���� ��尡 �÷��̾� ��ġ�� ���� ��
					if (player->GetPosition().x == movePath[animNum][j]->position.x
						&& player->GetPosition().y == movePath[animNum][j]->position.y)
					{
						// ���� ���� �÷��̾ �̵� ��Ų��.
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

					// ���� ������忩�� ��Ʈ ��� ���� ������ ����
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

	// Ű �˸��� �ؽ�Ʈ
	std::string str = "����Ű: �̵�, W: ��, R: �� �ʱ�ȭ, S: ���, T: ����(���2�� �̻�), DEL: �ʱ�ȭ";
	Engine::Get().WriteToBuffer(
		Vector2(0, grid->GetHeight() + 2),
		str.c_str(),
		Color::White
	);
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
	// Ž���ؾ��� ��� ����
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

	// ��ȸ �ѹ� �Ϸ� ��
	if (aStar.GetIsFindDestination())
	{
		// ���� ��θ� ����
		movePath.emplace_back(path);

		// astar �� ���� ����Ʈ, ���� ����Ʈ �ʱ�ȭ�ϱ�
		aStar.ResetOpenClosedList(path);

		// grid �� ����/���� ����Ʈ �ʱ�ȭ
		grid->ResetOpenClosedList();


		// ��ȸ Ƚ�� �߰�
		findNum += 1;

		// ��ȸ�� �������Ǿ�����
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
	// Grid �ʱ�ȭ
	grid->ResetSettings(aStar.GetIsFindDestination());

	// Astar �ʱ�ȭ
	aStar.ResetAStar();

	// ������ Start ��常 openlist ���� �ٷ� ����
	// �������� new node�� �����
	// 0���� �����ϰ� �����
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



	// ���� �ʱ�ȭ
	isFindingPath = false;
	isDrawingPath = false;
	isMoveToPath = false;

	// aStar ���� ��� ����鼭 ������
	movePath.clear();

	renderTimer.Reset();
	renderTimer.SetTargetTime(renderTargetTime);

	player->ResetSettings();

	findNum = 0;
	animNum = 0;
}

