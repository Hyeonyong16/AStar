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

	// 검색.
	bool Find(int data, RBTNode*& outNode);

	// 추가.
	bool Insert(int data);
	bool Delete(int data);


	// 출력 함수.
	void Print(int depth = 0, int blackCount = 0);

	RBTNode* GetNil() const { return nil; }

	int GetLeftChildNodeNum(RBTNode* node) const;
	int GetRightChildNodeNum(RBTNode* node) const;
	int GetChildNodeNum(RBTNode* node) const;

	RBTNode* GetRoot() const { return root; }

private:
	RBTNode* FindMinNode(RBTNode* node);
	

private:

	// 노드 생성 함수.
	// 팩토리 패턴.
	RBTNode* CreateNode(int data, NodeColor color);

	/// <summary>
	/// 검색 재귀 함수. 
	/// </summary>
	/// <param name="data">검색할 값</param>
	/// <param name="node">현재 검색하는 노드</param>
	/// <param name="outNode">검색한 노드를 출력할 파라미터</param>
	bool FindRecursive(int data, RBTNode* node, RBTNode*& outNode);
	bool FindTempNilNode(RBTNode* node, RBTNode*& outNode);

	// 노드 삽입 재귀 함수.
	void InsertRecursive(RBTNode* node, RBTNode* newNode);

	// 노드 삽입 후 재정렬 처리 함수.
	void RestructureAfterInsert(RBTNode* newNode);

	// 노드 삭제 재귀 함수
	RBTNode* DeleteRecursive(RBTNode* node, int data, bool& isRed);

	// 노드 삭제 후 재정렬 처리 함수
	void RestructureAfterDelete(RBTNode* node, bool isRed);

	// 좌회전 함수.
	void RotateToLeft(RBTNode* node);

	// 우회전 함수.
	void RotateToRight(RBTNode* node);

	// 출력 재귀 함수.
	void PrintRecursive(RBTNode* node, int depth, int blackCount);

	// 트리 파괴 재귀 함수.
	void DestroyRecursive(RBTNode* node);

private:

	// 루트 노드.
	RBTNode* root = nullptr;

	// Nil 노드.
	/*static */RBTNode* nil = nullptr;

	RedBlackTreeLevel* ownLevel;
};

enum class TextColor
{
	// 콘솔 색상 정의.
	Red = FOREGROUND_RED,
	Green = FOREGROUND_GREEN,
	Blue = FOREGROUND_BLUE,
	White = Red | Green | Blue,
};

// 콘솔 색상 변경 함수.
void SetTextColor(TextColor color);