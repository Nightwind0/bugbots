#include "utility.h"
#include "BugBot.h"
#include "food.h"
#include "clump.h"
#include "mainbrain.h"
#include "soundmanager.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <map>
#include <list>


#include "sutil.h"
#include "MapHandler.h"


const extern int SCREENWIDTH;
const extern int SCREENHEIGHT;

extern MapHandler MH;

BugBot::BugBot()
{
    m_dest = NoPosition;
    m_health = 1000;
    m_targetbot = -1;
    m_me = -1;
	starved = false;
}

void BugBot::CheckHunger()
{

    if(m_health < 100)
    {
	m_flags += HUNGRY;
	if(HasItem() && !m_item.bot || (HasItem() && m_item.bot && IsCannibal()))
	{
	    Eat(m_item);
	    m_dest = (*MH.GetMainBrainIter(m_team)).GetTargetPos(m_pos);
	    if(m_dest == NoPosition)
	    {
		m_dest.Random();
		m_flags += SEARCHING;
	    }
	    if(m_health >= 100)
	    {
		m_flags -= HUNGRY;
	    }
	}
	if(MH.IsOccupiedByItem(m_pos))
	{
	    std::list<BugBot>::iterator Corpse = MH.GetCorpseAt(m_pos);
	    std::list<Food>::iterator Food = MH.GetFoodAt(m_pos);
	    if(Food != MH.GetFoodEnd())
	    {
		Eat(Item((*Food).GetMe(), false));
	    }
	    if(Corpse != MH.GetBugBotEnd() && IsCannibal())
	    {
		Eat(Item((*Corpse).GetMe(), true));
	    }
	}
    }
    else
    {
	m_flags -= HUNGRY;
    }
    if(m_health < 40)
    {
	m_flags += STARVING;
	if(HasItem())
	{
	    Eat(m_item);
	    m_dest = (*MH.GetMainBrainIter(m_team)).GetTargetPos(m_pos);
	    if(m_dest == NoPosition)
	    {
		m_dest.Random();
		m_flags += SEARCHING;
	    }
	}
	if(MH.IsOccupiedByItem(m_pos))
	{
	    std::list<BugBot>::iterator Corpse = MH.GetCorpseAt(m_pos);
	    std::list<Food>::iterator Food = MH.GetFoodAt(m_pos);
	    if(Food != MH.GetFoodEnd())
	    {
		Eat(Item((*Food).GetMe(), false));
	    }
	    if(Corpse != MH.GetBugBotEnd())
	    {
		Eat(Item((*Corpse).GetMe(), true));
	    }
	}
    }
    else
    {
	m_flags -= STARVING;
    }
    if(m_health <=0)
    {
	if(HasItem())
	{
	    Eat(m_item);
	    m_dest = (*MH.GetMainBrainIter(m_team)).GetTargetPos(m_pos);
	    if(m_dest == NoPosition)
	    {
		m_dest.Random();
		m_flags += SEARCHING;
	    }
	}
	if(MH.IsOccupiedByItem(m_pos))
	{
	    std::list<BugBot>::iterator Corpse = MH.GetCorpseAt(m_pos);
	    std::list<Food>::iterator Food = MH.GetFoodAt(m_pos);
	    if(Food != MH.GetFoodEnd())
	    {
		Eat(Item((*Food).GetMe(), false));
	    }
	    if(Corpse != MH.GetBugBotEnd())
	    {
		Eat(Item((*Corpse).GetMe(), true));
	    }
	}
	if(m_health <= 0)
	{
		starved = true;
	    Die();
	}
    }
    if(m_health >= 40)
    {
	m_flags -= STARVING;
	if(m_health >= 100)
	{
	    m_flags -= HUNGRY;
	}
    }
}

void BugBot::CheckFollow()
{
    if(m_targetbot != -1)
    {
	std::list<BugBot>::iterator i = MH.GetBugBotIter(m_targetbot);
	if(i==MH.GetBugBotEnd()) 
	{	
	    //std::cout << "CheckFollow() Invalid target:" << m_targetbot << endl;

	    m_targetbot=-1;
	    return;
	}
	
	m_dest =(*i).GetPos();
    }
}

