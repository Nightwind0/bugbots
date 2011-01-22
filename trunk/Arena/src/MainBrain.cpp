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

void MainBrain::Update()
{
    if(m_resources >= Utilities::GetConfig(Utilities::BUGBOT_COST)){
	spawn_bugbot();
    }
}

void MainBrain::spawn_bugbot()
{
    m_resources -= Utilities::GetConfig(Utilities::BUGBOT_COST);
    QTCircle spawnCircle(GetPos(),20);
    //std::list<GameObject*> objects;
    //scan(spawnCircle,objects);
    // Fuck it i'm putting the bugbot in my location
    // until I think of a good way to look for a spot.
    // (loop over radius++)
    BugBot * bb = new BugBot(m_team);
    spawn(bb,GetPos());
}


