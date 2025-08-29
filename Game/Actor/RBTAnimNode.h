#pragma once
#include "Actor/Actor.h"
#include "Utils/RBT/RBTNode.h"

// RBT 삽입, 삭제 과정을 보여주기 위한 액터
// 갖고있을 값
// 1. 위치
// 2. Red, Black 값
// 3. Data(출력될 값)
// 4. 부모
// 5. 왼쪽자식
// 6. 오른쪽 자식
// 7. 참고노드? 넣어야하는가

// nullptr == nil

// 레벨에서 처리해줄 것
// 1. 생성
// 2. 제거

// 3. 색상 변경
// 4. 좌회전
// 5. 우회전

class RBTAnimNode : public Actor
{
	RTTI_DECLARATIONS(RBTAnimNode, Actor)

public:
	RBTAnimNode();
	RBTAnimNode(RBTNode* reference);
	~RBTAnimNode();

	// Getter, Setter
	inline NodeColor GetNodeColor() const { return nodeColor; }
	inline void SetNodeColor(NodeColor color) { nodeColor = color; }

	inline int GetData() const { return data; }
	inline void SetData(int data) { this->data = data; }

	inline RBTAnimNode* GetParentNode() const { return parentNode; }
	inline void SetParentNode(RBTAnimNode* node) { parentNode = node; }

	inline RBTAnimNode* GetLeftNode() const { return leftNode; }
	inline void SetLeftNode(RBTAnimNode* node) { leftNode = node; }

	inline RBTAnimNode* GetRightNode() const { return rightNode; }
	inline void SetRightNode(RBTAnimNode* node) { rightNode = node; }

	inline RBTNode* GetReferenceNode() const { return referenceNode; }
	inline void SetReferenceNode(RBTNode* node) { referenceNode = node; }

private:
	// 현재 노드 색
	NodeColor nodeColor = NodeColor::Red;

	// Data
	int data = 0;

	// 부모 노드
	RBTAnimNode* parentNode = nullptr;

	// 자식 노드
	RBTAnimNode* leftNode = nullptr;
	RBTAnimNode* rightNode = nullptr;

	// 참고 노드
	RBTNode* referenceNode = nullptr;
};