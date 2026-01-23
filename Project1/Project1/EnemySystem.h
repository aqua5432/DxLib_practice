#pragma once
#include "Entity.h"
class EnemySystem { // “G‚ÌˆÚ“®‘¬“xA“G‚ÆƒvƒŒƒCƒ„[‚ÌÚGA“G“¯m‚ÌÚG”»’è
public:
	void Update(std::vector<Enemy>& Enemies, MainCharacter& MainChar, int Sta_PosX) {
		for (auto& e : Enemies)
		{
			float dist = abs(MainChar.Pos.X - e.Pos.X);
			float speed = 1.5f + dist * 0.001f;
			speed = min(speed, 4.0f);

			if (e.Dir == DIR_RI)
				e.Pos.X += e.SpeedX * speed;
			else
				e.Pos.X -= e.SpeedX * speed;

			int camLeft = -Sta_PosX;
			int camRight = -Sta_PosX + WIN_MAX_X;

			if (e.Pos.X <= camLeft)
				e.Dir = DIR_RI;
			if (e.Pos.X >= camRight - CELL)
				e.Dir = DIR_LE;

			/*** “GÚG”»’è‰E ***/
			if ((MainChar.Pos.X - Sta_PosX + CELL >= e.Pos.X + ENEMY_HIT_RANGE) &&
				(MainChar.Pos.X - Sta_PosX + CELL < e.Pos.X + CELL) &&
				(MainChar.Pos.Y >= e.Pos.Y) &&
				(MainChar.Pos.Y < e.Pos.Y + CELL))
			{
				e.Touch = DIR_RI;
			}
			/*** “GÚG”»’è¶ ***/
			else if ((MainChar.Pos.X - Sta_PosX >= e.Pos.X) &&
				(MainChar.Pos.X - Sta_PosX < e.Pos.X + CELL - ENEMY_HIT_RANGE) &&
				(MainChar.Pos.Y >= e.Pos.Y) &&
				(MainChar.Pos.Y < e.Pos.Y + CELL))
			{
				e.Touch = DIR_LE;
			}
		}

		for (int i = 0; i < Enemies.size(); i++)// “G“¯m‚ª‚Ô‚Â‚©‚é‚Æ‹tŒü‚«‚É“®‚«o‚·
		{
			for (int j = i + 1; j < Enemies.size(); j++)
			{
				if (abs(Enemies[i].Pos.X - Enemies[j].Pos.X) < CELL)
				{
					std::swap(Enemies[i].Dir, Enemies[j].Dir);
				}
			}
		}
	}
};