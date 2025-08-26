#pragma once

#include "Actor/Actor.h"
#include <vector>
#include <list>

// A*
// �׸��� ������ �����ϰ� ���� �׸� Actor

class Node;
class GridMap : public Actor
{
	RTTI_DECLARATIONS(GridMap, Actor)

public:
	GridMap();
	GridMap(int width, int height);
	~GridMap();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;


public:
	// Getter
	inline std::vector<std::vector<int*>>& GetGridInfo() { return grid; }
	inline std::list<Node*>& GetPurposeNode() { return purposeNode; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	// �� ����
	void SetWall(Vector2 pos);

	// �� ����
	void WallReset();

	// ��ǥ ��ġ ����
	void SetPurposeNode(Vector2 pos);

	// ��ǥ ��ġ ����
	void PurPoseNodeReset();

	void ResetSettings(bool isSafeDelete);

public:
	void SetIsDraw(bool isDraw) { isDrawList = isDraw; }
	

private:
	// Astar ���� �ʵ�
	// 0 : ��ĭ
	// -1 : ��
	// 1~10 : ��ǥ ��ġ (1�� ���� > 2���� ������)
	// 20 : ���� ����Ʈ
	// 21 : ���� ����Ʈ
	std::vector<std::vector<int*>> grid;

	// �׸��� ũ��
	int width = 20;
	int height = 20;

	// �ִ� ��ǥ ��� ��
	int maxPurposeNode = 10;

	// ��ǥ ��� ���� ����
	bool isNodeChange = false;

	// ��ǥ ������ ������ ����Ʈ
	// head ���� �̵�
	std::list<Node*> purposeNode;

	bool isDrawList = false;
};
