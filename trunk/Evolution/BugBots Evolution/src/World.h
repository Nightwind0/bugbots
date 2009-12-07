#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "Constants.h"
#include "Quadtree/Quadtree.hxx"
#include "Quadtree/Visitor.hxx"
#include "WorldObject.h"

namespace BugBots{
    class World{
        public:
        World();
        ~World();


        void AddObject ( WorldObject* );
        void RemoveObject ( WorldObject* );
        void HandleObjectMovement ( WorldObject * );

        void TraverseAllObjects ( Quadtree::Visitor<WorldObject*>& visitor ) const;
        void TraverseObjects ( Quadtree::Visitor<WorldObject*>& visitor, const Quadtree::Geometry::Circle<double> &circle )const;

        private:



        Quadtree::Node<WorldObject*> m_quadtree;


    };
}

#endif // WORLD_H_INCLUDED