void BugBot::CheckDest()
{
    // //std::cout << "My team is " << m_team << endl;
    if(m_dest == m_pos)
    {
	m_dest = NoPosition; //clear m_dest when arrived
	m_flags -= SEARCHING;
    }//m_dest == mpos
    
    
    if(m_dest == NoPosition)
    {
	if(HasItem() && !IsRenegade())
	{
	    std::list<MainBrain>::iterator mbit= MH.GetMainBrainIter(m_team);
	    
	    if(mbit!=MH.GetMainBrainEnd())
		m_dest = (*mbit).GetPos(); //head towards mainbrain
	}
	if(!HasItem() && !IsRenegade())// && !m_flags[SEARCHING])
	{
	    std::list<MainBrain>::iterator mbit= MH.GetMainBrainIter(m_team);
	    
	    m_dest = (*mbit).GetTargetPos(m_pos);
	    
	    if(m_dest == NoPosition) //mainbrain doesnt know
	    {
		m_flags.SetFlag(SEARCHING);
		m_dest.Random();
	    }
	    else
	    {
		m_flags -= SEARCHING;
	    }
	}
	if(IsRenegade())
	{
	    //m_targetbot = MH.GetRandomBot()->GetMe();
	}
    }
    if(m_flags[SEARCHING] && !IsRenegade())
    {
	std::list<MainBrain>::iterator mbit = MH.GetMainBrainIter(m_team);
	
	if(!m_team) return;
	
	Position tpos = (*mbit).GetTargetPos(m_pos);
	if(tpos != NoPosition)
	{
	    m_dest = tpos;
	    m_flags -= SEARCHING;
	}
    }

	if(m_dest == NoPosition)
	{
		m_flags.SetFlag(SEARCHING);
		m_dest.Random();
	}
}

bool BugBot::MoveToDest()
{
    
    Position tmp;
    Pixel Location;
    vector<Pixel> Points;
    
    Location.Location = m_pos;
    Location.DistToDest = DistPTP(m_pos, m_dest);
    
    tmp = m_pos;
    
    tmp.x++;
    if(tmp.x < SCREENWIDTH && tmp.y < SCREENHEIGHT && tmp.x > 0 && tmp.y >0)
    {
	Points.push_back(Pixel(tmp, DistPTP(m_dest, tmp)));
    }
    tmp.x--;
    
    tmp.x--;
    if(tmp.x < SCREENWIDTH && tmp.y < SCREENHEIGHT && tmp.x > 0 && tmp.y >0)
    {
	Points.push_back(Pixel(tmp, DistPTP(m_dest, tmp)));
    }
    tmp.x++;
    
    tmp.y++;
    if(tmp.x < SCREENWIDTH && tmp.y < SCREENHEIGHT && tmp.x > 0 && tmp.y >0)
    {
	Points.push_back(Pixel(tmp, DistPTP(m_dest, tmp)));
    }
    tmp.y--;
    
    tmp.y--;
    if(tmp.x < SCREENWIDTH && tmp.y < SCREENHEIGHT && tmp.x > 0 && tmp.y >0)
    {
	Points.push_back(Pixel(tmp, DistPTP(m_dest, tmp)));
    }
    tmp.y++;
    
    sort(Points.begin(),Points.end());
    
    for(int i = 0; i < Points.size(); i++ )
    {
	if(!IsCorpse())
	{
	    if( Move(Points[i].Location) )
	    {
		if( Points[i] > Location ) // we moved away from our target.
		{
		    m_avoids.push_back(Avoid( Location.Location, AvoidLength ) );
		}
		if(MH.IsOccupiedByItem(Points[i].Location))
		{
		    ItemInPath(Points[i].Location);
		}
		break;
	    }
	}
	else
	{
	    break;
	}
    }
    if(HasItem())
    {
	if(!m_item.bot)
	{
	    (*MH.GetFoodIter(m_item.vnum)).SetPos(m_pos,true);
	    
	}
	else
	{
	    std::list<BugBot>::iterator it = MH.GetBugBotIter(m_item.vnum);
	    if( it == MH.GetBugBotEnd() )
	    {
		std::cout << "MoveToDest() Bad item vnum:" << m_item.vnum << endl;
	    }
	    else
	    {
		(*it).SetPos(m_pos,true);
	    }
	}
    }
    if(IsCorpse())
    {
	return false;
    }
    else
    {
	return true;
    }
}


