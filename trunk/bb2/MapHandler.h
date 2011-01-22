// MapHandler.h: interface for the MapHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPHANDLER_H__2E8B88C0_68ED_11D2_9794_00104B3461CE__INCLUDED_)
#define AFX_MAPHANDLER_H__2E8B88C0_68ED_11D2_9794_00104B3461CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "utility.h"
#include "sutil.h"
#include "BugBot.h"
#include "food.h"
#include "clump.h"
#include "mainbrain.h"


const int SCREENWIDTH = 320;
const int SCREENHEIGHT = 200;

class MapHandler  
{
private:
	std::list<BugBot> m_BugBotList;
	std::list<MainBrain> m_MainBrainList;
	std::list<Clump> m_ClumpList;
	std::list<Food> m_FoodList;
	std::vector<std::list<MainBrain>::iterator> m_MBIts;
	std::vector<std::list<BugBot>::iterator> m_BotIts;
	std::vector<std::list<Food>::iterator> m_FoodIts;
	std::vector<std::list<Clump>::iterator> m_ClumpIts;

	int RegNewBot(std::list<BugBot>::iterator i);
	int RegNewFood(std::list<Food>::iterator i);
	int RegNewClump(std::list<Clump>::iterator i);
	int RegNewMainBrain(std::list<MainBrain>::iterator i);
	Spot Map[SCREENWIDTH][SCREENHEIGHT];
public:
	void CorrectMap();
	MapHandler();
	std::list<BugBot>::iterator GetBugBotAt(Position pos);
	std::list<BugBot>::iterator GetCorpseAt(Position pos);
	std::list<Food>::iterator GetFoodAt(Position pos);

	void SetBugBotAt(Position pos, std::list<BugBot>::iterator i);
	void SetCorpseAt(Position pos, std::list<BugBot>::iterator i);
	void SetFoodAt(Position pos, std::list<Food>::iterator i);

	bool WithinSpace(Position pos);

	std::list<BugBot>::iterator GetBugBotIter(int bot);
	std::list<BugBot>::const_iterator GetBugBotEnd() const 
	  { return m_BugBotList.end(); }
	std::list<Food>::iterator GetFoodIter(int food);
	std::list<Food>::const_iterator GetFoodEnd() const
	  { return m_FoodList.end();  }
	std::list<Clump>::iterator GetClumpIter(int clump);
	std::list<Clump>::const_iterator GetClumpEnd() const
	  { return m_ClumpList.end(); }
	std::list<MainBrain>::iterator GetMainBrainIter(int mb);
	std::list<MainBrain>::const_iterator GetMainBrainEnd() const
	  { return m_MainBrainList.end(); }

	bool IsOccupiedByBugBot(Position pos);
	bool IsOccupiedByItem(Position pos);
	bool IsOccupiedByFood(Position pos);
        bool IsOccupiedByCorpse(Position pos);

	Position NoFood(Position pos);
	Position NoBot(Position pos);

	std::list<BugBot>::iterator NewBugBot(int team, Position pos);
	std::list<MainBrain>::iterator NewMainBrain(Position pos,int c1,int c2, int c3, int c4,int c5);
	std::list<Food>::iterator NewFood(int clump, Position pos);
	std::list<Clump>::iterator NewClump(Position pos, int spawnrate);

	void EraseBugBot(int bot);
	void EraseFood(int food);
	virtual ~MapHandler();

	std::list<BugBot>::iterator GetBugBotBegin(){return m_BugBotList.begin();}
	std::list<BugBot>::iterator GetBugBotEnd(){return m_BugBotList.end();}
	std::list<Food>::iterator GetFoodBegin(){return m_FoodList.begin();}
	std::list<Food>::iterator GetFoodEnd(){return m_FoodList.end();}
	std::list<MainBrain>::iterator GetMainBrainBegin(){return m_MainBrainList.begin();}
	std::list<MainBrain>::iterator GetMainBrainEnd(){return m_MainBrainList.end();}
	std::list<Clump>::iterator GetClumpBegin(){return m_ClumpList.begin();}
	std::list<Clump>::iterator GetClumpEnd(){return m_ClumpList.end();}

	std::list<BugBot>::iterator GetBugBotNearest(const Position &pos);
	std::list<Clump>::iterator GetClumpNearest(const Position &pos);
	std::list<BugBot>::iterator GetCorpseNearest(const Position &pos);

	



};

#endif // !defined(AFX_MAPHANDLER_H__2E8B88C0_68ED_11D2_9794_00104B3461CE__INCLUDED_)

