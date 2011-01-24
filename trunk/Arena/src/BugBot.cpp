/*
 *    <one line to give the program's name and a brief idea of what it does.>
 *    Copyright (C) <year>  <name of author>
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "BugBot.h"
#include "Utilities.h"
#include "App.h"

using namespace BugBots;


BugBot::BugBot(MainBrain& brain):m_mainbrain(brain),m_pItem(NULL),m_state(SEARCHING),m_goal(0,0)
{
}

BugBot::~BugBot()
{
}



BugBots::Color BugBot::GetColor() const
{
    if(m_pItem) return m_pItem->GetColor();
    return Utilities::DefaultTeamColor(m_mainbrain.GetTeam());
}

void BugBot::Update()
{
    if(m_pItem)
		m_pItem->SetPos(GetPos());
    
    std::list<GameObject*> blips;
    scan(QTCircle(GetPos(),10),blips);
    
    for(std::list<GameObject*>::const_iterator iter = blips.begin();
	iter != blips.end(); iter++)
	{
	    if(m_state == TARGETING_ITEM)
	    {
		Food * pFood = dynamic_cast<Food*>(*iter);
		if(pFood)
		{
		    if(pFood->GetPos() == GetPos())
		    {
			pFood->Detach();
			LOG("Grabbing food");
			m_pItem = pFood;
			m_state = GOING_HOME;
			StartMovingTo(m_mainbrain.GetPos());
			break;
		    }
		}
	    }
	    else if(m_state == SEARCHING){
		Food * pFood = dynamic_cast<Food*>(*iter);
		
		if(pFood)
		{
		    StartMovingTo(pFood->GetPos());
		    m_state = TARGETING_ITEM;
		    LOG("Found food");
		    break;
		}
	    }
	    else if(m_state == GOING_HOME)
	    {
		MainBrain * pMainBrain = dynamic_cast<MainBrain*>(*iter);
		// Don't assume theres only one mainbrain on your team
		if(pMainBrain && (pMainBrain->GetTeam() == m_mainbrain.GetTeam()))
		{
		    // I'm at my teams mainbrain and I have an item. Drop it.
		    m_pItem->Attach();
		    LOG("Dropping item");
		    m_pItem = NULL;
		    m_state = SEARCHING;
		    StartMovingTo(Utilities::RandomPosition());
		    break;
		}
	    }
	}
	
	if (AtDest()) {
	    switch(m_state)
	    {
		case SEARCHING:
		    StartMovingTo(Utilities::RandomPosition());
		    break;
		case GOING_HOME:
		case TARGETING_ITEM:
		    // if we get here, we didn't find anything
		    LOG("Giving up");
		    m_state = SEARCHING;
		    StartMovingTo(Utilities::RandomPosition());
		    break;
		break;
	    }
	}
	else {
	    MoveStep();		
	}
}

void BugBot::StartMovingTo(QTVector pos)
{
	m_goal = pos;
	QTVector loc = GetPos();
	m_dx = abs(loc.GetX() - pos.GetX());
	m_dy = abs(loc.GetY() - pos.GetY());
	m_sx = GetPos().GetX() < pos.GetX() ? 1 : -1;
	m_sy = GetPos().GetY() < pos.GetY() ? 1 : -1;
	m_err = m_dx - m_dy;
}

void BugBot::MoveStep()
{
	int x = GetPos().GetX();
	int y = GetPos().GetY();
	int e2 = m_err << 1;
	if (e2 > -m_dy) {
		m_err -= m_dy;
		x += m_sx;
	}
	else if (e2 < m_dx) {
		m_err += m_dx;
		y += m_sy;
	}
	MoveTo(QTVector(x,y));
}

bool BugBot::AtDest() const
{
	return m_goal == GetPos();
}


/*
 function line(x0, y0, x1, y1)
 dx := abs(x1-x0)
 dy := abs(y1-y0) 
 if x0 < x1 then sx := 1 else sx := -1
 if y0 < y1 then sy := 1 else sy := -1
 err := dx-dy
 
 loop
 setPixel(x0,y0)
 if x0 = x1 and y0 = y1 exit loop
 e2 := 2*err
 if e2 > -dy then 
 err := err - dy
 x0 := x0 + sx
 end if
 if e2 <  dx then 
 err := err + dx
 y0 := y0 + sy 
 end if
 end loop
 */

