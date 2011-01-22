// MapHandler.cpp: implementation of the MapHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "MapHandler.h"
#include "utility.h"
#include "sutil.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MapHandler::MapHandler()
{
    for(int x=0;x<SCREENWIDTH;x++)
    {
	for(int y=0;y<SCREENHEIGHT;y++)
	{
	  SetFoodAt(Position(x,y),GetFoodEnd());
	  SetCorpseAt(Position(x,y),GetBugBotEnd());
	  SetBugBotAt(Position(x,y),GetBugBotEnd());    
	}
    }
}

MapHandler::~MapHandler() {   }

int MapHandler::RegNewBot(std::list<BugBot>::iterator i)
{
    m_BotIts.push_back(i);
    
    return (m_BotIts.size()-1);
}

int MapHandler::RegNewFood(std::list<Food>::iterator i)
{
    m_FoodIts.push_back(i);

    return (m_FoodIts.size()-1);
}

int MapHandler::RegNewClump(std::list<Clump>::iterator i)
{
    m_ClumpIts.push_back(i);

    return (m_ClumpIts.size()-1);
}

int MapHandler::RegNewMainBrain(std::list<MainBrain>::iterator i)
{
    m_MBIts.push_back(i);

    return (m_MBIts.size()-1);
}


std::list<BugBot>::iterator MapHandler::GetBugBotAt(Position pos)
{
    if(WithinSpace(pos))
    {
	return Map[pos.x][pos.y].bugbot;
    }
    else
    {
      return m_BugBotList.end();
    }
}

std::list<BugBot>::iterator MapHandler::GetCorpseAt(Position pos)
{
    if(WithinSpace(pos))
    {
	return Map[pos.x][pos.y].corpse;
    }
    else
    {
      return m_BugBotList.end();
    }
}

std::list<Food>::iterator MapHandler::GetFoodAt(Position pos)
{
    if(WithinSpace(pos))
    {
	return Map[pos.x][pos.y].food;
    }
    else
    {
      return m_FoodList.end();
    }
}

void MapHandler::SetBugBotAt(Position pos, std::list<BugBot>::iterator i)
{
    if(WithinSpace(pos))
    {
	Map[pos.x][pos.y].bugbot = i;
    }
}

void MapHandler::SetCorpseAt(Position pos,std::list<BugBot>::iterator i)
{
    if(WithinSpace(pos))
    {
	Map[pos.x][pos.y].corpse=i;
    }
}

void MapHandler::SetFoodAt(Position pos,std::list<Food>::iterator i)
{
    if(WithinSpace(pos))
    {
	Map[pos.x][pos.y].food=i;
    }
}

bool MapHandler::WithinSpace(Position pos)
{
    if(pos.x > 0 && pos.x < SCREENWIDTH && pos.y >0 && pos.y <SCREENHEIGHT)
    {
	return true;
    }
    else
    {
	return false;
    }
}

std::list<BugBot>::iterator MapHandler::GetBugBotIter(int bot)
{
    if(bot <= m_BotIts.size() - 1 && bot > -1)
	return m_BotIts[bot];
    else
    {
	std::cout << "Tried bot at loc:" << bot << endl;
	
	return m_BugBotList.end();
    }
}

std::list<Food>::iterator MapHandler::GetFoodIter(int food)
{
    if(food <= m_FoodIts.size()-1 && food >-1)
    {
	return m_FoodIts[food];
    }
    else
    {
	//std::cout << "Tried food at loc:" << food << endl;

      return m_FoodList.end();
    }
}

std::list<Clump>::iterator MapHandler::GetClumpIter(int clump)
{
    if(clump <= m_ClumpIts.size()-1 && clump > -1)
    {
	return m_ClumpIts[clump];
    }
    else
    {
	//std::cout << "Tried clump at loc:" << clump << endl;

      return m_ClumpList.end();
    }
}

std::list<MainBrain>::iterator MapHandler::GetMainBrainIter(int mb)
{
    if(mb <= m_MBIts.size()-1 && mb >-1)
    {
	return m_MBIts[mb];
    }
    else
    {
	std::cout << "Tried MB at loc:" << mb << endl;

	return m_MainBrainList.end();
    }
}

