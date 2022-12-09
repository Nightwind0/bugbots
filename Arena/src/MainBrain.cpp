/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "MainBrain.h"
#include "Utilities.h"
#include "BugBot.h"
#include "Game.h"

using namespace BugBots;

MainBrain::MainBrain(const Team& team):m_team(team),m_resources(Utilities::GetConfig(Utilities::INITIAL_RESOURCES))
{
    m_color = Utilities::CreateColor(1,1,1);
}

BugBots::Color MainBrain::GetColor() const
{
    return m_color;
}

Team MainBrain::GetTeam() const
{
    return m_team;
}

void MainBrain::Update(shared_ptr<GameObject> _this)
{    
	QTCircle scanCircle(GetPos(),10);
    std::list<shared_ptr<GameObject> > blips;
    scan(scanCircle,blips);
    
    for(std::list<shared_ptr<GameObject> >::iterator iter = blips.begin();
		iter != blips.end(); iter++)
	{
        Take(*iter);
	    
        //Food * pFood = dynamic_cast<Food*>(iter->get());
//	    shared_ptr<Food> pFood = std::tr1::dynamic_pointer_cast<Food>(*iter);
//	    shared_ptr<BugBot> pBB = std::tr1::dynamic_pointer_cast<BugBot>(*iter);
//	    
//	    if(pFood)
//	    {
//		detach(*iter);
//		m_resources += Utilities::GetConfig(Utilities::FOOD_VALUE);
//	    }
//	    else if(pBB)
//	    {
//		if(pBB->HasFlag(BugBot::DEAD)){
//		    detach(*iter);
//		    m_resources += Utilities::GetConfig(Utilities::CORPSE_VALUE);
//		}
//	    }
	}
    if(m_resources >= Utilities::GetConfig(Utilities::BUGBOT_COST)){
		spawn_bugbot();
    }
}

bool MainBrain::Take(shared_ptr<GameObject> p_Item)
{
    //Food * pFood = dynamic_cast<Food*>(iter->get());
    shared_ptr<Food> pFood = std::dynamic_pointer_cast<Food>(p_Item);
    shared_ptr<BugBot> pBB = std::dynamic_pointer_cast<BugBot>(p_Item);
    
    if(pFood)
    {
		detach(p_Item);
		m_resources += Utilities::GetConfig(Utilities::FOOD_VALUE);
        return true;
    }
    else if(pBB)
    {
		if(pBB->HasFlag(BugBot::DEAD)){
		    detach(p_Item);
		    m_resources += Utilities::GetConfig(Utilities::CORPSE_VALUE);
            return true;
		}
    }
    return false;
}

void MainBrain::spawn_bugbot()
{
    m_resources -= Utilities::GetConfig(Utilities::BUGBOT_COST);

    //std::list<GameObject*> objects;
    //scan(spawnCircle,objects);
    // Fuck it i'm putting the bugbot in my location
    // until I think of a good way to look for a spot.
    // (loop over radius++)
    shared_ptr<GameObject>  bb = shared_ptr<GameObject>(new BugBot(*this));
    spawn(bb,GetPos());
    shared_ptr<BugBot> pBB = std::dynamic_pointer_cast<BugBot>(bb);
    pBB->StartMovingTo(Utilities::RandomPosition());
}