bool BugBot::Move(const Position& pos)
{
    if(pos.x < 0 || pos.x > SCREENWIDTH || pos.y < 0 || pos.y > SCREENHEIGHT)
    {
	return false;
    }
    for(std::vector<Avoid>::iterator i = m_avoids.begin();i != m_avoids.end();i++)
    {
	if( (*i).Location == pos )
	{
	    return false;
	}
    }

	if(MH.IsOccupiedByCorpse(pos) && HasItem() && m_item.bot)
	{
		m_avoids.push_back(Avoid(pos, AvoidLength));
		return false;
	}

    if(MH.IsOccupiedByBugBot(pos))
    {
	if(BotInPath(pos))
	{
	    return false;
	}
    }
    if(!IsCorpse())
    {
		SetPos(pos,true); // WOOP
		return true;
    }
    else
    {
		return false;
    }
}

bool BugBot::BotInPath(const Position& pos)
{
    const bool can = IsCannibal();
    const bool ren = IsRenegade();
    //const bool food = HasItem();
    const bool hungry = IsHungry();
    const bool starving = IsStarving();
    const std::list<BugBot>::iterator BotIt = MH.GetBugBotAt(pos);
    
    //if(!BotIt) return false;
    
    const bool SameTeam = (m_team == (*BotIt).GetTeam());
    
    if(BotIt == MH.GetBugBotEnd())
    {
	return false;
    }
    
    if(IsCorpse())
    {
	return true;
    }
    if(SameTeam)
    {
	if(!ren)
	{
	    return true;
	}
	else
	{


	    if(Attack((*BotIt).GetMe()))
	    {
		if(starving || (can && hungry))
		{
		    Eat(Item((*BotIt).GetMe(),true));
		}
		return false;
	    }
	}
    }
    else
    {
	if(Attack((*BotIt).GetMe()))
	{
	    if(starving || (can && hungry))
	    {
		Eat(Item((*BotIt).GetMe(),true));
	    }
	    return false;
	}
    }
    return true;
}

void BugBot::ItemInPath(const Position& pos)
{
    std::list<Food>::iterator food = MH.GetFoodAt(pos);
    std::list<BugBot>::iterator corpse = MH.GetCorpseAt(pos);
    
    if(food != MH.GetFoodEnd())//food in path
    {
		if(!IsRenegade())
		{
			std::list<MainBrain>::iterator mbit = MH.GetMainBrainIter(m_team);
	    
			if(mbit!=MH.GetMainBrainEnd())
			(*mbit).ReportFood((*food).GetMe());
		}
		if(HasItem())
		{
			return;
		}
		if(IsHungry() || IsStarving())
		{
			Eat(Item((*food).GetMe(),false));
			return;
		}
		else
		{
			if(!(m_flags.IsSet(SEARCHING)) && (m_dest != pos))
			{
				return;
			}
			PickUp(Item((*food).GetMe(),false));
		}
    }
    if(corpse != MH.GetBugBotEnd() && !(*corpse).IsCarried())//corpse
    {
		if(HasItem() && (m_item.bot==false) )
		{
			
			if(!MH.IsOccupiedByFood(pos))
			{
			DropItem();
			PickUp(Item((*corpse).GetMe(),true));
			}
			return;
		}
		if(HasItem())
		{
			if(!IsRenegade())
			{
			std::list<MainBrain>::iterator mbit = MH.GetMainBrainIter(m_team);
			
			if(mbit!=MH.GetMainBrainEnd())
				(*mbit).ReportCorpse((*corpse).GetMe());
			}
			return;
		}
		if(IsStarving() || (IsHungry() && IsCannibal()))
		{
			Eat(Item((*corpse).GetMe(),true));
			return;
		}
		if(!(m_flags.IsSet(SEARCHING)) && (m_dest != pos))
		{
			if(!IsRenegade())
			{
			std::list<MainBrain>::iterator mbit = MH.GetMainBrainIter(m_team);
			
			if(mbit!=MH.GetMainBrainEnd())
				(*mbit).ReportCorpse((*corpse).GetMe());
			}
			return;
		}
		if(!HasItem())
		PickUp(Item((*corpse).GetMe(),true));
	}
}

