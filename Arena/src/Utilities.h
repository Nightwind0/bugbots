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

#ifndef UTILITIES_H
#define UTILITIES_H
#include "Defs.h"
    

namespace BugBots { 
    

class Utilities
{
public:
    enum eConfigValue {
	BUGBOT_COST,
	INITIAL_RESOURCES
    };
    static Color CreateColor(float r, float g, float b, float a=0.0);
    static Color DefaultTeamColor (const Team& team);
    static int GetConfig(eConfigValue);
};

};

#endif // UTILITIES_H
