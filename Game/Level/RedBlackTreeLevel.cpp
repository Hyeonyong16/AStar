#include "RedBlackTreeLevel.h"
#include "Core/Engine.h"
#include "Game/Game.h"

#include <Windows.h>
#include <string>

RedBlackTreeLevel::RedBlackTreeLevel()
{
	rbt = new RedBlackTree();
}

RedBlackTreeLevel::~RedBlackTreeLevel()
{
	SafeDelete(rbt);
}

void RedBlackTreeLevel::BeginPlay()
{
	super::BeginPlay();
}

void RedBlackTreeLevel::Tick(float _deltaTime)
{
	super::Tick(_deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().GoMenu();
	}

	if (command == CommandType::Insert)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (Input::Get().GetKeyDown('0' + i))
			{
				if (dataLength + 1 < (sizeof(inputData) / sizeof(inputData[0])))
				{
					inputData[dataLength] = ('0' + i);
					dataLength += 1;
				}
			}
		}

		if (Input::Get().GetKeyDown(VK_BACK))
		{
			if(dataLength > 0)
			{
				inputData[dataLength - 1] = '\0';
				dataLength -= 1;
			}
		}
		
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			inputData[dataLength] = '\0';
			if (inputData[0] != '\0')
			{
				rbt->Insert(atoi(inputData));
			}
			command = CommandType::None;

		}
	}

	else if (command == CommandType::Delete)
	{

	}
	
	// 아무 상태 아닐때만 명령어 받기
	else if(command == CommandType::None)
	{
		if (Input::Get().GetKeyDown('Q'))
		{
			command = CommandType::Insert;
			for (int i = 0; i < (sizeof(inputData) / sizeof(inputData[0])); ++i)
			{
				inputData[i] = '\0';
			}
			dataLength = 0;
		}

		if (Input::Get().GetKeyDown('E'))
		{
			command = CommandType::Delete;
		}
	}


}

void RedBlackTreeLevel::Render()
{
	if (command == CommandType::None)
	{
		Engine::Get().WriteToBuffer(Vector2(0, 0), "Q: 데이터 입력, E: 데이터 삭제, ESC: 메뉴로 ", Color::White);
	}

	else if (command == CommandType::Insert)
	{
		Engine::Get().WriteToBuffer(Vector2(0, 0), "데이터를 숫자로 입력: ", Color::WhiteIntensity);
		int  i = 0;
		while (inputData[i] != '\0')
		{
			Engine::Get().WriteToBuffer(Vector2(i, 1), inputData[i], Color::WhiteIntensity);
			++i;
		}
	}

	Print();
}

void RedBlackTreeLevel::Print()
{
	PrintRecursive(rbt->GetRoot(), 0, 0);
}

void RedBlackTreeLevel::PrintRecursive(RBTNode* node, int depth, int spaceCount)
{
	// 탈출 조건.
	if (node == rbt->GetNil())
	{
		return;
	}
	// 현재 노드 값 출력.
	
	int spaceNum = 0;

	// 현재 루트 노드라면
	if (node->GetParent() == nullptr)
	{
		spaceNum = rbt->GetRightChildNodeNum(node);
	}

	// 루트 노드가 아님
	else
	{
		// 부모의 왼쪽 노드라면
		if (node == node->GetParent()->GetLeft())
		{
			spaceNum = spaceCount + rbt->GetRightChildNodeNum(node);
		}

		// 부모의 오른쪽 노드라면
		else
		{
			spaceNum = spaceCount - rbt->GetLeftChildNodeNum(node);
		}
	}
	std::string buffer = std::to_string(node->GetData());

	Color nodeColor = Color::BlueIntensity;
	if (node->GetColor() == NodeColor::Red)
	{
		nodeColor = Color::RedIntensity;
	}

	Engine::Get().WriteToBuffer(Vector2(depth * 5, spaceNum + 2), buffer.c_str(), nodeColor);

	// 하위 노드 출력.
	PrintRecursive(node->GetLeft(), depth + 1, spaceNum);
	PrintRecursive(node->GetRight(), depth + 1, spaceNum);
}