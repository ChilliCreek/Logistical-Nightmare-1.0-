#pragma once
#include"pch.h"
#include<algorithm>
enum class unitType { EMPTY = -1, INF = 0, AT = 1, LT = 2, MT = 3, HT = 4 };


class unit {
private:
	static const float modifiers[5][5];
	bool m_leftOrNot;
	unitType m_type = unitType::EMPTY;
	float m_strength = 1000.0;
	bool m_movedOrNot;
public:
	static int typeToNum(unitType inp);
	void create(int g,bool side);
	void move(unit& targetTile);
	void battle(unit& targetTile);
	float getStrength();
	unitType getType();
	bool getLeftOrNot();
	void setLeftOrNot(bool side);
	void setStrength(float strength);
	void setType(unitType type);
	void setMovedOrNot(bool inp);
	bool getMovedOrNot();
};