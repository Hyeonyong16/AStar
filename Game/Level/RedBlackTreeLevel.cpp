#include "RedBlackTreeLevel.h"
#include "Core/Engine.h"
#include "Game/Game.h"
#include "Actor/RBTAnimNode.h"

#include <Windows.h>
#include <string>

RedBlackTreeLevel::RedBlackTreeLevel()
{
	rbt = new RedBlackTree(this);
}

RedBlackTreeLevel::~RedBlackTreeLevel()
{
	SafeDelete(rbt);
	for (RBTAnimNode* node : animNodes)
	{
		SafeDelete(node);
	}
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

	if (command == CommandType::Insert || command == CommandType::Delete)
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
			if (dataLength > 0)
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
				if (command == CommandType::Insert)
				{
					rbt->Insert(atoi(inputData));
				}
				else if (command == CommandType::Delete)
				{
					rbt->Delete(atoi(inputData));
				}
			}
			command = CommandType::None;
			isAnimPlaying = true;
			rbtTimer.Reset();
			rbtTimer.SetTargetTime(rbtRenderDelay);
		}
	}


	// 아무 상태 아닐때만 명령어 받기
	else if (command == CommandType::None && isAnimPlaying == false)
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
			for (int i = 0; i < (sizeof(inputData) / sizeof(inputData[0])); ++i)
			{
				inputData[i] = '\0';
			}
			dataLength = 0;
		}
	}

	if (isAnimPlaying)
	{
		rbtTimer.Tick(_deltaTime);
		if(rbtTimer.IsTimeout())
		{
			PlayAnim();
			rbtTimer.Reset();
			rbtTimer.SetTargetTime(rbtRenderDelay);
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

	else if (command == CommandType::Delete)
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

void RedBlackTreeLevel::InsertAnim(NodeFunc* func)
{
	animQueue.emplace_back(func);
}

void RedBlackTreeLevel::Print()
{
	//PrintRecursive(rbt->GetRoot(), 0, 0);
	RBTAnimNode* root = nullptr;
	for (int i = 0; i < animNodes.size(); ++i)
	{
		if (animNodes[i]->GetParentNode() == nullptr)
		{
			root = animNodes[i];
		}
	}

	PrintRecursive(root, 0, 0);
	
}

//void RedBlackTreeLevel::PrintRecursive(RBTNode* node, int depth, int spaceCount)
//{
//	// 탈출 조건.
//	if (node == rbt->GetNil())
//	{
//		return;
//	}
//	// 현재 노드 값 출력.
//
//	int spaceNum = 0;
//
//	// 현재 루트 노드라면
//	if (node->GetParent() == nullptr)
//	{
//		spaceNum = rbt->GetRightChildNodeNum(node);
//	}
//
//	// 루트 노드가 아님
//	else
//	{
//		// 부모의 왼쪽 노드라면
//		if (node == node->GetParent()->GetLeft())
//		{
//			spaceNum = spaceCount - rbt->GetRightChildNodeNum(node);
//		}
//
//		// 부모의 오른쪽 노드라면
//		else
//		{
//			spaceNum = spaceCount + rbt->GetLeftChildNodeNum(node);
//		}
//	}
//	std::string buffer = std::to_string(node->GetData());
//
//	Color nodeColor = Color::BlueIntensity;
//	if (node->GetColor() == NodeColor::Red)
//	{
//		nodeColor = Color::RedIntensity;
//	}
//
//	//Engine::Get().WriteToBuffer(Vector2(depth * 5, spaceNum + 2), buffer.c_str(), nodeColor);
//	Engine::Get().WriteToBuffer(Vector2(spaceNum * 4, depth * 3 + 5), buffer.c_str(), nodeColor);
//
//	// 하위 노드 출력.
//	PrintRecursive(node->GetLeft(), depth + 1, spaceNum);
//	PrintRecursive(node->GetRight(), depth + 1, spaceNum);
//}

void RedBlackTreeLevel::PrintRecursive(RBTAnimNode* node, int depth, int spaceCount)
{
	// 탈출 조건.
	if (node == nullptr)
	{
		return;
	}
	// 현재 노드 값 출력.

	int spaceNum = 0;

	// 현재 루트 노드라면
	if (node->GetParentNode() == nullptr)
	{
		spaceNum = GetRightChildNodeNum(node);
	}

	// 루트 노드가 아님
	else
	{
		// 부모의 왼쪽 노드라면
		if (node == node->GetParentNode()->GetLeftNode())
		{
			spaceNum = spaceCount - GetRightChildNodeNum(node);
		}

		// 부모의 오른쪽 노드라면
		else
		{
			spaceNum = spaceCount + GetLeftChildNodeNum(node);
		}
	}
	std::string buffer = std::to_string(node->GetData());

	Color nodeColor = Color::BlueIntensity;
	if (node->GetNodeColor() == NodeColor::Red)
	{
		nodeColor = Color::RedIntensity;
	}

	//Engine::Get().WriteToBuffer(Vector2(depth * 5, spaceNum + 2), buffer.c_str(), nodeColor);
	Engine::Get().WriteToBuffer(Vector2(spaceNum * 4, depth * 3 + 5), buffer.c_str(), nodeColor);

	// 하위 노드 출력.
	PrintRecursive(node->GetLeftNode(), depth + 1, spaceNum);
	PrintRecursive(node->GetRightNode(), depth + 1, spaceNum);
}

void RedBlackTreeLevel::PlayAnim()
{
	// anim Queue 에 아무것도 없다면 반환
	if (animQueue.size() == 0)
	{
		isAnimPlaying = false;
		return;
	}

	int i = 0;
	switch (animQueue[i]->type)
	{
	case funcType::Create:
	{
		// 루트에 들어간거라면
		if (animQueue[i]->color == NodeColor::Black)
		{
			RBTAnimNode* temp = new RBTAnimNode(animQueue[i]->node1);
			temp->SetNodeColor(animQueue[i]->color);
			temp->SetData(animQueue[i]->data);
			animNodes.emplace_back(temp);
		}
		// 아니라면
		else
		{
			RBTAnimNode* temp = new RBTAnimNode(animQueue[i]->node1);
			temp->SetNodeColor(animQueue[i]->color);
			temp->SetData(animQueue[i]->data);

			RBTAnimNode* parent = FindAnimNodeByRef(animQueue[i]->node2);

			// 왼쪽 삽입
			if (animQueue[i]->insertLeft)
			{
				parent->SetLeftNode(temp);
			}
			// 오른쪽 삽입
			else
			{
				parent->SetRightNode(temp);
			}

			temp->SetParentNode(parent);
			animNodes.emplace_back(temp);
		}
		break;
	}

	case funcType::Change_Color:
	{
		RBTAnimNode* temp = FindAnimNodeByRef(animQueue[i]->node1);
		temp->SetNodeColor(animQueue[i]->color);
		break;
	}

	case funcType::Rotate_Left:
	{
		RBTAnimNode* mainNode = FindAnimNodeByRef(animQueue[i]->node1);
		RBTAnimNode* rightNode = mainNode->GetRightNode();

		mainNode->SetRightNode(rightNode->GetLeftNode());

		if (rightNode->GetLeftNode() != nullptr)
		{
			rightNode->GetLeftNode()->SetParentNode(mainNode);
		}

		if (rightNode->GetParentNode()->GetParentNode() == nullptr)
		{
			rightNode->SetParentNode(nullptr);
		}

		else
		{
			if (mainNode == mainNode->GetParentNode()->GetLeftNode())
			{
				mainNode->GetParentNode()->SetLeftNode(rightNode);
			}
			else
			{
				mainNode->GetParentNode()->SetRightNode(rightNode);
			}
		}

		rightNode->SetLeftNode(mainNode);
		rightNode->SetParentNode(mainNode->GetParentNode());
		mainNode->SetParentNode(rightNode);

		break;
	}

	case funcType::Rotate_Right:
	{

		RBTAnimNode* mainNode = FindAnimNodeByRef(animQueue[i]->node1);
		RBTAnimNode* leftNode = mainNode->GetLeftNode();

		mainNode->SetLeftNode(leftNode->GetRightNode());

		if (leftNode->GetRightNode() != nullptr)
		{
			leftNode->GetRightNode()->SetParentNode(mainNode);
		}

		if (leftNode->GetParentNode()->GetParentNode() == nullptr)
		{
			leftNode->SetParentNode(nullptr);
		}

		else
		{
			if (mainNode == mainNode->GetParentNode()->GetLeftNode())
			{
				mainNode->GetParentNode()->SetLeftNode(leftNode);
			}
			else
			{
				mainNode->GetParentNode()->SetRightNode(leftNode);
			}
		}

		leftNode->SetRightNode(mainNode);
		leftNode->SetParentNode(mainNode->GetParentNode());
		mainNode->SetParentNode(leftNode);

		break;
	}
	}

	std::vector<NodeFunc*>::iterator iter = animQueue.begin();
	SafeDelete(*iter);
	animQueue.erase(iter);
}

RBTAnimNode* RedBlackTreeLevel::FindAnimNodeByRef(RBTNode* ref)
{
	for (RBTAnimNode* node : animNodes)
	{
		if (node->GetReferenceNode() == ref)
			return node;
	}
	return nullptr;
}

int RedBlackTreeLevel::GetLeftChildNodeNum(RBTAnimNode* node) const
{
	if (node == nullptr)
	{
		return 0;
	}
	int childNum = GetChildNodeNum(node->GetLeftNode());
	return childNum == 0 ? 1 : childNum + 1;
}

int RedBlackTreeLevel::GetRightChildNodeNum(RBTAnimNode* node) const
{
	if (node == nullptr)
	{
		return 0;
	}
	int childNum = GetChildNodeNum(node->GetRightNode());
	return childNum == 0 ? 1 : childNum + 1;
}

int RedBlackTreeLevel::GetChildNodeNum(RBTAnimNode* node) const
{
	if (node == nullptr)
	{
		return 0;
	}

	int childNum = GetChildNodeNum(node->GetLeftNode()) + GetChildNodeNum(node->GetRightNode());
	childNum += 1;

	return childNum;
}