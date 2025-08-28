#include "RedBlackTree.h"

// �޸� ���� �Լ�.
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
	// �ʱ� ����.
	if (nil == nullptr)
	{
		// Nil ��带 ����.
		nil = new RBTNode(0, NodeColor::Black);
	}

	// ��Ʈ ��带 Nil�� ���� (Ʈ�� �� ����).
	root = nil;
}

RedBlackTree::~RedBlackTree()
{
	// ��������� ��� ��带 �����ϴ� �Լ� ȣ��.
	DestroyRecursive(root);

	// Nil ��� ����.
	SafeDelete(nil);
}

RBTNode* RedBlackTree::CreateNode(int data, NodeColor color)
{
	// ��� ����.
	RBTNode* newNode = new RBTNode(data, color);
	newNode->SetLeft(nil);
	newNode->SetRight(nil);

	return newNode;
}

// �˻�.
bool RedBlackTree::Find(int data, RBTNode*& outNode)
{
	// Ʈ���� ������� Ȯ��.
	if (root == nil)
	{
		return false;
	}

	// ��������� �˻� ����.
	return FindRecursive(data, root, outNode);
}

// �߰�.
bool RedBlackTree::Insert(int data)
{
	// �ߺ� ���� Ȯ��.
	RBTNode* node = nullptr;
	if (Find(data, node))
	{
		std::cout << "Error: �̹� ���� ���� �ֽ��ϴ�.\n";
		return false;
	}

	// ���� ó��.

	// Ʈ���� ������� ��Ʈ�� �߰�.
	if (root == nil)
	{
		// ��Ʈ�� ��.
		root = CreateNode(data, NodeColor::Black);
		return true;
	}

	// ��������� ������ ��ġ �˻� �� ���� ó��.
	RBTNode* newNode = CreateNode(data, NodeColor::Red);
	InsertRecursive(root, newNode);

	// Todo: �Ʒ� ���� �ʿ����� Ȯ���� ��.
	// ���� �� ��� ����.
	newNode->SetColor(NodeColor::Red);
	newNode->SetLeft(nil);
	newNode->SetRight(nil);

	// ���� �� ����.
	RestructureAfterInsert(newNode);
}

