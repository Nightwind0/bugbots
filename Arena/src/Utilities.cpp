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

#include "Defs.h"
#include "Utilities.h"
#include <SDL/SDL.h>

using BugBots::Utilities;

BugBots::Color Utilities::CreateColor(float r, float g, float b, float a)
{
    //return BugBots::Color{(char)r*255,(char)g*255,(char)b*255,(char)a*255};
    BugBots::Color color;
    color.r = (unsigned char)(r*255.0);
    color.g = (unsigned char)(g*255.0);
    color.b = (unsigned char)(b*255.0);
    color.a = (unsigned char)(a*255.0);
    
    return color;
}

BugBots::Color Utilities::DefaultTeamColor (const Team& team)
{
    // Stupid for now. Later this will be data driven
    switch(team){
	case TEAM_BLUE:
	    return CreateColor(0,0,1);
	case TEAM_RED:
	    return CreateColor(1,0,0);
	default:
	    assert(0);
	    return CreateColor(0,0,0);
    }
}

BugBots::QTVector Utilities::RandomPosition()
{
    int rand_x = rand() % (SCREEN_WIDTH/2);
    int rand_y = rand() % (SCREEN_HEIGHT/2);
    if(rand() % 2) rand_x = -rand_x;
    if(rand() % 2) rand_y = -rand_y;
    
    return BugBots::QTVector(rand_x,rand_y);
}

int Utilities::GetConfig(eConfigValue config)
{
    switch(config)
    {
	case BUGBOT_COST:
	    return 50;
	case INITIAL_RESOURCES:
	    return 300;
	case FOOD_VALUE:
	    return 35;
	case CORPSE_VALUE:
	    return 50;
	default:
	    assert(0);
	    return 0;
    }
}