void BugBot::AreaScan()
{
    std::vector<Item> Objects;
    std::vector<int> Food;
    std::vector<int> BotSameTeam;
    std::vector<int> BotDifferentTeam;
    std::vector<int> Corpse;
    
    Objects = Radar();
    
    if(!Objects.empty())
    {
	for(std::vector<Item>::iterator i = Objects.begin(); i != Objects.end(); i++)
	{
	    if((*i).bot && MH.GetBugBotIter((*i).vnum) != MH.GetBugBotEnd())
	    {
			if( (*MH.GetBugBotIter((*i).vnum)).IsCorpse() && !(MH.GetBugBotIter(i->vnum)->GetFlags().IsSet(CARRIED)))
			{
				Corpse.push_back((*i).vnum);
			}
			else if((*MH.GetBugBotIter((*i).vnum)).GetTeam() == m_team)
			{
				BotSameTeam.push_back((*i).vnum);
			}
			else
			{
				BotDifferentTeam.push_back((*i).vnum);
			}
	    }
	    else
	    {
			if((*MH.GetFoodIter((*i).vnum)).GetClump() != -2)
			{
				Food.push_back((*i).vnum);
			}
	    }
	}
	if(!Corpse.empty())
	{
	    for(std::vector<int>::iterator i = Corpse.begin(); i != Corpse.end(); i++)
	    {
		(*MH.GetMainBrainIter(m_team)).ReportCorpse(*i);
	    }
	    if(!HasItem() && m_flags[SEARCHING])
	    {
		std::vector<Pixel> CorpseSpots;
		Position tmp;
		for(std::vector<int>::iterator i = Corpse.begin(); i != Corpse.end(); i++)
		{
		    std::list<BugBot>::iterator it = MH.GetBugBotIter(*i);
		    if( it == MH.GetBugBotEnd() )
		    {
			//std::cout << "AreaScan, Bad corpse" << endl;
		    }
		    else
		    {
			tmp = (*it).GetPos();
			CorpseSpots.push_back(Pixel(tmp, DistPTP(tmp, m_pos)));
		    }
		}
		std::sort(CorpseSpots.begin(), CorpseSpots.end());
		std::vector<Pixel>::iterator vi = CorpseSpots.begin();
		m_dest = (*vi).Location;
		m_flags -= SEARCHING; // VLOOP
	    }
	}
	if(!Food.empty())
	{
	    for(std::vector<int>::iterator i = Food.begin(); i != Food.end(); i++)
	    {
		(*MH.GetMainBrainIter(m_team)).ReportFood(*i);
	    }
	    if(m_item.vnum == -1 && m_flags[SEARCHING])
	    {
		std::vector<Pixel> FoodSpots;
		Position tmp;
		for(std::vector<int>::iterator i = Food.begin(); i != Food.end(); i++)
		{
		    tmp = (*MH.GetFoodIter(*i)).GetPos();
		    FoodSpots.push_back(Pixel(tmp, DistPTP(tmp, m_pos)));
		}
		std::sort(FoodSpots.begin(), FoodSpots.end());
		std::vector<Pixel>::iterator vi2 = FoodSpots.begin();
		m_dest = (*vi2).Location;
		m_flags -= SEARCHING; //VLOOP
	    }
	}
	if(!BotSameTeam.empty())
	{
	    Position tmp;
	    std::vector<Pixel> BotPos;
	    if(m_flags[RENEGADE])
	    {
			for(std::vector<int>::iterator i = BotSameTeam.begin(); i != BotSameTeam.end(); i++)
			{
		   // tmp = (MH->GetMainBrainIter(*i))->GetPos();
				tmp = (MH.GetBugBotIter(*i))->GetPos();
				if(MH.GetBugBotAt(tmp) == MH.GetBugBotEnd() )
				{
					std::cout << "BUGBOT LOCATION MISMATCH!" << endl;
				}
				if(tmp != m_pos)
				BotPos.push_back(Pixel(tmp, DistPTP(tmp, m_pos)));
			}

			if(BotPos.size())
			{
				std::sort(BotPos.begin(), BotPos.end());
				m_dest = (BotPos.begin())->Location;
				if(MH.GetBugBotAt(m_dest) != MH.GetBugBotEnd())
				m_targetbot = (MH.GetBugBotAt(m_dest))->GetMe();
				else std::cout << "RENEGADE: Tried bugbot at " << m_dest.x << ',' << m_dest.y << endl;
			}
			


	    }
	}
	if(m_flags[HUNGRY] && m_flags[SEARCHING])
	{
	    Position tmp;
	    if(Food.empty() && Corpse.empty())
	    {
		if((*MH.GetMainBrainIter(m_team)).CanFeed(m_health))
		{
		    m_dest = (*MH.GetMainBrainIter(m_team)).GetPos();
		    m_flags -= SEARCHING;
		}
		else if((tmp = (*MH.GetMainBrainIter(m_team)).GetTargetPos(m_pos)) != NoPosition)
		{
		    m_dest = tmp;
		    m_flags -= SEARCHING;
		}
		else
		{
		    m_dest.Random();
		}
	    }
	}
    }
}

