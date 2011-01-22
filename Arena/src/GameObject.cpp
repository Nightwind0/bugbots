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

#include "GameObject.h"
#include "Game.h"

using BugBots::GameObject;

GameObject::GameObject():m_pos(0,0)
{
}

GameObject::~GameObject()
{
}

void GameObject::Destroy()
{
    Game* game = dynamic_cast<Game*>(GetApp());
    game->remove_game_object(this);
}

void GameObject::MoveTo(const QTVector& pos)
{
    Game* game = dynamic_cast<Game*>(GetApp());
    game->move_game_object(this,pos);
}

void GameObject::scan(const QTCircle& area, std::list<GameObject*>& bucket)
{
    Game* game = dynamic_cast<Game*>(GetApp());
    game->scan_area(area,bucket);
}

void GameObject::spawn(GameObject * pObject, const QTVector& pos)
{
    Game* game = dynamic_cast<Game*>(GetApp());
    pObject->SetPos(pos);
    game->add_game_object(pObject);
}

void GameObject::SetPos(const QTVector& pos)
{
    m_pos = pos;
}

BugBots::QTVector GameObject::GetPos() const
{
    return m_pos;
}