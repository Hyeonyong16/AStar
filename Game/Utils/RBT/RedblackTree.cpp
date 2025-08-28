#include "RedBlackTree.h"

// 메모리 해제 함수.
template<typename T>
void SafeDelete(T*& resource)
{
	if (resource)
	{
		delete resource;
		resource = nullptr;
	}
}

//Node* RedBlackTree::nil = nullptr;

RedBlackTree::RedBlackTree()
{
	// 초기 설정.
	if (nil == nullptr)
	{
		// Nil 노드를 생성.
		nil = new RBTNode(0, NodeColor::Black);
	}

	// 루트 노드를 Nil로 설정 (트리 빈 상태).
	root = nil;
}

RedBlackTree::~RedBlackTree()
{
	// 재귀적으로 모든 노드를 삭제하는 함수 호출.
	DestroyRecursive(root);

	// Nil 노드 삭제.
	SafeDelete(nil);
}

RBTNode* RedBlackTree::CreateNode(int data, NodeColor color)
{
	// 노드 생성.
	RBTNode* newNode = new RBTNode(data, color);
	newNode->SetLeft(nil);
	newNode->SetRight(nil);

	return newNode;
}

// 검색.
bool RedBlackTree::Find(int data, RBTNode*& outNode)
{
	// 트리가 비었는지 확인.
	if (root == nil)
	{
		return false;
	}

	// 재귀적으로 검색 진행.
	return FindRecursive(data, root, outNode);
}

// 추가.
bool RedBlackTree::Insert(int data)
{
	// 중복 여부 확인.
	RBTNode* node = nullptr;
	if (Find(data, node))
	{
		std::cout << "Error: 이미 같은 값이 있습니다.\n";
		return false;
	}

	// 삽입 처리.

	// 트리가 비었으면 루트에 추가.
	if (root == nil)
	{
		// 루트는 블랙.
		root = CreateNode(data, NodeColor::Black);
		return true;
	}

	// 재귀적으로 삽입할 위치 검색 후 삽입 처리.
	RBTNode* newNode = CreateNode(data, NodeColor::Red);
	InsertRecursive(root, newNode);

	// Todo: 아래 로직 필요한지 확인할 것.
	// 삽입 후 노드 정리.
	newNode->SetColor(NodeColor::Red);
	newNode->SetLeft(nil);
	newNode->SetRight(nil);

	// 삽입 후 정리.
	RestructureAfterInsert(newNode);
}

bool RedBlackTree::Delete(int data)
{
	// 트리가 비었으면 예외처리
	if (root == nil)
	{
		return false;
	}

	// 값 존재 여부 확인
	RBTNode* node = nullptr;
	if (Find(data, node) == false)
	{
		return false;
	}

	// 지운 노드가 빨간색인지 여부
	bool isRed = false;
	// 기존 노드를 지우고 대신한 노드를 받아옴
	RBTNode* deleteNode = DeleteRecursive(root, data, isRed);
	if (deleteNode == nil)
	{
		// 1. 루트만 있던걸 지운 경우
		if (root == nil)
		{
			return true;
		}
		// 2. 값을 못찾은 경우
		else
		{
			return false;
		}
	}

	RestructureAfterDelete(deleteNode, isRed);

	return true;
}

void RedBlackTree::Print(int depth, int blackCount)
{
	PrintRecursive(root, depth, blackCount);
}

int RedBlackTree::GetLeftChildNodeNum(RBTNode* node) const
{
	if (node == nil)
	{
		return 0;
	}
	int childNum = GetChildNodeNum(node->GetLeft());
	return childNum == 0 ? 1 : childNum + 1;
}

int RedBlackTree::GetRightChildNodeNum(RBTNode* node) const
{
	if (node == nil)
	{
		return 0;
	}
	int childNum = GetChildNodeNum(node->GetRight());
	return childNum == 0 ? 1 : childNum + 1;
}

int RedBlackTree::GetChildNodeNum(RBTNode* node) const
{
	if (node == nil)
	{
		return 0;
	}

	int childNum = GetChildNodeNum(node->GetLeft()) + GetChildNodeNum(node->GetRight());
	childNum += 1;

	return childNum;
}

RBTNode* RedBlackTree::FindMinNode(RBTNode* node)
{
	while (node->GetLeft() != nil)
	{
		node = node->GetLeft();
	}

	return node;
}

