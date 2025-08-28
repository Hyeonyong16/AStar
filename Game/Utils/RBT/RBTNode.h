#pragma once

// 노드의 색상을 나타내는 열거형.
enum class NodeColor
{
	Red,
	Black
};

class RBTNode
{
public:
	RBTNode(int data, NodeColor color = NodeColor::Red);
	~RBTNode() = default;

	// 데이터 필드.
	int GetData() const { return data; }
	void SetData(int newData) { data = newData; }

	// 색상.
	NodeColor GetColor() const { return color; }
	void SetColor(NodeColor newColor) { color = newColor; }
	const char* GetColorString() const;

	// 부모 노드.
	RBTNode* GetParent() const { return parent; }
	void SetParent(RBTNode* newParent) { parent = newParent; }

	// 왼쪽 자손 노드.
	RBTNode* GetLeft() const { return left; }
	void SetLeft(RBTNode* newLeft) { left = newLeft; }

	// 오른쪽 자손 노드.
	RBTNode* GetRight() const { return right; }
	void SetRight(RBTNode* newRight) { right = newRight; }

private:

	// 데이터 필드.
	int data = 0;

	// 색상.
	NodeColor color = NodeColor::Red;

	// 부모 노드.
	RBTNode* parent = nullptr;

	// 왼쪽 자손 노드.
	RBTNode* left = nullptr;

	// 오른쪽 자손 노드.
	RBTNode* right = nullptr;
};