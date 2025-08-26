#pragma once

#include "Actor/Actor.h"
#include "Utils/AStar.h"
#include "Utils/Timer.h"

// 길 찾은 후 움직이는 오브젝트
// 필요 기능
// 1. 타이머에 따른 위치 이동
// 2. 현재 길 표시 여부를 받아서 이동 시작

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

	// Player 를 특정 위치로 이동시키는 함수
	void Move(Vector2 pos);

	void ResetSettings();

private:
	// 길 찾았을 시에만 둘 다 활성화
	bool isDraw = false;

	// 타이머 다 차서 한칸 이동해야할 시
	bool isPlay = false;

	// 이동 속도
	float moveTime = 0.3f;

	Timer moveTimer;
};