std::vector<Item> BugBot::Radar()
{
    std::vector<Item> ItemList;
    std::list<BugBot>::iterator Bot;
    std::list<BugBot>::iterator Corpse;
    std::list<Food>::iterator Food;
    
    for(int x = m_pos.x - RadarDist; x <= m_pos.x + RadarDist; x++)
    {
		for(int y = m_pos.y - RadarDist; y <= m_pos.y + RadarDist; y++)
		{
			if(DistPTP(m_pos, Position(x,y)) <= float(RadarDist))
			{
			  if((Bot = MH.GetBugBotAt(Position(x, y))) != MH.GetBugBotEnd())
				{
					ItemList.push_back(Item((*Bot).GetMe(), true));
				}
			  if((Corpse = MH.GetCorpseAt(Position(x, y))) != MH.GetBugBotEnd())
				{
					ItemList.push_back(Item((*Corpse).GetMe(), true));
				}
			  if((Food = MH.GetFoodAt(Position(x, y))) != MH.GetFoodEnd())
				{
					ItemList.push_back(Item((*Food).GetMe(), false));
				}
			}
		}
    }
    return ItemList;
}


void BugBot::ReachedTarget()
{
    if(IsRenegade())
    {
	return;
    }
    if(HasItem())
    {
	if((m_item.bot && !MH.IsOccupiedByCorpse(m_pos)) || (!m_item.bot && !MH.IsOccupiedByFood(m_pos)))
	{
	    DropItem();
	    std::list<MainBrain>::iterator mbit = MH.GetMainBrainIter(m_team);
	    
	    if(mbit!= MH.GetMainBrainEnd())
		m_dest = (*mbit).GetTargetPos(m_pos);

		if(m_dest == NoPosition)
		{
			m_dest.Random();
		}

	}
    }
    if(IsHungry() || IsStarving())
    {
	std::list<MainBrain>::iterator mbit = MH.GetMainBrainIter(m_team);
	
	if(mbit!=MH.GetMainBrainEnd())
	    (*mbit).Feed(m_me);
    }
}

void BugBot::PrepareForAttack()
{
    if(MH.IsOccupiedByItem(m_pos))
    {
	std::list<BugBot>::iterator corpse = MH.GetCorpseAt(m_pos);
	std::list<Food>::iterator food = MH.GetFoodAt(m_pos);
	
	if(corpse != MH.GetBugBotEnd())
	{
	    Eat(Item((*corpse).GetMe(),true));
	}
	if(food != MH.GetFoodEnd())
	{
	    Eat(Item((*food).GetMe(),false));
	}
    }
    if(HasItem())
    {
		Eat(m_item);
		m_item.vnum = -1;
    }
}

