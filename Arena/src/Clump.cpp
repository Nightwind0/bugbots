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

#include "Clump.h"
#include "Defs.h"
#include "Utilities.h"
#include "Food.h"

using namespace BugBots;

Clump::Clump():m_tics(0)
{
}

void Clump::Update()
{
  //  if(++m_tics % 10 == 0)
	spawn(new Food(),GetPos());
    
}

BugBots::Color Clump::GetColor() const
{
    return Utilities::CreateColor(0.0,0.5,0.5);
}

