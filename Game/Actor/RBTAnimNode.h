#pragma once
#include "Actor/Actor.h"
#include "Utils/RBT/RBTNode.h"

// RBT ����, ���� ������ �����ֱ� ���� ����
// �������� ��
// 1. ��ġ
// 2. Red, Black ��
// 3. Data(��µ� ��)
// 4. �θ�
// 5. �����ڽ�
// 6. ������ �ڽ�
// 7. ������? �־���ϴ°�

// nullptr == nil

// �������� ó������ ��
// 1. ����
// 2. ����

// 3. ���� ����
// 4. ��ȸ��
// 5. ��ȸ��

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
	// ���� ��� ��
	NodeColor nodeColor = NodeColor::Red;

	// Data
	int data = 0;

	// �θ� ���
	RBTAnimNode* parentNode = nullptr;

	// �ڽ� ���
	RBTAnimNode* leftNode = nullptr;
	RBTAnimNode* rightNode = nullptr;

	// ���� ���
	RBTNode* referenceNode = nullptr;
};