bool BugBot::Attack(int bot)
{

	SoundManager *manager = SoundManager::GetInstance();
	manager->PlaySound(SoundManager::SND_ATTACK);
    std::list<BugBot>::iterator botit = MH.GetBugBotIter(bot);

    if( botit == MH.GetBugBotEnd() )
    {
	//std::cout << "Attack, bad bot:" << bot << endl;
    }
    else
    {
	PrepareForAttack();
	(*botit).PrepareForAttack();
	if(m_health < (*botit).GetHealth())
	{
	//	std::cout << "Other died in attack." << endl;
	    Die();
	    return false;
	}
	else
	{
	    //you win
	    (*botit).Die();
	   
	    return true;
	}
    }
	return false;
}

void BugBot::Eat(Item item)
{
	if(item.vnum == -1)
	{
		std::cout << "Tried to eat something you didnt have! Why??" << endl;
		return;
	}
    if(!item.bot)
    {
	//food

	SoundManager *manager = SoundManager::GetInstance();
	manager->PlaySound(SoundManager::SND_EAT);
		std::list<Food>::iterator foodit = MH.GetFoodIter(item.vnum);
		
		if(foodit==MH.GetFoodEnd())
		{
			return;
		}
		
		int clump = (*foodit).GetClump();
		MH.SetFoodAt((*foodit).GetPos(),MH.GetFoodEnd());
		
		
		if(clump != -1 && !(*foodit).IsCarried())
		{
			std::list<Clump>::iterator cit = MH.GetClumpIter(clump);
			
			if(cit!=MH.GetClumpEnd())
			(*cit).RemoveFood((*foodit).GetMe());
		}
		m_health += FoodValue;
		MH.EraseFood(item.vnum);
	}
    else
    {

	
	SoundManager *manager = SoundManager::GetInstance();
	manager->PlaySound(SoundManager::SND_EAT_BOT);
	//if(GlobalBugBotList.count(item.bugbot))
	std::list<BugBot>::iterator botit = MH.GetBugBotIter(item.vnum);
	


	if(botit==MH.GetBugBotEnd() || ! botit->IsCorpse())
	{
	    std::cout << "Eat, bad item vnum (or tried to eat live bot): " << item.vnum << endl;
	    return;
	}
//	std::cout << "Ate corpse: " << item.vnum << endl;
	Position pos = (*botit).GetPos();
	MH.SetCorpseAt(pos,MH.GetBugBotEnd());
	
	MH.EraseBugBot(item.vnum);
	
	m_health += CorpseValue;
	if(!m_flags[CANNIBAL])
	{
		m_flags += CANNIBAL;
		manager->PlaySound(SoundManager::SND_CANNIBAL);
	}


    } // was bot

    if(m_health > 1000)
    {
	m_health=1000;
    }

	item.vnum = -1;
}