bool RedBlackTree::FindRecursive(int data, RBTNode* node, RBTNode*& outNode)
{
	// 탈출 조건.
	if (node == nil)
	{
		outNode = nullptr;
		return false;
	}

	// 값 확인 (검색 성공 확인).
	if (node->GetData() == data)
	{
		outNode = node;
		return true;
	}

	// 작은 경우.
	if (node->GetData() > data)
	{
		return FindRecursive(data, node->GetLeft(), outNode);
	}

	// 큰 경우.
	return FindRecursive(data, node->GetRight(), outNode);
}

void RedBlackTree::InsertRecursive(RBTNode* node, RBTNode* newNode)
{
	// 작으면 왼쪽 하위 트리에 삽입.
	if (node->GetData() > newNode->GetData())
	{
		// 왼쪽 하위 자손이 없으면, 왼쪽 자손으로 추가.
		if (node->GetLeft() == nil)
		{
			// 노드의 왼쪽 자손으로 새 노드 설정.
			node->SetLeft(newNode);

			// 새 노드의 부모를 현재 노드로 지정.
			newNode->SetParent(node);
			return;
		}

		// 재귀적으로 더 진행.
		InsertRecursive(node->GetLeft(), newNode);
	}

	// 큰 경우.
	else
	{
		// 오른쪽 하위 자손이 없으면, 오른쪽 자손으로 추가.
		if (node->GetRight() == nil)
		{
			// 노드의 오른쪽 자손으로 새 노드 설정.
			node->SetRight(newNode);

			// 새 노드의 부모를 현재 노드로 지정.
			newNode->SetParent(node);
			return;
		}

		// 재귀적으로 더 진행.
		InsertRecursive(node->GetRight(), newNode);
	}
}

void RedBlackTree::RestructureAfterInsert(RBTNode* newNode)
{
	// 부모 노드가 빨간색인 경우 (Red-Red Conflict).
	while (newNode != root
		&& newNode->GetParent()->GetColor() == NodeColor::Red)
	{
		// 삼촌 가져오기 -> 부모의 위치에 따라 달라짐.
		// 부모가 할아버지 기준 왼쪽 자손일 때 -> 할아버지의 오른쪽 자손 = 삼촌.
		if (newNode->GetParent()
			== newNode->GetParent()->GetParent()->GetLeft())
		{
			// 삼촌.
			RBTNode* uncle = newNode->GetParent()->GetParent()->GetRight();

			// Case1: 삼촌도 빨강.
			// 해결: 부모와 삼촌을 Black, 할아버지를 Red로 변경 후, 
			// 할아버지 기준으로 다시 검사.
			if (uncle->GetColor() == NodeColor::Red)
			{
				// 부모/삼촌을 검정색으로 변경.
				newNode->GetParent()->SetColor(NodeColor::Black);
				uncle->SetColor(NodeColor::Black);

				// 할아버지를 빨간색으로 변경.
				newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

				// 기준을 할아버지로 변경.
				newNode = newNode->GetParent()->GetParent();
				continue;
			}

			// 삼촌이 검정색일 때 (Case2/Case3).
			// 해결: 부모를 Black, 할아버지를 Red로 변경 후, 
			// 할아버지를 기준으로 회전.

			// Case2: 지그재그.
			// Case3(일직선)으로 변경 후 문제 해결 시도.
			if (newNode == newNode->GetParent()->GetRight())
			{
				// 좌회전.
				// 부모를 중심으로 회전 처리 (일직선으로 만들기 위해).
				newNode = newNode->GetParent();
				RotateToLeft(newNode);
			}

			// Case3: 부모와 나의 위치가 일직선일 때.
			// 부모를 Black, 할아버지를 Red로 변경 후, 
			// 할아버지를 기준으로 회전.
			newNode->GetParent()->SetColor(NodeColor::Black);
			newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

			// 우회전.
			RotateToRight(newNode->GetParent()->GetParent());
		}

		// 부모가 할아버지 기준 오른쪽 자손일 때.
		else
		{
			// 삼촌.
			RBTNode* uncle = newNode->GetParent()->GetParent()->GetLeft();

			// Case1: 삼촌도 빨강.
			// 해결: 부모와 삼촌을 Black, 할아버지를 Red로 변경 후, 
			// 할아버지 기준으로 다시 검사.
			if (uncle->GetColor() == NodeColor::Red)
			{
				// 부모/삼촌을 검정색으로 변경.
				newNode->GetParent()->SetColor(NodeColor::Black);
				uncle->SetColor(NodeColor::Black);

				// 할아버지를 빨간색으로 변경.
				newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

				// 기준을 할아버지로 변경.
				newNode = newNode->GetParent()->GetParent();
				continue;
			}

			// 삼촌이 검정색일 때 (Case2/Case3).
			// 해결: 부모를 Black, 할아버지를 Red로 변경 후, 
			// 할아버지를 기준으로 회전.

			// Case2: 지그재그.
			// Case3(일직선)으로 변경 후 문제 해결 시도.
			if (newNode == newNode->GetParent()->GetLeft())
			{
				// 우회전.
				// 부모를 중심으로 회전 처리 (일직선으로 만들기 위해).
				newNode = newNode->GetParent();
				RotateToRight(newNode);
			}

			// Case3: 부모와 나의 위치가 일직선일 때.
			// 부모를 Black, 할아버지를 Red로 변경 후, 
			// 할아버지를 기준으로 회전.
			newNode->GetParent()->SetColor(NodeColor::Black);
			newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

			// 좌회전.
			RotateToLeft(newNode->GetParent()->GetParent());
		}
	}

	// 루트 노드 색상 정리.
	root->SetColor(NodeColor::Black);
}

