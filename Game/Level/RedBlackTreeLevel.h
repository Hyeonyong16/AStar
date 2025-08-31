#pragma once
#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Utils/RBT/RedblackTree.h"
#include <vector>

// Red-Black Tree �ð�ȭ ����
// 1. �������� Ű�Է��� ����
// 2. Ű�Է��� ������ Tick �� �������� ���� (��¥�� �� ���������� �ǽð� ó���� ����)
// 2-1. Ű �Է� �� ������ �߰�/���� �� �� �Է�
// 2-2. �Է� ���� �� �߰�/���� ���� Ȯ��
// 2-3. RedBlackTree ����
// 3. ���ĵ� Tree ��� ������

// �ݺ�

enum class CommandType
{
	None,
	Insert,
	Delete,
	Sort,
};

enum class funcType
{
	Create,	// ���� ���Ա��� ó��
	Delete,
	Change_Data,
	Change_Color,
	Rotate_Left,
	Rotate_Right,

};

struct NodeFunc
{
	funcType type;
	RBTNode* node1 = nullptr;
	RBTNode* node2 = nullptr;
	RBTNode* node3 = nullptr;
	NodeColor color = NodeColor::Red;
	bool insertLeft = false;
	int data = 0;

	NodeFunc(funcType type, bool insertLeft, RBTNode* node1, RBTNode* node2, RBTNode* node3, NodeColor color, int data)
		: type(type), insertLeft(insertLeft), node1(node1), node2(node2), node3(node3), color(color), data(data)
	{

	}
	~NodeFunc() = default;
};

class RBTAnimNode;

class RedBlackTreeLevel : public Level
{
	RTTI_DECLARATIONS(RedBlackTreeLevel, Level)
	
public:
	RedBlackTreeLevel();
	~RedBlackTreeLevel();

	// ���� �Լ�
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

	void InsertAnim(NodeFunc* func);

private:
	void Print();
	void PrintRecursive(RBTAnimNode* node, int depth, int spaceCount);
	void PrintRecursive(RBTNode* node, int depth, int spaceCount);

	void PlayAnim();
	RBTAnimNode* FindAnimNodeByRef(RBTNode* ref);

	int GetLeftChildNodeNum(RBTAnimNode* node) const;
	int GetRightChildNodeNum(RBTAnimNode* node) const;
	int GetChildNodeNum(RBTAnimNode* node) const;



private:
	// ���� �� Ʈ��
	RedBlackTree* rbt;
	RBTAnimNode* animTreeRoot = nullptr;
	
	CommandType command = CommandType::None;

	// cin ���ϱ� �ڲ� ����Ǽ� ����
	// ����Ű down, back space �޾Ƽ� �Է� ���� ó���� ����
	char inputData[100] = { };
	int dataLength = 0;
	

	// ������ �����ֱ� ���� Ÿ�̸�
	Timer rbtTimer;
	float rbtRenderDelay = 0.1f;
	bool isAnimPlaying = false;


	std::vector<RBTAnimNode*> animNodes;

	std::vector<NodeFunc*> animQueue;
};