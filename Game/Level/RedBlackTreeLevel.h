#pragma once
#include "Level/Level.h"
#include "Utils/Timer.h"
#include "Utils/RBT/RedblackTree.h"
#include <vector>

// Red-Black Tree 시각화 레벨
// 1. 레벨에서 키입력을 받음
// 2. 키입력이 없으면 Tick 을 보내주지 않음 (어짜피 이 레벨에서는 실시간 처리가 없음)
// 2-1. 키 입력 시 데이터 추가/삭제 할 값 입력
// 2-2. 입력 받을 시 추가/삭제 여부 확인
// 2-3. RedBlackTree 돌림
// 3. 정렬된 Tree 결과 렌더링

// 반복

enum class CommandType
{
	None,
	Insert,
	Delete,
	Sort,
};

enum class funcType
{
	Create,	// 최초 삽입까지 처리
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

	// 시점 함수
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
	// 레드 블랙 트리
	RedBlackTree* rbt;
	RBTAnimNode* animTreeRoot = nullptr;
	
	CommandType command = CommandType::None;

	// cin 쓰니까 자꾸 개행되서 만듬
	// 숫자키 down, back space 받아서 입력 숫자 처리할 예정
	char inputData[100] = { };
	int dataLength = 0;
	

	// 과정을 보여주기 위한 타이머
	Timer rbtTimer;
	float rbtRenderDelay = 0.1f;
	bool isAnimPlaying = false;


	std::vector<RBTAnimNode*> animNodes;

	std::vector<NodeFunc*> animQueue;
};