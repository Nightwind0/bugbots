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

namespace BugBots {

class BugBot : public BugBots::GameObject
{
public:
    BugBot(MainBrain& mainbrain);
    virtual ~BugBot();
    virtual Color GetColor() const;
    virtual void Update(shared_ptr<GameObject> _this);
    void StartMovingTo(QTVector);
protected:

	void MoveStep(shared_ptr<GameObject> _this);
	bool AtDest() const;
private:
    enum eState {
	SEARCHING,
	TARGETING_ITEM,
	GOING_HOME
    };
    shared_ptr<GameObject> m_pItem;
    QTVector m_goal;
    MainBrain& m_mainbrain;
    eState m_state;
	int m_dx;
	int m_dy;
	int m_sx;
	int m_sy;
	int m_err;
};

}
#endif // BUGBOT_H
