#include "World.h"

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