RBTNode* RedBlackTree::DeleteRecursive(RBTNode* node, int data, bool& isRed)
{
	if (node == nil)
	{
		return nil;
	}

	// 데이터 보다작은 경우.
	if (node->GetData() > data)
	{
		DeleteRecursive(node->GetLeft(), data, isRed);
	}

	// 큰 경우.
	else if (node->GetData() < data)
	{
		DeleteRecursive(node->GetRight(), data, isRed);
	}

	// 삭제할 노드 찾음.
	else
	{
		// 삭제된 위치를 대신하는 노드 반환
		RBTNode* tempNode = nil;

		// 경우의 수1 (자식이 둘 다 없는 경우).
		if (node->GetLeft() == nil && node->GetRight() == nil)
		{
			if (root == node)
			{
				root = nil;
			}

			else if (node == node->GetParent()->GetLeft())
			{
				node->GetParent()->SetLeft(nil);
			}

			else
			{
				node->GetParent()->SetRight(nil);
			}

			tempNode = nil;
			if (node->GetColor() == NodeColor::Red)
				isRed = true;
			delete node;
			return tempNode;
		}

		// 경우의 수2 (자식이 둘 다 있는 경우).
		if (node->GetLeft() != nil && node->GetRight() != nil)
		{
			// 오른쪽 하위에서 가장 작은 값으로 대체.

			// 삭제할 위치의 노드 값을 대체 값으로 할당.
			node->SetData(
				FindMinNode(node->GetRight())->GetData()
			);

			// 대체 당하는 노드를 삭제.
			DeleteRecursive(FindMinNode(node->GetRight()), FindMinNode(node->GetRight())->GetData(), isRed);
		}

		// 경우의 수3 (자식이 하나만 있는 경우).
		else
		{
			// 왼쪽만 있는 경우.
			if (node->GetLeft() != nil)
			{
				if (root == node)
				{
					tempNode = node->GetLeft();
					node->SetLeft(nil);
				}

				else
				{
					// 부모의 왼쪽 노드일 경우
					if (node == node->GetParent()->GetLeft())
					{
						// 부모의 왼쪽 노드를 본인의 왼쪽 자식 노드로 변경
						node->GetParent()->SetLeft(node->GetLeft());
						node->GetLeft()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetLeft();
						//본인의 자식 노드를 nil 로 변경
						node->SetLeft(nil);
					}
					// 부모의 오른쪽 노드일 경우
					else
					{
						// 부모의 오른쪽 노드를 본인의 왼쪽 자식 노드로 변경
						node->GetParent()->SetRight(node->GetLeft());
						node->GetLeft()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetRight();

						//본인의 자식 노드를 nil 로 변경
						node->SetLeft(nil);
					}
				}
			}

			// 오른쪽만 있는 경우.
			else if (node->GetRight() != nil)
			{
				if (root == node)
				{
					tempNode = node->GetRight();
					node->SetLeft(nil);
				}

				else
				{
					// 부모의 왼쪽 노드일 경우
					if (node == node->GetParent()->GetLeft())
					{
						// 부모의 왼쪽 노드를 본인의 왼쪽 자식 노드로 변경
						node->GetParent()->SetLeft(node->GetRight());
						node->GetRight()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetLeft();

						//본인의 자식 노드를 nil 로 변경
						node->SetRight(nil);
					}
					// 부모의 오른쪽 노드일 경우
					else
					{
						// 부모의 오른쪽 노드를 본인의 왼쪽 자식 노드로 변경
						node->GetParent()->SetRight(node->GetRight());
						node->GetRight()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetRight();

						//본인의 자식 노드를 nil 로 변경
						node->SetRight(nil);
					}
				}
			}

			if (root == node)
			{
				root = tempNode;
			}

			// 노드 삭제.
			if (node->GetColor() == NodeColor::Red)
				isRed = true;
			delete node;
			return tempNode;
		}
	}
}

