#ifndef WORLDOBJECT_H_INCLUDED
#define WORLDOBJECT_H_INCLUDED

#include "Position.h"

namespace BugBots{
    class WorldObject{
        public:
        WorldObject(){}
        virtual ~WorldObject(){}

        virtual Position GetPosition() const=0;
        // Used by the World to notify of collisions between other objects
        virtual void HandleCollision ( WorldObject &other )=0;


        private:
    };
}

#endif // WORLDOBJECT_H_INCLUDED