void BugBot::Die()
{
	ticks_dead = 0;

	SoundManager *manager = SoundManager::GetInstance();
	manager->PlaySound(SoundManager::SND_DIE);
    if(m_team != -1 )
    {
	std::list<MainBrain>::iterator i= MH.GetMainBrainIter(m_team);
	
	if(i!=MH.GetMainBrainEnd())
	{
	    (*i).RemoveBugBot(m_me);
	    (*i).ReportCorpse(m_me);
	}
	
    }
    
    m_flags+=CORPSE;
    
    if(HasItem())
    {
		cout << GetMe() << " died with item in hand. " << endl;
		if(m_item.bot)
		{
			//we have to drop the corpse in a different square, or else we have two corpses in the same square.
			(MH.GetBugBotIter(m_item.vnum))->SetPos( MH.NoBot(m_pos), true);
			(MH.GetBugBotIter(m_item.vnum))->SetCarried(false);
		//	std::cout << "Dying and dropping a corpse." << endl;
		}
		else DropItem();
    }
    
	

//	if(m_health > 0)
//		std::cout << GetMe() << " died with health: " << m_health << endl;
    MH.SetBugBotAt(m_pos,MH.GetBugBotEnd());
    MH.SetCorpseAt(m_pos,MH.GetBugBotIter(m_me)); //it _should_ be doing this automatically later
    
}

 void BugBot::PickUp(Item item)
{
    m_item = item;
    if(!item.bot)
    {
	std::list<Food>::iterator foodit = MH.GetFoodIter(item.vnum);
	int clump=(*foodit).GetClump();
	if(clump != -1)
	{
	    (*foodit).CarryClump();
	    std::list<Clump>::iterator cit = MH.GetClumpIter(clump);
	    
	    if(cit != MH.GetClumpEnd())
		(*cit).RemoveFood((*foodit).GetMe());
	}
    }
    else
    {
	
	std::list<BugBot>::iterator botit = MH.GetBugBotIter(item.vnum);
	
	if(!botit->IsCorpse())
	{
		std::cout << "Picking up alive bot: " << item.vnum << endl;
	}

	if(botit != MH.GetBugBotEnd())
	{
	    (*botit).SetCarried(true);
	}
	else
	{
	    std::cout << "Pickup, bad item vnum: " << item.vnum << endl;
	}
    }
}
void BugBot::AvoidUpdate(Avoid Param)
{
    Param.Update();
    if(Param.IsOver())
    {
	std::vector<Avoid>::iterator i = std::find(m_avoids.begin(),m_avoids.end(),Param);
	m_avoids.erase(i);
    }
}


void BugBot::Update()
{
    if(!m_flags[CARRIED] && !m_flags[CORPSE])
    {
	//MH.SetBugBotAt(m_pos, 0); //WOOP
	////std::cout << "E BB::Upd" << endl;
	CheckRenegade();
	for(std::vector<Avoid>::iterator i=m_avoids.begin();i!=m_avoids.end();i++)
	{
	    AvoidUpdate(*i);
	}
	m_health--;
	CheckHunger();
	if(!m_flags[CORPSE])
	{
	CheckFollow();
	}
	////std::cout << "done with checkhunger and checkfollow" << endl;
	if(!m_flags[CORPSE])
	AreaScan();
	// //std::cout << "doing checkdest" << endl;
	if(!m_flags[CORPSE])
	CheckDest();
	////std::cout << "done with checkdest, doing movetodest" << endl;
	if(!m_flags[CORPSE])
	MoveToDest();
	////std::cout << "end mtd" << endl;
	////std::cout << "L BB Upd" << endl;
	//MH.SetBugBotAt(m_pos, MH.GetBugBotIter(m_me));



    }
	if(IsCorpse())ticks_dead++;
	if(!IsCorpse() && MH.GetBugBotAt(m_pos) == MH.GetBugBotEnd())
	{
		std::cout << "ERROR - I'm here but the map doesnt show me. " << GetMe() << endl;
	}

	if(IsCorpse() && MH.GetCorpseAt(m_pos) != MH.GetBugBotIter(GetMe()) &&  MH.GetCorpseAt(m_pos) != MH.GetBugBotEnd())
	{
		std::cout << "somebody just walked over me! " << GetMe() << endl;
	}

	if(IsCorpse() && MH.GetCorpseAt(m_pos) == MH.GetBugBotEnd())
	{
		std::cout << ticks_dead << " ticks ";
		if(starved)

		std::cout << "ERROR - STARVED CORPSE here but the map doesnt show me. " << GetMe() << endl;
		else std::cout << "ERROR - corpse here but the map doesnt show me." << GetMe() << endl;
	}

}

bool BugBot::IsCannibal()
{
    return m_flags.IsSet(CANNIBAL);
}

bool BugBot::IsHungry()
{
    return m_flags.IsSet(HUNGRY);
}

bool BugBot::IsStarving()
{ 
    return m_flags.IsSet(STARVING);
}

bool BugBot::HasItem()
{
    return (m_item.vnum != -1);
}

