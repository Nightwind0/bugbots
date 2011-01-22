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
        typedef Quadtree::Node<WorldObject*> NodeType;


        void AddObject ( WorldObject* );
        void RemoveObject ( WorldObject* );
        void HandleObjectMovement ( WorldObject * );

        void TraverseAllObjects ( Quadtree::Visitor<WorldObject*, NodeType>& visitor ) const;
        void TraverseObjects ( Quadtree::Visitor<WorldObject*,NodeType>& visitor, const Quadtree::Geometry::Circle<double> &circle )const;

        private:

        Quadtree::RootNode<WorldObject*> m_quadtree;


    };
}

#endif // WORLD_H_INCLUDED