void RedBlackTree::RestructureAfterDelete(RBTNode* node, bool isRed)
{
	// 삭제한 노드가 빨간색이면 삭제완료
	if (isRed == true)
	{
		return;
	}
	// 대체하는 노드가 빨간색이면 더블블랙 X
	if (node->GetColor() == NodeColor::Red)
	{
		node->SetColor(NodeColor::Black);
		return;
	}

	while (node != root && node->GetColor() == NodeColor::Black)
	{
		RBTNode* siblingNode = nil;
		// 부모의 왼쪽노드라면
		if (node == node->GetParent()->GetLeft())
		{
			siblingNode = node->GetParent()->GetRight();
		}
		else
		{
			siblingNode = node->GetParent()->GetLeft();
		}

		// 형제 노드가 Red
		if (siblingNode->GetColor() == NodeColor::Red)
		{
			siblingNode->SetColor(NodeColor::Black);
			node->GetParent()->SetColor(NodeColor::Red);
			
			// 더블블랙노드가 어느쪽 자식인지에 맞춰서 회전
			if (node == node->GetParent()->GetLeft())
			{
				RotateToLeft(node->GetParent());
				siblingNode = node->GetParent()->GetRight();
			}
			else
			{
				RotateToRight(node->GetParent());
				siblingNode = node->GetParent()->GetLeft();
			}
		}

		// 형제가 black, 양쪽 자식이 Black
		if (siblingNode->GetLeft()->GetColor() == NodeColor::Black
			&& siblingNode->GetRight()->GetColor() == NodeColor::Black)
		{
			siblingNode->SetColor(NodeColor::Red);
			node = node->GetParent();
		}

		// 형제가 Black, 한쪽만 Red
		else
		{
			//Left 가 Red
			if (siblingNode->GetLeft()->GetColor() == NodeColor::Red)
			{
				siblingNode->GetLeft()->SetColor(NodeColor::Black);
				siblingNode->SetColor(NodeColor::Red);
				RotateToRight(siblingNode);
				siblingNode = node->GetParent()->GetRight();
			}

			// Right 가 Red
			siblingNode->SetColor(node->GetParent()->GetColor());
			node->GetParent()->SetColor(NodeColor::Black);
			siblingNode->GetRight()->SetColor(NodeColor::Black);
			RotateToLeft(node->GetParent());
			node = root;
		}
	}

}

// 회전: 부모와 자손의 위치를 바꾸는 기능.
// 좌회전 -> 왼쪽으로 회전. 오른쪽 자손과 부모의 위치를 교환.
void RedBlackTree::RotateToLeft(RBTNode* node)
{
	// 오른쪽 자손 노드.
	RBTNode* right = node->GetRight();

	// 오른쪽 자손의 왼쪽 자손 노드를 부모의 오른쪽 자손으로 등록.
	// 이진 탐색 트리 특징의 의해 오른쪽 자손의 모든 자손은 부모보다 큼.
	node->SetRight(right->GetLeft());

	// 부모 정리.
	if (right->GetLeft() != nil)
	{
		right->GetLeft()->SetParent(node);
	}

	// 부모가 root인 경우.
	if (right->GetParent() == root)
	{
		// 루트 변경.
		root = right;
		right->SetParent(nullptr);
	}

	// 부모가 root가 아닌 경우.
	else
	{
		// 조부모 기준으로 부모가 어디에 있었는지 확인.
		if (node == node->GetParent()->GetLeft())
		{
			node->GetParent()->SetLeft(right);
		}
		else
		{
			node->GetParent()->SetRight(right);
		}
	}

	// 오른쪽 자손의 왼쪽 자손으로 부모 노드 등록.
	right->SetLeft(node);

	// 오른쪽 자손의 부모를 조부모로 설정.
	right->SetParent(node->GetParent());

	// 부모의 부모를 오른쪽 자손으로 설정.
	node->SetParent(right);
}

