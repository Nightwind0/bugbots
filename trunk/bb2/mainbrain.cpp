#include "mainbrain.h"
#include "utility.h"
#include "food.h"
#include "clump.h"
#include <list>
#include "sutil.h"
#include <algorithm>
#include <vector>
#include "MapHandler.h"
#include "soundmanager.h"


extern MapHandler MH;

const extern int SCREENHEIGHT;
const extern int SCREENWIDTH;


void MainBrain::SetMe(int i)
{
     m_me = i;
}

int MainBrain::GetMe()
{
     return m_me;
}

MainBrain::MainBrain()
{
     m_resources = 30000;
     m_pos = NoPosition;
     m_me = -1;
}

void MainBrain::SetColors(int c1,int c2, int c3, int c4, int c5)
{
     m_colors[0] = c1;
     m_colors[1] = c2;
     m_colors[2] = c3;
     m_colors[3] = c4;
     m_colors[4] = c5;
}


void MainBrain::ReportCorpse(int corpse)
{
     using std::count;
     
     if(!count(m_corpses.begin(),m_corpses.end(),corpse))
     {
	  m_corpses.push_back(corpse);
     }
}

void MainBrain::ReportFood(int food)
{
     using std::list;
     using std::count;
     
     int clump;
     list<Food>::iterator foodit= MH.GetFoodIter(food);
     
     if(!food) return;
     
     clump = (*foodit).GetClump();
     if(clump == -1 || (*foodit).IsCarried())
     {
	  return;
     }
     if(!count(m_clumps.begin(),m_clumps.end(),clump))
     {
	  m_clumps.push_back(clump);
     }
}


bool MainBrain::InTargetZone(const Position& pos)
{
     return (DistPTP(m_pos,pos) < float(TargetZoneSize)) ? true : false;
}

bool MainBrain::Feed(int bot)
{
     using std::list;
     
     list<BugBot>::iterator i = MH.GetBugBotIter(bot);
     
     if(i == MH.GetBugBotEnd())
     {
	  return false;
     }
     int BotHealth = (*i).GetHealth();
     if(CanFeed(BotHealth))
     {
	  (*i).FoodFromGod(BugBotHealth-BotHealth);
	  m_resources -= (BugBotHealth-BotHealth);
	  return true;
     }
     return false;
}

bool MainBrain::CanFeed(int bothealth)
{
     if(m_resources - (BugBotHealth-bothealth) >= 0)
     {
	  return true;
     }
     else
     {
	  return false;
     }
}

int MainBrain::GetColor(Flag param)
{
     if(param[CORPSE])
     {
	  return m_colors[4];
     }
     if(param[CANNIBAL])
     {
	  if(param[RENEGADE])
	  {
	       return m_colors[3];
	  }
	  else
	  {
	       return m_colors[2];
	  }
     }
     if(param[RENEGADE])
     {
	  return m_colors[1];
     }
     else
     {
	  return m_colors[0];
     }
}

void MainBrain::SweepTargetZone()
{
     using std::list;
     for(int x = (m_pos.x - TargetZoneSize); x <= (m_pos.x + TargetZoneSize); x++)
     {
	  for(int y = (m_pos.y - TargetZoneSize); y <= (m_pos.y + TargetZoneSize); y++)
	  {
	       if(0 < y && y < SCREENHEIGHT && 0 < x && x < SCREENWIDTH)
	       {
		    if(DistPTP(m_pos, Position(x,y)) <= static_cast<float>(TargetZoneSize))
		    {
			 list<BugBot>::iterator Bot = MH.GetBugBotAt(Position(x,y));
			 list<BugBot>::iterator Corpse = MH.GetCorpseAt(Position(x,y));
			 list<Food>::iterator Food = MH.GetFoodAt(Position(x,y));
			 if(Bot != MH.GetBugBotEnd() && (*Bot).GetTeam() == m_me)
			 {
			      (*Bot).ReachedTarget();
			 }
			 if(Corpse != MH.GetBugBotEnd())
			 {
			      Assimilate(Item((*Corpse).GetMe(),true));
			 }
			 if(Food  != MH.GetFoodEnd())
			 {
			      Assimilate(Item((*Food).GetMe(),false));
			 }
		    }
	       }//within range
	  }
     }
}


void MainBrain::Assimilate(Item item)
{
     using std::list;
     if(item.bot)
     {
	  m_resources+=CorpseValue;
	  //if(GlobalBugBotList.count(item.bugbot))
	  list<BugBot>::iterator botit = MH.GetBugBotIter(item.vnum);
	  //GlobalMap[(*GetBugBotIt(item.vnum)).GetPos().x][(*GetBugBotIt(item.vnum)).GetPos().y].corpse=0;
	  MH.SetCorpseAt((*botit).GetPos(),MH.GetBugBotEnd());
	  MH.EraseBugBot(item.vnum);
     }
     else
     {
	  list<Food>::iterator foodit = MH.GetFoodIter(item.vnum);
	  int clump = (*foodit).GetClump();
	  
	  MH.SetFoodAt((*foodit).GetPos(),MH.GetFoodEnd());
	  
	  if(clump != -1 && !(*foodit).IsCarried())
	  {
	       list<Clump>::iterator cit = MH.GetClumpIter(clump);
	       
	       if(cit != MH.GetClumpEnd())
		    (*cit).RemoveFood((*foodit).GetMe());
	  }
	  m_resources+=FoodValue;
	  MH.EraseFood(item.vnum);
     }
}

