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


BugBot::BugBot(MainBrain& brain):m_mainbrain(brain),m_goal(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT),m_pItem(NULL),m_state(SEARCHING)
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
			m_goal = m_mainbrain.GetPos();
			break;
		    }
		}
	    }
	    else if(m_state == SEARCHING){
		Food * pFood = dynamic_cast<Food*>(*iter);
		
		if(pFood)
		{
		    m_goal = pFood->GetPos();
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
		    m_goal = QTVector(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
		    break;
		}
	    }
	}
	
	QTVector pos = GetPos();
	int xDif = abs(pos.GetX() - m_goal.GetX());
	int yDif = abs(pos.GetY() - m_goal.GetY());
	
	if (xDif == 0 && yDif ==0) {
	    switch(m_state)
	    {
		case SEARCHING:
		    m_goal = QTVector(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
		    break;
		case GOING_HOME:
		case TARGETING_ITEM:
		    // if we get here, we didn't find anything
		    LOG("Giving up");
		    m_state = SEARCHING;
		    m_goal =  QTVector(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
		    break;

		break;
	    }
	}
	else {
	    if (xDif > yDif) {
		if (pos.GetX() > m_goal.GetX()) {
		    MoveTo(QTVector(pos.GetX()-1,pos.GetY()));
		}
		else {
		    MoveTo(QTVector(pos.GetX()+1,pos.GetY()));
		}
	    }
	    else {
		if (pos.GetY() > m_goal.GetY()) {
		    MoveTo(QTVector(pos.GetX(),pos.GetY()-1));
		}
		else {
		    MoveTo(QTVector(pos.GetX(),pos.GetY()+1));
		}
	    }		
	}
	
	
}


