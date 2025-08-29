#pragma once

#include "RBTNode.h"
#include <iostream>
#include <Windows.h>
#include <vector>

class RedBlackTreeLevel;
class RedBlackTree
{
public:
	RedBlackTree();
	RedBlackTree(RedBlackTreeLevel* ownLevel);
	~RedBlackTree();

	// �˻�.
	bool Find(int data, RBTNode*& outNode);

	// �߰�.
	bool Insert(int data);
	bool Delete(int data);


	// ��� �Լ�.
	void Print(int depth = 0, int blackCount = 0);

	RBTNode* GetNil() const { return nil; }

	int GetLeftChildNodeNum(RBTNode* node) const;
	int GetRightChildNodeNum(RBTNode* node) const;
	int GetChildNodeNum(RBTNode* node) const;

	RBTNode* GetRoot() const { return root; }

private:
	RBTNode* FindMinNode(RBTNode* node);
	

private:

	// ��� ���� �Լ�.
	// ���丮 ����.
	RBTNode* CreateNode(int data, NodeColor color);

	/// <summary>
	/// �˻� ��� �Լ�. 
	/// </summary>
	/// <param name="data">�˻��� ��</param>
	/// <param name="node">���� �˻��ϴ� ���</param>
	/// <param name="outNode">�˻��� ��带 ����� �Ķ����</param>
	bool FindRecursive(int data, RBTNode* node, RBTNode*& outNode);
	bool FindTempNilNode(RBTNode* node, RBTNode*& outNode);

	// ��� ���� ��� �Լ�.
	void InsertRecursive(RBTNode* node, RBTNode* newNode);

	// ��� ���� �� ������ ó�� �Լ�.
	void RestructureAfterInsert(RBTNode* newNode);

	// ��� ���� ��� �Լ�
	RBTNode* DeleteRecursive(RBTNode* node, int data, bool& isRed);

	// ��� ���� �� ������ ó�� �Լ�
	void RestructureAfterDelete(RBTNode* node, bool isRed);

	// ��ȸ�� �Լ�.
	void RotateToLeft(RBTNode* node);

	// ��ȸ�� �Լ�.
	void RotateToRight(RBTNode* node);

	// ��� ��� �Լ�.
	void PrintRecursive(RBTNode* node, int depth, int blackCount);

	// Ʈ�� �ı� ��� �Լ�.
	void DestroyRecursive(RBTNode* node);

private:

	// ��Ʈ ���.
	RBTNode* root = nullptr;

	// Nil ���.
	/*static */RBTNode* nil = nullptr;

	RedBlackTreeLevel* ownLevel;
};

enum class TextColor
{
	// �ܼ� ���� ����.
	Red = FOREGROUND_RED,
	Green = FOREGROUND_GREEN,
	Blue = FOREGROUND_BLUE,
	White = Red | Green | Blue,
};

// �ܼ� ���� ���� �Լ�.
void SetTextColor(TextColor color);