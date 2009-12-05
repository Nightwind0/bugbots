#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include "Quadtree/Quadtree.hxx"

namespace BugBots
{
    // Represents a position in the game world
    struct Position{
        double m_x;
        double m_y;
        operator Quadtree::Geometry::Vector<double> () const{
            return Quadtree::Geometry::Vector<double>( m_x, m_y );
        }
    };
}

#endif // POSITION_H_INCLUDED
