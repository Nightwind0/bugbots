#include "utility.h"
#ifndef NP_FOOD_H
#define NP_FOOD_H

#include <list>

class Food
{
private:
	int m_clump;
	Position m_pos;
	bool m_carried;
	int m_me;

public:
	void FoodReport();
	Food();
	void SetMe(int i);
	int GetMe();
	void SetClump(int aclump);
	int GetClump();
	int GetIndex();
	Position GetPos();
	void NoClump();
	void CarryClump();
	void NoCarryClump();
	bool IsCarried();
	void SetPos(const Position& npos, bool replace);
};

#endif

