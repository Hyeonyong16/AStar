#pragma once

#include "Actor/Actor.h"
#include "Utils/AStar.h"
#include "Utils/Timer.h"

// �� ã�� �� �����̴� ������Ʈ
// �ʿ� ���
// 1. Ÿ�̸ӿ� ���� ��ġ �̵�
// 2. ���� �� ǥ�� ���θ� �޾Ƽ� �̵� ����

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player();
	~Player();

	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void Render() override;

public:
	inline bool GetIsDraw() const { return isDraw; }
	inline bool GetIsPlay() const { return isPlay; }

	inline void SetIsDraw(bool _isDraw) { isDraw = _isDraw; }
	inline void SetIsPlay(bool _isPlay) { isPlay = _isPlay; }

	// Player �� Ư�� ��ġ�� �̵���Ű�� �Լ�
	void Move(Vector2 pos);

	void ResetSettings();

private:
	// �� ã���� �ÿ��� �� �� Ȱ��ȭ
	bool isDraw = false;

	// Ÿ�̸� �� ���� ��ĭ �̵��ؾ��� ��
	bool isPlay = false;

	// �̵� �ӵ�
	float moveTime = 0.3f;

	Timer moveTimer;
};