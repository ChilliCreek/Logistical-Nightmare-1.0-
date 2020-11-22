#pragma once
#include"pch.h"
#include"Unit.h"
#include<iostream>
const float unit::modifiers[5][5] = { {0.6,0.45,1.1,1.25,1.5}, {0.9,0.5,0.7,0.85,1.05}, {0.5,0.85,0.95,1.2,1.5}, {0.4,0.8,0.75,0.9,1.05}, {0.2,0.6,0.55,0.7,1} };

int unit::typeToNum(unitType inp)
{
	if (inp == unitType::INF)return 0;
	else if (inp == unitType::AT)return 1;
	else if (inp == unitType::LT)return 2;
	else if (inp == unitType::MT)return 3;
	else if (inp == unitType::HT)return 4;
	return -1;
}

void unit::create(int g,bool side)
{
	if (g == 0)m_type = unitType::INF;
	else if (g == 1)m_type = unitType::AT;
	else if (g == 2)m_type = unitType::LT;
	else if (g == 3)m_type = unitType::MT;
	else if (g == 4)m_type = unitType::HT;
	m_strength = 1000.0;
	m_leftOrNot = side;
	m_movedOrNot = true;
}

void unit::move(unit & targetTile)
{
	unitType temp_type = m_type;
	float temp_strength = m_strength;
	m_strength = targetTile.getStrength();
	m_type = targetTile.getType();
	targetTile.setStrength(temp_strength);
	targetTile.setType(temp_type);
	if (targetTile.getLeftOrNot() != m_leftOrNot) {
		targetTile.setLeftOrNot(m_leftOrNot);
	}
	m_movedOrNot = true;
	targetTile.setMovedOrNot(true);
}

//gets called when there is an emeny unit
void unit::battle(unit & targetTile)
{
	float attackStrength;
	float modifier = modifiers[unit::typeToNum(m_type)][unit::typeToNum(targetTile.getType())];
	attackStrength = fmin(100.0, m_strength);
	if (m_strength <= attackStrength * modifier) {
		m_strength = 1000.0;
		m_type = unitType::EMPTY;
	}
	else {
		m_strength -= attackStrength * modifier;
	}
	if (targetTile.getStrength() <= attackStrength) {
		targetTile.setType(unitType::EMPTY);
		targetTile.setStrength(1000.0);
	}
	else {
		targetTile.setStrength(targetTile.getStrength() - attackStrength);
	}
	m_movedOrNot = true;
	targetTile.setMovedOrNot(true);
}


float unit::getStrength()
{
	return m_strength;
}

unitType unit::getType()
{
	return m_type;
}

bool unit::getLeftOrNot()
{
	return m_leftOrNot;
}

void unit::setLeftOrNot(bool side)
{
	m_leftOrNot = side;
}

void unit::setStrength(float strength)
{
	m_strength = strength;
}

void unit::setType(unitType type)
{
	m_type = type;
}

void unit::setMovedOrNot(bool inp)
{
	m_movedOrNot = inp;
}

bool unit::getMovedOrNot()
{
	return m_movedOrNot;
}
