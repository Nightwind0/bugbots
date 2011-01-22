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
    color.r = (char)r*255;
    color.g = (char)g*255;
    color.b = (char)b*255;
    color.a = (char)a*255;
    
    return color;
}