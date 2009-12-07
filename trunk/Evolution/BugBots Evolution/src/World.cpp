#include "World.h"
#include <cassert>

using BugBots::World;
using BugBots::WorldObject;

World::World():m_quadtree(NULL,
                                  Quadtree::Geometry::Square<double>(
                                      Quadtree::Geometry::Vector<double>( 0.0f, 0.0f ),
                                      WORLD_SIZE)
                                 )
{
}

World::~World()
{
}


void World::AddObject ( WorldObject* object)
{
    assert ( object );

    Quadtree::Geometry::Circle<double> circle(
        object->GetOrigin(),
        object->GetRadius() );

    m_quadtree.Add ( circle, object );

}


void World::RemoveObject ( WorldObject* object )
{
    assert ( object );

    Quadtree::Geometry::Circle<double> circle(
        object->GetOrigin(),
        object->GetRadius() );

    m_quadtree.Remove ( circle, object );
}

void World::HandleObjectMovement ( WorldObject* object)
{
    assert ( object );

    RemoveObject ( object );
    AddObject ( object );
}

void World::TraverseAllObjects ( Quadtree::Visitor<WorldObject*>& visitor ) const
{
    m_quadtree.TraverseAll ( visitor );
}

void World::TraverseObjects ( Quadtree::Visitor<WorldObject*>& visitor, const Quadtree::Geometry::Circle<double>& circle ) const
{
    m_quadtree.Traverse ( visitor, circle );
}
