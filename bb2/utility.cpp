#include "utility.h"
#include "BugBot.h"
#include "food.h"
#include "sutil.h"
#include "MapHandler.h"
#include "mainbrain.h"
#include "clump.h"
#include <map>
//#include <iostream>
#include <list>
#include <algorithm>
//#include <fstream.h>
#include <math.h>

const extern int SCREENWIDTH;
const extern int SCREENHEIGHT;


Spot::Spot()
{
}

double DistPTP(const Position& pt1, const Position& pt2)
{
    int dx = abs(pt1.x - pt2.x);
    int dy =  abs(pt1.y - pt2.y);
    return  dx > dy ? dx : dy;
}

Flag::Flag()
{
        for(int i=0;i<=NumWords;i++)
        {
                flag[i]=0;
        }
}

int Flag::SetFlag(int FlagNum)
{
	int FlagVar, ReturnCode;
	long int FlagBit;

	if(FlagNum > MaxFlag)
	{
		ReturnCode = -1;
	}
	else
	{
		FlagVar = FlagNum / 32;
		FlagBit = FlagNumToBit(FlagNum % 32);
		ReturnCode = 0;

		flag[FlagVar] |= FlagBit;
	}

	return ReturnCode;
}

int Flag::RemoveFlag(int FlagNum)
{
	int FlagVar, ReturnCode;
	long int FlagBit;

	if(FlagNum > MaxFlag)
	{
		ReturnCode = -1;
	}
	else
	{
		FlagVar = FlagNum / 32;
		FlagBit = ~FlagNumToBit(FlagNum % 32);
		ReturnCode = 0;

		flag[FlagVar] &= FlagBit;
	}

	return ReturnCode;
}

bool Flag::IsSet(int FlagNum)
{
	int FlagVar;
	long int FlagBit;
	
	if(FlagNum > MaxFlag)
	{
		return false;
	}
	else
	{
		FlagVar = FlagNum / 32;
		FlagBit = FlagNumToBit(FlagNum % 32);
		return flag[FlagVar] & FlagBit ? true : false;
	}
}

long int Flag::FlagNumToBit(int FlagNum)
{
	return 1 << FlagNum;
}

bool Flag::operator[](int FlagNum)
{
	return IsSet(FlagNum);
}

int Flag::operator+=(int FlagNum)
{
	return SetFlag(FlagNum);
}

int Flag::operator-=(int FlagNum)
{
	return RemoveFlag(FlagNum);
}

Position::Position(int param1, int param2)
{
	x = param1;
	y = param2;
}

Position::Position(const Position& pos)
{
	x = pos.x;
	y = pos.y;
}

bool Position::operator==(const Position& operand) const
{
	return (x == operand.x && y == operand.y);
}

Position Position::operator=(const Position& operand)
{
	x = operand.x;
	y = operand.y;
	return operand;
}

void Position::Random()
{
      x = rand() % SCREENWIDTH;
      y = rand() % SCREENHEIGHT;
}


Avoid::Avoid() : Location(NoPosition)
{
	Timer = AvoidLength;
}

Avoid::Avoid(const Position& pos, int time) : Location(pos)
{
	Timer = time;
}

Avoid::Avoid(int x, int y, int time) : Location(x,y)
{
	Timer = time;
}

bool Avoid::operator==(Avoid op)
{
      return (Timer == op.Timer && Location == op.Location);
}

void Avoid::Update()
{
	if(Timer != 0)
	{
		Timer--;
	}
}

bool Avoid::IsOver()
{
	return Timer == 0;
}

Item::Item()
{
	vnum=-1;
	bot = false;
}

Item::Item(int avnum, bool abot)
{
	vnum=avnum;
	bot=abot;
}

Pixel::Pixel()
{
	DistToDest = 0;
}

Pixel::Pixel(int x, int y, double dist)
{
	Location.x = x;
	Location.y = y;
	DistToDest = dist;
}

Pixel::Pixel(const Position& InitLoc, double dist)
{
	Location = InitLoc;
	DistToDest = dist;
}

bool Pixel::operator<(Pixel operand) const
{
	return DistToDest<operand.DistToDest;
}

bool Pixel::operator>(Pixel operand) const
{
	return DistToDest>operand.DistToDest;
}

bool Pixel::operator==(Pixel operand) const
{
	return DistToDest==operand.DistToDest;
}