// 회전: 부모와 자손의 위치를 바꾸는 기능.
// 우회전 -> 오른쪽으로 회전. 왼쪽 자손과 부모의 위치를 교환.
void RedBlackTree::RotateToRight(RBTNode* node)
{
	// 왼쪽 자손 노드.
	RBTNode* left = node->GetLeft();

	// 왼쪽 자손의 오른쪽 자손 노드를 부모의 왼쪽 자손으로 등록.
	// 이진 탐색 트리 특징의 의해 왼쪽 자손의 모든 자손은 부모보다 작음.
	node->SetLeft(left->GetRight());

	// 부모 정리.
	if (left->GetRight() != nil)
	{
		left->GetRight()->SetParent(node);
	}

	// 부모가 root인 경우.
	if (left->GetParent() == root)
	{
		// 루트 변경.
		root = left;
		left->SetParent(nullptr);
	}

	// 부모가 root가 아닌 경우.
	else
	{
		// 조부모 기준으로 부모가 어디에 있었는지 확인.
		if (node == node->GetParent()->GetLeft())
		{
			node->GetParent()->SetLeft(left);
		}
		else
		{
			node->GetParent()->SetRight(left);
		}
	}

	// 왼쪽 자손의 오른쪽 자손으로 부모 노드 등록.
	left->SetRight(node);

	// 왼쪽 자손의 부모를 조부모로 설정.
	left->SetParent(node->GetParent());

	// 부모의 부모를 왼쪽 자손으로 설정.
	node->SetParent(left);
}

void RedBlackTree::PrintRecursive(RBTNode* node, int depth, int blackCount)
{
	// 탈출 조건.
	if (node == nil)
	{
		return;
	}

	// 노드 색상이 검정이면 blackCount 증가.
	if (node->GetColor() == NodeColor::Black)
	{
		++blackCount;
	}

	// 부모 표기 문자열 설정.
	int parentData = 0;
	const char* position = "Root";

	// 부모 노드가 있는지 확인.
	if (node->GetParent())
	{
		// 부모 노드의 데이터 저장.
		parentData = node->GetParent()->GetData();

		// 부모 노드로부터 현재 노드의 위치 설정.
		if (node == node->GetParent()->GetLeft())
		{
			position = "Left";
		}
		else
		{
			position = "Right";
		}
	}

	// 검은색 수 출력을 위한 문자열.
	char blackCountString[50] = { };

	// 자손이 없으면 현재까지의 검은색 노드 수 설정.
	if (node->GetLeft() == nil && node->GetRight() == nil)
	{
		sprintf_s(blackCountString, "------ %d", blackCount);
	}

	// Depth 출력.
	for (int ix = 0; ix < depth; ++ix)
	{
		std::cout << "  ";
	}

	// 노드 색상에 따른 콘솔 설정.
	if (node->GetColor() == NodeColor::Red)
	{
		SetTextColor(TextColor::Red);
	}
	else
	{
		SetTextColor(TextColor::White);
	}

	// 현재 노드 값 출력.
	std::cout
		<< node->GetData() << " "
		<< node->GetColorString() << " ["
		<< position << ", "
		<< parentData << "] "
		<< blackCountString << "\n";

	// 노드를 출력한 뒤에는 콘솔 원래대로.
	SetTextColor(TextColor::White);

	// 하위 노드 출력.
	PrintRecursive(node->GetLeft(), depth + 1, blackCount);
	PrintRecursive(node->GetRight(), depth + 1, blackCount);
}

void RedBlackTree::DestroyRecursive(RBTNode* node)
{
	// 재귀 탈출 조건.
	if (node == nil)
	{
		return;
	}

	// 자식이 있는 경우.
	DestroyRecursive(node->GetLeft());
	DestroyRecursive(node->GetRight());

	// 노드 삭제.
	SafeDelete(node);
}

void SetTextColor(TextColor color)
{
	// 콘솔 색상 설정.
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}