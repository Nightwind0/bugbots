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

#ifndef MAINBRAIN_H
#define MAINBRAIN_H

#include "GameObject.h"
#include "../../common/steel/src/SteelInterpreter.h"

namespace BugBots{


  class TeamObject {
  public:
    virtual Team GetTeam() const=0;
  };

  class MainBrain : public BugBots::GameObject, public TeamObject
{
public:
    MainBrain(const Team& team);
    virtual Color GetColor() const;
    virtual void Update(shared_ptr<GameObject> _this);
    virtual Team GetTeam() const;
    bool Take(shared_ptr<GameObject> m_pItem);
private:
    Steel::SteelInterpreter* m_pInterpreter;
    void spawn_bugbot();
    Color  m_color;
    Team m_team;   
    int m_resources;
};

};

#endif // MAINBRAIN_H
