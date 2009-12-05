#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "Constants.h"
#include "Quadtree/Quadtree.hxx"
#include "WorldObject.h"

namespace BugBots{
    class World{
        public:
        World();
        ~World();


        void AddObject ( WorldObject* );
        void RemoveObject ( WorldObject* );
        void HandleObjectMovement ( WorldObject * );

        private:



        Quadtree::Node<WorldObject*> m_quadtree;


    };
}

#endif // WORLD_H_INCLUDED