bool MapHandler::IsOccupiedByBugBot(Position pos)
{
    if(WithinSpace(pos))
    {
      return (GetBugBotAt(pos)!=m_BugBotList.end());
    }
    return false;
}

bool MapHandler::IsOccupiedByItem(Position pos)
{
    if(WithinSpace(pos))
    {
      return ((GetCorpseAt(pos)!=m_BugBotList.end()) || (GetFoodAt(pos) !=m_FoodList.end()));
    }
    return false;
}

bool MapHandler::IsOccupiedByFood(Position pos)
{
    if(WithinSpace(pos))
    {
      return (GetFoodAt(pos) != m_FoodList.end() );
    }
    return false;
}

bool MapHandler::IsOccupiedByCorpse(Position pos)
{
    if(WithinSpace(pos))
    {
      return (GetCorpseAt(pos) != m_BugBotList.end());
    }
    return false;
}

Position MapHandler::NoFood(Position pos)
{
    using std::vector;
    vector<Pixel> PixelList;
    for(int SquareSize = 0; SquareSize <= 500; SquareSize += 1)
    {
	for(int x = pos.x - SquareSize; x <= pos.x + SquareSize; x++)
	{
	    for(int y = pos.y - SquareSize; y <= pos.y + SquareSize; y++)
	    {
		if(0 < x && x < SCREENWIDTH && 0 < y && y < SCREENHEIGHT)
		{
		    if(!IsOccupiedByFood(Position(x,y)))
		    {
			return Position(x,y);
		    }
		}
			}
	}
    }
    return NoPosition;
}

Position MapHandler::NoBot(Position pos)
{
    using std::vector;
    vector<Pixel> PixelList;
    for(int SquareSize = 0; SquareSize <= 500; SquareSize += 1)
    {
	for(int x = pos.x - SquareSize; x <= pos.x + SquareSize; x++)
	{
	    for(int y = pos.y - SquareSize; y <= pos.y + SquareSize; y++)
	    {
		if(0 < x && x< SCREENWIDTH && 0 < y && y < SCREENHEIGHT)
		{
		    if(!IsOccupiedByBugBot(Position(x,y)))
		    {
			return Position(x,y);
		    }
		}
	    }
	}
    }
    return NoPosition;
}


std::list<BugBot>::iterator MapHandler::NewBugBot(int team, Position pos)
{
    using std::list;
    BugBot b;
    m_BugBotList.push_back(b);
    list<BugBot>::iterator i = m_BugBotList.end();
    --i;
    (*i).SetMe(RegNewBot(i));
    (*i).SetPos(pos);
    (*i).SetTeam(team);
    return i;
}

std::list<MainBrain>::iterator MapHandler::NewMainBrain(Position pos,int c1, int c2, int c3, int c4, int c5)
{
    using std::list;
    MainBrain mb;
    m_MainBrainList.push_back(mb);
    list<MainBrain>::iterator i = m_MainBrainList.end();
    --i;
    (*i).SetMe(RegNewMainBrain(i));
    (*i).SetPos(pos);
    (*i).SetColors(c1,c2,c3,c4,c5);
    return i;
}

std::list<Clump>::iterator MapHandler::NewClump(Position pos, int spawnrate)
{
    using std::list;
    Clump c;
    m_ClumpList.push_back(c);
    list<Clump>::iterator i = m_ClumpList.end();
    --i;
    (*i).SetMe(RegNewClump(i));
    (*i).SetPos(pos);
    (*i).SetSpawnRate(spawnrate);
    return i;
}

std::list<Food>::iterator MapHandler::NewFood(int clump, Position pos)
{
    using std::list;
    Food f;
    m_FoodList.push_back(f);
    std::list<Food>::iterator i = m_FoodList.end();
    --i;
    (*i).SetMe(RegNewFood(i));
    (*i).SetPos(pos,false);
    (*i).SetClump(clump);
    return i;
}

