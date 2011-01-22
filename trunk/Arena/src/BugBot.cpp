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

#include "BugBot.h"
#include "Utilities.h"

using namespace BugBots;


BugBot::BugBot(const Team& team):m_team(team),m_goal(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT)
{
}

BugBot::~BugBot()
{
}



BugBots::Color BugBot::GetColor() const
{
    return Utilities::DefaultTeamColor(m_team);
}

void BugBot::Update()
{
    QTVector pos = GetPos();
    
    // Dead Stupid AI
    if(pos.GetX() > m_goal.GetX()){
	MoveTo(QTVector(pos.GetX()-1,pos.GetY()));
    }else if(pos.GetX() < m_goal.GetX()){
	MoveTo(QTVector(pos.GetX()+1,pos.GetY()));
    }else if(pos.GetY() < m_goal.GetY()){
	MoveTo(QTVector(pos.GetX(),pos.GetY()+1));
    }else if(pos.GetY() > m_goal.GetY()){
	MoveTo(QTVector(pos.GetX(),pos.GetY()-1));
    }
    
}

