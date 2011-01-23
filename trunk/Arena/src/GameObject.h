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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Defs.h"

namespace BugBots{

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();
    
    void Attach(); // put in the quadtree
    void Detach(); // Remove from the quadtree
    void MoveTo(const QTVector& );

    QTVector GetPos() const;
    virtual Color GetColor() const=0;
    virtual void Update()=0;
    
    // To be called by the Game only.
    // Everyone else call MoveTo
    void SetPos(const QTVector& );
protected:
    void scan(const QTCircle& area, std::list<GameObject*>& bucket);
    void spawn(GameObject * pObject, const QTVector& pos);
private:
    QTVector m_pos;    
};


};
#endif // GAMEOBJECT_H