int BugBot::GetColor()
{
    return m_color;
}

Flag BugBot::GetFlags()
{
    return m_flags;
}

int BugBot::GetTeam()
{
    return m_team;
}

int BugBot::GetHealth()
{
    return m_health;
}

Position BugBot::GetPos()
{
    return m_pos;
}

bool BugBot::IsCorpse()
{
    return m_flags[CORPSE];
}

bool BugBot::IsRenegade()
{
    return m_flags[RENEGADE];
}

void BugBot::SetPos(const Position& pos, bool replace)
{
    
    if(m_pos == pos) replace=false;
    
    std::list<BugBot>::iterator meit = MH.GetBugBotIter(m_me);
    Position tpos = m_pos;
    m_pos = pos;
    if(!IsCorpse())
    {
	MH.SetBugBotAt(pos,meit);
    }
    else
    {
	MH.SetCorpseAt(pos,meit);
    }
    
    if(replace)
    {
	if(!IsCorpse())
	{
	  MH.SetBugBotAt(tpos,MH.GetBugBotEnd());
	}
	else
	{
	  MH.SetCorpseAt(tpos,MH.GetBugBotEnd());
	}
    }
}

void BugBot::SetMe(int i)
{
    m_me = i;
}

int BugBot::GetMe()
{
    return m_me;
}

void BugBot::CheckRenegade()
{
    std::list<MainBrain>::iterator mbit = MH.GetMainBrainIter(m_team);
    
    if(mbit==MH.GetMainBrainEnd()) return;
    if((*mbit).NumberOfBotsControlled() >= 20)
    {
	if(1.0 / (float)sqrt((float)(*mbit).NumberOfBotsControlled()-19) + 1>rand())
	{
	    m_flags+=RENEGADE;
		SoundManager *manager = SoundManager::GetInstance();
		manager->PlaySound(SoundManager::SND_RENEGADE);

	    //GlobalMainBugBotList[m_team].RemoveBot(m_vnum);
	}
    }
}

void BugBot::DropItem()
{
	if(!HasItem())
	{
		std::cout << "Tried to drop item you didnt have." << endl;
		return;
	}
    //if(m_item.bot) ////std::cout << "Bot # " : ////std::cout << "Food # ";
    ////std::cout << m_item.vnum << " at " << m_pos.x << ", " << m_pos.y;
    if(!m_item.bot)
    {
	std::list<Food>::iterator foodit = MH.GetFoodIter(m_item.vnum);
	if(foodit==MH.GetFoodEnd()) return;
	(*foodit).NoClump();
	(*foodit).NoCarryClump();
    }
    else
    {
		std::cout << "Dropping corpse." << m_item.vnum << endl;
	std::list<BugBot>::iterator botit = MH.GetBugBotIter(m_item.vnum);
	if( botit == MH.GetBugBotEnd())
	{
	    std::cout << "Drop Item, Bad item vnum: " << m_item.vnum << endl;
	    return;
	}
	(*botit).SetCarried(false);
    }
    m_item.vnum=-1;
    m_item.bot=false;
}

void BugBot::FoodFromGod(int FoodValue )
{
	SoundManager *manager = SoundManager::GetInstance();
	manager->PlaySound(SoundManager::SND_FOOD_FROM_GOD);
    m_health+=FoodValue;
    if(m_health > 1000)
    {
	m_health=1000;
    }
}

bool BugBot::IsCarried()
{
    return m_flags[CARRIED];
}

void BugBot::SetCarried(bool t)
{
    if(t)
    {
	m_flags+=CARRIED;
    }
    else
    {
	m_flags-=CARRIED;
    }
}

void BugBot::SetTeam(int ateam)
{
    m_team = ateam;
}

void BugBot::RemoveRefOfBot(int bot)
{
    if(m_targetbot == bot)
    {
	m_targetbot=-1;
    }
    if(m_item.vnum==bot)
    {
	m_item.vnum=-1;
    }
}

void BugBot::RemoveRefOfFood(int i)
{
    if(m_item.vnum==i)
    {
	m_item.vnum=-1;
    }
}