bool MainBrain::RemoveBugBot(int bot)
{
     using std::count;
     using std::find;
     using std::vector;
     if(!count(m_bugbots.begin(),m_bugbots.end(),bot))
     {
	  return false;
     }
     vector<int>::iterator i = find(m_bugbots.begin(),m_bugbots.end(),bot);
     m_bugbots.erase(i);
     return true;
}

Position MainBrain::GetTargetPos(const Position &current_pos)
{
     using std::list;
     int clump = -1;
     Position pos = NoPosition;
     if(m_corpses.empty())
     {
	  float TotalFood = 0;
	  float rnum = rand();
	  int i;
	  for(i = 0; i < m_clumps.size(); i++)
	  {
	       TotalFood += static_cast<float>((*MH.GetClumpIter(m_clumps[i])).GetFoodCount());
	  }
	  if(m_bugbots.size() > TotalFood)
	  {
	       if(rand() <= static_cast<float>(m_bugbots.size()-TotalFood)/TotalFood)
	       {
		    return NoPosition;
	       }
	  }
	  TotalFood += TotalFood/20.0f;
	  for(i = 0; i < m_clumps.size(); i++)
	  {
	       list<Clump>::iterator cit = MH.GetClumpIter(m_clumps[i]);
	       if((*cit).HasFood())
	       {
		    if(rnum <= static_cast<float>((*cit).GetFoodCount())/TotalFood)
		    {
			 clump = i;
			 break;
		    }
		    else
		    {
			 rnum -= static_cast<float>((*cit).GetFoodCount())/TotalFood;
		    }
	       }
	  }
	  if(clump != -1)
	  {
	       list<Clump>::iterator cit = MH.GetClumpIter(m_clumps[clump]);
	       int food;
	       food = (*cit).GetAFood();
	       list<Food>::iterator foodit = MH.GetFoodIter(food);
	       if(food != -1)
		    pos = (*foodit).GetPos();			
	  }
	  else
	  {
			return NoPosition;
	  }
     }
     else
     {
	  //int bot = *(m_corpses.begin());
	  //list<BugBot>::iterator botit = MH.GetBugBotNearest(current_pos);
	   Position tpos;
	   std::vector<Pixel> tpix;
	   int corpse = -1;

		 for(std::vector<int>::iterator i = m_corpses.begin(); i != m_corpses.end(); i++)
		 {
		   if(MH.GetBugBotIter(*i) != MH.GetBugBotEnd() && MH.GetBugBotIter(*i)->IsCorpse())
			 {

				tpos = MH.GetBugBotIter(*i)->GetPos();
				tpix.push_back(Pixel(tpos,DistPTP(tpos,current_pos)));
			 }
		 }

		 sort(tpix.begin(),tpix.end());

		 pos = tpix.begin()->Location;

		   if(MH.GetCorpseAt(pos) != MH.GetBugBotEnd())
		 {
		 corpse = MH.GetCorpseAt(pos)->GetMe();
		 }


	  if(corpse != -1)
	  {
		  vector<int>::iterator ci = find(m_corpses.begin(),m_corpses.end(),corpse);
		  if( ci != m_corpses.end())
	       m_corpses.erase(ci);
		  return pos;
	  } 
	  else return NoPosition;
     
	 }
     return pos;

}


void MainBrain::CreateBugBot()
{

     if(m_resources > BugBotHealth)
     {

		SoundManager *manager = SoundManager::GetInstance();
		manager->PlaySound(SoundManager::SND_BORN);
	  Position pos = MH.NoBot(m_pos);
	  if(pos == NoPosition)
	       return;
	  
	  m_bugbots.push_back((*MH.NewBugBot(m_me,pos)).GetMe());
	  
	  m_resources-=BugBotHealth;
	  
     }
}

void MainBrain::Update()
{
     //cout << "MB::Upd" << endl;
     CreateBugBot();
     //cout << "done creating bugbot, sweeping targetzone" << endl;
     SweepTargetZone();
     //cout << "L MB::Upd" << endl;
}

bool MainBrain::OwnerOf(int bot)
{
     using std::count;
     return count(m_bugbots.begin(),m_bugbots.end(),bot);
}

int MainBrain::NumberOfBotsControlled()
{
     return m_bugbots.size();
}

Position MainBrain::GetPos()
{
     return m_pos;
}

void MainBrain::SetPos(const Position& pos)
{
     m_pos = pos;
}

void MainBrain::RemoveRefOfCorpse(int bot)
{
     using std::count;
     using std::find;
     using std::vector;
     if(count(m_corpses.begin(),m_corpses.end(),bot))
     {
	  vector<int>::iterator i = find(m_corpses.begin(),m_corpses.end(),bot);
	  m_corpses.erase(i);
     }
}