void MapHandler::EraseBugBot(int bot)
{
    using std::list;
    // a bot MUST be dead before it can be erased
    list<BugBot>::iterator botit = GetBugBotIter(bot);
    if( botit == m_BugBotList.end() )
    {
	//std::cout << "Failed to Erase bot at:" << bot << endl;

	return;
    }
    for(int x=0;x<SCREENWIDTH;x++)
    {
	for(int y=0;y<SCREENHEIGHT;y++)
	{
	    if(Map[x][y].corpse == botit)
	    {
	      Map[x][y].corpse= m_BugBotList.end();
	    }
	}
    }
    for(std::list<MainBrain>::iterator i=m_MainBrainList.begin();i!= m_MainBrainList.end();i++)
    {
	(*i).RemoveRefOfCorpse(bot);
    }
    for(std::list<BugBot>::iterator v=m_BugBotList.begin();v!=m_BugBotList.end();v++)
    {
	(*v).RemoveRefOfBot(bot);
    }
    m_BugBotList.erase(botit);
}

void MapHandler::EraseFood(int food)
{
    using std::list;
    list<Food>::iterator foodit = GetFoodIter(food);
    if(foodit==m_FoodList.end())
    {
	//std::cout << "Failed to Erase food at:" << food << endl;
	
	return;
    }
    for(int x=0;x<SCREENWIDTH;x++)
    {
	for(int y=0;y<SCREENHEIGHT;y++)
	{
	    if(Map[x][y].food == foodit)
	    {
	      Map[x][y].food=m_FoodList.end();
	    }
	}
    }
    for(list<BugBot>::iterator v=m_BugBotList.begin();v!=m_BugBotList.end();v++)
    {
	(*v).RemoveRefOfFood(food);
    }
    m_FoodList.erase(foodit);
}

void MapHandler::CorrectMap()
{
	return;  // WOOP 

	//This is a kluge we SHOULDNT need! damnit!!

#if 0

    using std::list;
    int i, j;
    Position pos;
    for(i = 0; i < SCREENWIDTH; i++)
    {
	for(j = 0; j < SCREENHEIGHT; j++)
	{
	    Map[i][j].bugbot = 0;
	    Map[i][j].corpse = 0;
	    Map[i][j].food = 0;
	}
    }
    for(list<BugBot>::iterator BotIter = m_BugBotList.begin(); BotIter != m_BugBotList.end(); BotIter++)
    {
	pos = (*BotIter).GetPos();
	if(WithinSpace(pos))
	{
	    if((*BotIter).IsCorpse())
	    {
		Map[pos.x][pos.y].corpse = BotIter;
	    }
	    else
	    {
		Map[pos.x][pos.y].bugbot = BotIter;
	    }
	}
    }
    for(list<Food>::iterator FoodIter = m_FoodList.begin(); FoodIter != m_FoodList.end(); FoodIter++)
    {
	pos = (*FoodIter).GetPos();
	if(WithinSpace(pos))
	{
	    Map[pos.x][pos.y].food = FoodIter;
	}
    }

#endif
}


#if 0
std::list<BugBot>::iterator MapHandler::GetBugBotNearest(const Position &pos)
{
	std::vector<Pixel> bots;
	Position pos2;

	for(list<BugBot>::iterator BotIter = m_BugBotList.begin(); BotIter != m_BugBotList.end(); BotIter++)
	{
		if(!BotIter->IsCorpse())
		{
			pos2  = BotIter->GetPos();
			bots.push_back(Pixel(pos,DistPTP(pos,pos2)));

		}
	}

	std::sort(bots.begin(),bots.end());


	return bots.begin();

}

std::list<Clump>::iterator MapHandler::GetClumpNearest(const Position &pos)
{
	std::vector<Pixel> bots;
	Position pos2;

	for(list<Clump>::iterator ClumpIter = m_ClumpList.begin(); ClumpIter != m_ClumpList.end(); ClumpIter++)
	{
			pos2  = BotIter->GetPosition();
			bots.push_back(Pixel(pos,DistPTP(pos,pos2)));

		
	}

	std::sort(bots.begin(),bots.end());


	return bots.begin();
}

std::list<BugBot>::iterator MapHandler::GetCorpseNearest(const Position &pos)
{
	std::vector<Pixel> bots;
	Position pos2;

	for(list<BugBot>::iterator BotIter = m_BugBotList.begin(); BotIter != m_BugBotList.end(); BotIter++)
	{
		if(BotIter->IsCorpse())
		{
			pos2  = BotIter->GetPos();
			bots.push_back(Pixel(pos,DistPTP(pos,pos2)));

		}
	}

	std::sort(bots.begin(),bots.end());


	return bots.begin();
}

#endif