bool RedBlackTree::Delete(int data)
{
	// Ʈ���� ������� ����ó��
	if (root == nil)
	{
		return false;
	}

	// �� ���� ���� Ȯ��
	RBTNode* node = nullptr;
	if (Find(data, node) == false)
	{
		return false;
	}

	// ���� ��尡 ���������� ����
	bool isRed = false;
	// ���� ��带 ����� ����� ��带 �޾ƿ�
	RBTNode* deleteNode = DeleteRecursive(root, data, isRed);
	if (deleteNode == nil)
	{
		// 1. ��Ʈ�� �ִ��� ���� ���
		if (root == nil)
		{
			return true;
		}
		// 2. ���� ��ã�� ���
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
	// Ż�� ����.
	if (node == nil)
	{
		outNode = nullptr;
		return false;
	}

	// �� Ȯ�� (�˻� ���� Ȯ��).
	if (node->GetData() == data)
	{
		outNode = node;
		return true;
	}

	// ���� ���.
	if (node->GetData() > data)
	{
		return FindRecursive(data, node->GetLeft(), outNode);
	}

	// ū ���.
	return FindRecursive(data, node->GetRight(), outNode);
}

void RedBlackTree::InsertRecursive(RBTNode* node, RBTNode* newNode)
{
	// ������ ���� ���� Ʈ���� ����.
	if (node->GetData() > newNode->GetData())
	{
		// ���� ���� �ڼ��� ������, ���� �ڼ����� �߰�.
		if (node->GetLeft() == nil)
		{
			// ����� ���� �ڼ����� �� ��� ����.
			node->SetLeft(newNode);

			// �� ����� �θ� ���� ���� ����.
			newNode->SetParent(node);
			return;
		}

		// ��������� �� ����.
		InsertRecursive(node->GetLeft(), newNode);
	}

	// ū ���.
	else
	{
		// ������ ���� �ڼ��� ������, ������ �ڼ����� �߰�.
		if (node->GetRight() == nil)
		{
			// ����� ������ �ڼ����� �� ��� ����.
			node->SetRight(newNode);

			// �� ����� �θ� ���� ���� ����.
			newNode->SetParent(node);
			return;
		}

		// ��������� �� ����.
		InsertRecursive(node->GetRight(), newNode);
	}
}

void RedBlackTree::RestructureAfterInsert(RBTNode* newNode)
{
	// �θ� ��尡 �������� ��� (Red-Red Conflict).
	while (newNode != root
		&& newNode->GetParent()->GetColor() == NodeColor::Red)
	{
		// ���� �������� -> �θ��� ��ġ�� ���� �޶���.
		// �θ� �Ҿƹ��� ���� ���� �ڼ��� �� -> �Ҿƹ����� ������ �ڼ� = ����.
		if (newNode->GetParent()
			== newNode->GetParent()->GetParent()->GetLeft())
		{
			// ����.
			RBTNode* uncle = newNode->GetParent()->GetParent()->GetRight();

			// Case1: ���̵� ����.
			// �ذ�: �θ�� ������ Black, �Ҿƹ����� Red�� ���� ��, 
			// �Ҿƹ��� �������� �ٽ� �˻�.
			if (uncle->GetColor() == NodeColor::Red)
			{
				// �θ�/������ ���������� ����.
				newNode->GetParent()->SetColor(NodeColor::Black);
				uncle->SetColor(NodeColor::Black);

				// �Ҿƹ����� ���������� ����.
				newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

				// ������ �Ҿƹ����� ����.
				newNode = newNode->GetParent()->GetParent();
				continue;
			}

			// ������ �������� �� (Case2/Case3).
			// �ذ�: �θ� Black, �Ҿƹ����� Red�� ���� ��, 
			// �Ҿƹ����� �������� ȸ��.

			// Case2: �������.
			// Case3(������)���� ���� �� ���� �ذ� �õ�.
			if (newNode == newNode->GetParent()->GetRight())
			{
				// ��ȸ��.
				// �θ� �߽����� ȸ�� ó�� (���������� ����� ����).
				newNode = newNode->GetParent();
				RotateToLeft(newNode);
			}

			// Case3: �θ�� ���� ��ġ�� �������� ��.
			// �θ� Black, �Ҿƹ����� Red�� ���� ��, 
			// �Ҿƹ����� �������� ȸ��.
			newNode->GetParent()->SetColor(NodeColor::Black);
			newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

			// ��ȸ��.
			RotateToRight(newNode->GetParent()->GetParent());
		}

		// �θ� �Ҿƹ��� ���� ������ �ڼ��� ��.
		else
		{
			// ����.
			RBTNode* uncle = newNode->GetParent()->GetParent()->GetLeft();

			// Case1: ���̵� ����.
			// �ذ�: �θ�� ������ Black, �Ҿƹ����� Red�� ���� ��, 
			// �Ҿƹ��� �������� �ٽ� �˻�.
			if (uncle->GetColor() == NodeColor::Red)
			{
				// �θ�/������ ���������� ����.
				newNode->GetParent()->SetColor(NodeColor::Black);
				uncle->SetColor(NodeColor::Black);

				// �Ҿƹ����� ���������� ����.
				newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

				// ������ �Ҿƹ����� ����.
				newNode = newNode->GetParent()->GetParent();
				continue;
			}

			// ������ �������� �� (Case2/Case3).
			// �ذ�: �θ� Black, �Ҿƹ����� Red�� ���� ��, 
			// �Ҿƹ����� �������� ȸ��.

			// Case2: �������.
			// Case3(������)���� ���� �� ���� �ذ� �õ�.
			if (newNode == newNode->GetParent()->GetLeft())
			{
				// ��ȸ��.
				// �θ� �߽����� ȸ�� ó�� (���������� ����� ����).
				newNode = newNode->GetParent();
				RotateToRight(newNode);
			}

			// Case3: �θ�� ���� ��ġ�� �������� ��.
			// �θ� Black, �Ҿƹ����� Red�� ���� ��, 
			// �Ҿƹ����� �������� ȸ��.
			newNode->GetParent()->SetColor(NodeColor::Black);
			newNode->GetParent()->GetParent()->SetColor(NodeColor::Red);

			// ��ȸ��.
			RotateToLeft(newNode->GetParent()->GetParent());
		}
	}

	// ��Ʈ ��� ���� ����.
	root->SetColor(NodeColor::Black);
}

RBTNode* RedBlackTree::DeleteRecursive(RBTNode* node, int data, bool& isRed)
{
	if (node == nil)
	{
		return nil;
	}

	// ������ �������� ���.
	if (node->GetData() > data)
	{
		DeleteRecursive(node->GetLeft(), data, isRed);
	}

	// ū ���.
	else if (node->GetData() < data)
	{
		DeleteRecursive(node->GetRight(), data, isRed);
	}

	// ������ ��� ã��.
	else
	{
		// ������ ��ġ�� ����ϴ� ��� ��ȯ
		RBTNode* tempNode = nil;

		// ����� ��1 (�ڽ��� �� �� ���� ���).
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

		// ����� ��2 (�ڽ��� �� �� �ִ� ���).
		if (node->GetLeft() != nil && node->GetRight() != nil)
		{
			// ������ �������� ���� ���� ������ ��ü.

			// ������ ��ġ�� ��� ���� ��ü ������ �Ҵ�.
			node->SetData(
				FindMinNode(node->GetRight())->GetData()
			);

			// ��ü ���ϴ� ��带 ����.
			DeleteRecursive(FindMinNode(node->GetRight()), FindMinNode(node->GetRight())->GetData(), isRed);
		}

		// ����� ��3 (�ڽ��� �ϳ��� �ִ� ���).
		else
		{
			// ���ʸ� �ִ� ���.
			if (node->GetLeft() != nil)
			{
				if (root == node)
				{
					tempNode = node->GetLeft();
					node->SetLeft(nil);
				}

				else
				{
					// �θ��� ���� ����� ���
					if (node == node->GetParent()->GetLeft())
					{
						// �θ��� ���� ��带 ������ ���� �ڽ� ���� ����
						node->GetParent()->SetLeft(node->GetLeft());
						node->GetLeft()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetLeft();
						//������ �ڽ� ��带 nil �� ����
						node->SetLeft(nil);
					}
					// �θ��� ������ ����� ���
					else
					{
						// �θ��� ������ ��带 ������ ���� �ڽ� ���� ����
						node->GetParent()->SetRight(node->GetLeft());
						node->GetLeft()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetRight();

						//������ �ڽ� ��带 nil �� ����
						node->SetLeft(nil);
					}
				}
			}

			// �����ʸ� �ִ� ���.
			else if (node->GetRight() != nil)
			{
				if (root == node)
				{
					tempNode = node->GetRight();
					node->SetLeft(nil);
				}

				else
				{
					// �θ��� ���� ����� ���
					if (node == node->GetParent()->GetLeft())
					{
						// �θ��� ���� ��带 ������ ���� �ڽ� ���� ����
						node->GetParent()->SetLeft(node->GetRight());
						node->GetRight()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetLeft();

						//������ �ڽ� ��带 nil �� ����
						node->SetRight(nil);
					}
					// �θ��� ������ ����� ���
					else
					{
						// �θ��� ������ ��带 ������ ���� �ڽ� ���� ����
						node->GetParent()->SetRight(node->GetRight());
						node->GetRight()->SetParent(node->GetParent());
						tempNode = node->GetParent()->GetRight();

						//������ �ڽ� ��带 nil �� ����
						node->SetRight(nil);
					}
				}
			}

			if (root == node)
			{
				root = tempNode;
			}

			// ��� ����.
			if (node->GetColor() == NodeColor::Red)
				isRed = true;
			delete node;
			return tempNode;
		}
	}
}

void RedBlackTree::RestructureAfterDelete(RBTNode* node, bool isRed)
{
	// ������ ��尡 �������̸� �����Ϸ�
	if (isRed == true)
	{
		return;
	}
	// ��ü�ϴ� ��尡 �������̸� ����� X
	if (node->GetColor() == NodeColor::Red)
	{
		node->SetColor(NodeColor::Black);
		return;
	}

	while (node != root && node->GetColor() == NodeColor::Black)
	{
		RBTNode* siblingNode = nil;
		// �θ��� ���ʳ����
		if (node == node->GetParent()->GetLeft())
		{
			siblingNode = node->GetParent()->GetRight();
		}
		else
		{
			siblingNode = node->GetParent()->GetLeft();
		}

		// ���� ��尡 Red
		if (siblingNode->GetColor() == NodeColor::Red)
		{
			siblingNode->SetColor(NodeColor::Black);
			node->GetParent()->SetColor(NodeColor::Red);
			
			// �������尡 ����� �ڽ������� ���缭 ȸ��
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

		// ������ black, ���� �ڽ��� Black
		if (siblingNode->GetLeft()->GetColor() == NodeColor::Black
			&& siblingNode->GetRight()->GetColor() == NodeColor::Black)
		{
			siblingNode->SetColor(NodeColor::Red);
			node = node->GetParent();
		}

		// ������ Black, ���ʸ� Red
		else
		{
			//Left �� Red
			if (siblingNode->GetLeft()->GetColor() == NodeColor::Red)
			{
				siblingNode->GetLeft()->SetColor(NodeColor::Black);
				siblingNode->SetColor(NodeColor::Red);
				RotateToRight(siblingNode);
				siblingNode = node->GetParent()->GetRight();
			}

			// Right �� Red
			siblingNode->SetColor(node->GetParent()->GetColor());
			node->GetParent()->SetColor(NodeColor::Black);
			siblingNode->GetRight()->SetColor(NodeColor::Black);
			RotateToLeft(node->GetParent());
			node = root;
		}
	}

}

// ȸ��: �θ�� �ڼ��� ��ġ�� �ٲٴ� ���.
// ��ȸ�� -> �������� ȸ��. ������ �ڼհ� �θ��� ��ġ�� ��ȯ.
void RedBlackTree::RotateToLeft(RBTNode* node)
{
	// ������ �ڼ� ���.
	RBTNode* right = node->GetRight();

	// ������ �ڼ��� ���� �ڼ� ��带 �θ��� ������ �ڼ����� ���.
	// ���� Ž�� Ʈ�� Ư¡�� ���� ������ �ڼ��� ��� �ڼ��� �θ𺸴� ŭ.
	node->SetRight(right->GetLeft());

	// �θ� ����.
	if (right->GetLeft() != nil)
	{
		right->GetLeft()->SetParent(node);
	}

	// �θ� root�� ���.
	if (right->GetParent() == root)
	{
		// ��Ʈ ����.
		root = right;
		right->SetParent(nullptr);
	}

	// �θ� root�� �ƴ� ���.
	else
	{
		// ���θ� �������� �θ� ��� �־����� Ȯ��.
		if (node == node->GetParent()->GetLeft())
		{
			node->GetParent()->SetLeft(right);
		}
		else
		{
			node->GetParent()->SetRight(right);
		}
	}

	// ������ �ڼ��� ���� �ڼ����� �θ� ��� ���.
	right->SetLeft(node);

	// ������ �ڼ��� �θ� ���θ�� ����.
	right->SetParent(node->GetParent());

	// �θ��� �θ� ������ �ڼ����� ����.
	node->SetParent(right);
}

// ȸ��: �θ�� �ڼ��� ��ġ�� �ٲٴ� ���.
// ��ȸ�� -> ���������� ȸ��. ���� �ڼհ� �θ��� ��ġ�� ��ȯ.
void RedBlackTree::RotateToRight(RBTNode* node)
{
	// ���� �ڼ� ���.
	RBTNode* left = node->GetLeft();

	// ���� �ڼ��� ������ �ڼ� ��带 �θ��� ���� �ڼ����� ���.
	// ���� Ž�� Ʈ�� Ư¡�� ���� ���� �ڼ��� ��� �ڼ��� �θ𺸴� ����.
	node->SetLeft(left->GetRight());

	// �θ� ����.
	if (left->GetRight() != nil)
	{
		left->GetRight()->SetParent(node);
	}

	// �θ� root�� ���.
	if (left->GetParent() == root)
	{
		// ��Ʈ ����.
		root = left;
		left->SetParent(nullptr);
	}

	// �θ� root�� �ƴ� ���.
	else
	{
		// ���θ� �������� �θ� ��� �־����� Ȯ��.
		if (node == node->GetParent()->GetLeft())
		{
			node->GetParent()->SetLeft(left);
		}
		else
		{
			node->GetParent()->SetRight(left);
		}
	}

	// ���� �ڼ��� ������ �ڼ����� �θ� ��� ���.
	left->SetRight(node);

	// ���� �ڼ��� �θ� ���θ�� ����.
	left->SetParent(node->GetParent());

	// �θ��� �θ� ���� �ڼ����� ����.
	node->SetParent(left);
}

void RedBlackTree::PrintRecursive(RBTNode* node, int depth, int blackCount)
{
	// Ż�� ����.
	if (node == nil)
	{
		return;
	}

	// ��� ������ �����̸� blackCount ����.
	if (node->GetColor() == NodeColor::Black)
	{
		++blackCount;
	}

	// �θ� ǥ�� ���ڿ� ����.
	int parentData = 0;
	const char* position = "Root";

	// �θ� ��尡 �ִ��� Ȯ��.
	if (node->GetParent())
	{
		// �θ� ����� ������ ����.
		parentData = node->GetParent()->GetData();

		// �θ� ���κ��� ���� ����� ��ġ ����.
		if (node == node->GetParent()->GetLeft())
		{
			position = "Left";
		}
		else
		{
			position = "Right";
		}
	}

	// ������ �� ����� ���� ���ڿ�.
	char blackCountString[50] = { };

	// �ڼ��� ������ ��������� ������ ��� �� ����.
	if (node->GetLeft() == nil && node->GetRight() == nil)
	{
		sprintf_s(blackCountString, "------ %d", blackCount);
	}

	// Depth ���.
	for (int ix = 0; ix < depth; ++ix)
	{
		std::cout << "  ";
	}

	// ��� ���� ���� �ܼ� ����.
	if (node->GetColor() == NodeColor::Red)
	{
		SetTextColor(TextColor::Red);
	}
	else
	{
		SetTextColor(TextColor::White);
	}

	// ���� ��� �� ���.
	std::cout
		<< node->GetData() << " "
		<< node->GetColorString() << " ["
		<< position << ", "
		<< parentData << "] "
		<< blackCountString << "\n";

	// ��带 ����� �ڿ��� �ܼ� �������.
	SetTextColor(TextColor::White);

	// ���� ��� ���.
	PrintRecursive(node->GetLeft(), depth + 1, blackCount);
	PrintRecursive(node->GetRight(), depth + 1, blackCount);
}

void RedBlackTree::DestroyRecursive(RBTNode* node)
{
	// ��� Ż�� ����.
	if (node == nil)
	{
		return;
	}

	// �ڽ��� �ִ� ���.
	DestroyRecursive(node->GetLeft());
	DestroyRecursive(node->GetRight());

	// ��� ����.
	SafeDelete(node);
}

void SetTextColor(TextColor color)
{
	// �ܼ� ���� ����.
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}