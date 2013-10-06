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

#include "Food.h"
#include "Utilities.h"

using BugBots::Food;

Food::Food()
{
}

Food::~Food()
{
}

BugBots::Color Food::GetColor() const
{
    return Utilities::CreateColor(0.0,0.75,0.0);
}

void Food::Update(shared_ptr<GameObject> _this)
{

    std::list<shared_ptr<GameObject>> blips;
    scan(QTCircle(GetPos(),1),blips);

    for(std::list<shared_ptr<GameObject>>::const_iterator iter = blips.begin();
	iter != blips.end(); iter++)
       {
	 shared_ptr<Food> food = std::tr1::dynamic_pointer_cast<Food>(*iter);
	 if(food && food->GetPos() == GetPos() && food != _this){
	   int x = GetPos().GetX();
	   int y = GetPos().GetY();
	   int dir = rand() % 4;
	   switch(dir){
	   case 0:
	     x++;
	     break;
	   case 1:
	     x--;
	     break;
	   case 2:
	     y++;
	     break;
	   case 3:
	     y--;
	     break;
	   }
	   MoveTo(_this,QTVector(x,y));
	 }
       }
    
}


