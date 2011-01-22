#include "utility.h"
#include "clump.h"
#include "food.h"
#include <algorithm>
#include <map>
#include "sutil.h"
#include "MapHandler.h"


extern MapHandler MH;

int Clump::GetAFood()
{
    if(m_food.size())
    {
	return *(m_food.begin());
    }
    else
    {
	return -1;
    }
}

bool Clump::OwnerOf(int food)
{    
    using std::vector;
    bool found=false;
    for(vector<int>::iterator i = m_food.begin() ; i<m_food.end() ; ++i)
    {
	if(*i == food)
	{
	    found = true;
	}
    }
    return found;
}

bool Clump::HasFood()
{
    return m_food.size() != 0;
}

bool Clump::RemoveFood(int food)
{
    using std::find;
    using std::vector;
    vector<int>::iterator i = find(m_food.begin(),m_food.end(),food);
    if(i != m_food.end())
    {
	m_food.erase(i);
    }
    return true;
}

void Clump::SpawnAFood()
{
	if(m_food.size() < 75)
	{
		Position pos = MH.NoFood(m_pos);
		m_food.push_back((*MH.NewFood(m_me,pos)).GetMe());
	}
}

void Clump::Update()
{
    ++m_progress;
    if(m_progress == m_SpawnRate)
    {
	SpawnAFood();
	m_progress = 0;
    }
}

void Clump::SetMe(int i)
{
    m_me = i;
}

int Clump::GetMe()
{
    return m_me;
}

Clump::Clump()
{
    m_progress = 0;
    m_SpawnRate = 50;
    m_me = -1;
}

void Clump::SetSpawnRate(int sr)
{
    m_SpawnRate = sr;
}

void Clump::SetPos(const Position& apos)
{
    m_pos = apos;
}

Position Clump::GetPosition()
{
    return m_pos;
}
int Clump::GetFoodCount()
{
    return m_food.size();
}

