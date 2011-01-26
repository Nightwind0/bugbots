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

#ifndef BUGBOT_H
#define BUGBOT_H

#include "GameObject.h"
#include "MainBrain.h"
#include "Food.h"
#include <ostream>

namespace BugBots {
    


class BugBot : public BugBots::GameObject
{
public:
    enum eFlags {
	DEAD = 1,
	CANNIBAL,
	ROGUE
    };
    BugBot(MainBrain& mainbrain);
    virtual ~BugBot();
    virtual Color GetColor() const;
    virtual void Update(shared_ptr<GameObject> _this);
    Team GetTeam() const { return m_mainbrain.GetTeam(); };
    void StartMovingTo(QTVector);
    bool HasFlag(eFlags flag) const;
    void SetFlag(eFlags flag);
    void RemoveFlag(eFlags flag);

protected:
    void MoveStep(shared_ptr<GameObject> _this);
    bool AtDest() const;
private:
    enum eState {
	SEARCHING,
	TARGETING_ITEM,
	GOING_HOME
    };
    void die();
    shared_ptr<GameObject> m_pItem;
    QTVector m_goal;
    MainBrain& m_mainbrain;
    eState m_state;
    int m_flags;
    int m_dx;
    int m_dy;
    int m_sx;
    int m_sy;
    int m_err;
    int m_age;
    
#if !defined(NDEBUG)
    friend std::ostream& operator<<(std::ostream& stream,const BugBot& bb);
#endif
};

#if !defined(NDEBUG)
std::ostream& operator<<(std::ostream&, const BugBots::BugBot&);
#endif

}



#endif // BUGBOT_H
