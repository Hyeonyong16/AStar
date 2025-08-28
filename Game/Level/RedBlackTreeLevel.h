#pragma once
#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Utils/RBT/RedblackTree.h"

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

private:
	void Print();
	void PrintRecursive(RBTNode* node, int depth, int spaceCount);

private:
	// ���� �� Ʈ��
	RedBlackTree* rbt;
	
	CommandType command = CommandType::None;

	// cin ���ϱ� �ڲ� ����Ǽ� ����
	// ����Ű down, back space �޾Ƽ� �Է� ���� ó���� ����
	char inputData[100] = { };
	int dataLength = 0;
	

	// ������ �����ֱ� ���� Ÿ�̸�
	Timer rbtTimer;
	float rbtRenderDelay = 0.3f;
};