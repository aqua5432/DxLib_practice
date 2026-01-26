#pragma once
#include <vector>
#include "Entity.h"
#include "../core/InputSystem.h"

class Action
{
public:
	Action();

	/*** Actシーン本処理 ***/
	int Out();
private:
	void Update();
	void Judge();

	/*** 移動計算 ***/
	void Cal();
	//描画計算
	void Draw();

	int NextScene(int ret);

	/*** 初期化 ***/
	void Init();

	void UpdateDirection(const InputState& in);
	void CheckBlockCollision();

private:
	int Sta_PosX;

	MainCharacter MainChar;
	std::vector<Enemy> Enemies;
	MoveState Mov;

	class Renderer* renderer;
	class Physics* physics;
	class EnemySystem* enemySystem;
	class InputSystem* inputSystem;

	int Goal;
	int EndFlag;
	int ClearWait;
	bool IsClearing;

};