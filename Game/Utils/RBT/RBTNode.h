#pragma once

// ����� ������ ��Ÿ���� ������.
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

	// ������ �ʵ�.
	int GetData() const { return data; }
	void SetData(int newData) { data = newData; }

	// ����.
	NodeColor GetColor() const { return color; }
	void SetColor(NodeColor newColor) { color = newColor; }
	const char* GetColorString() const;

	// �θ� ���.
	RBTNode* GetParent() const { return parent; }
	void SetParent(RBTNode* newParent) { parent = newParent; }

	// ���� �ڼ� ���.
	RBTNode* GetLeft() const { return left; }
	void SetLeft(RBTNode* newLeft) { left = newLeft; }

	// ������ �ڼ� ���.
	RBTNode* GetRight() const { return right; }
	void SetRight(RBTNode* newRight) { right = newRight; }

private:

	// ������ �ʵ�.
	int data = 0;

	// ����.
	NodeColor color = NodeColor::Red;

	// �θ� ���.
	RBTNode* parent = nullptr;

	// ���� �ڼ� ���.
	RBTNode* left = nullptr;

	// ������ �ڼ� ���.
	RBTNode* right = nullptr;
};