#ifndef DP_SUTIL_H
#define DP_SUTIL_H

#include "BugBot.h"
#include "clump.h"
#include "mainbrain.h"
#include "food.h"


struct Spot
{
	std::list<BugBot>::iterator bugbot;
	std::list<BugBot>::iterator corpse;
	std::list<Food>::iterator food;
	Spot();
}; 

#endif

