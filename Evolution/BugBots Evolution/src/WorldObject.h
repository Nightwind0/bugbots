#ifndef WORLDOBJECT_H_INCLUDED
#define WORLDOBJECT_H_INCLUDED

#include <string>
#include "Position.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_image.h"

namespace BugBots{
    class WorldObject{
        public:
        WorldObject(){}
        virtual ~WorldObject(){}

        virtual void Draw(float x, float y, float scale=1.0f, float rotation = 0.0f)=0;
        virtual Position GetOrigin() const=0;
        virtual double GetRadius() const=0;
        // Used by the World to notify of collisions between other objects
        virtual void HandleCollision ( WorldObject &other )=0;


        protected:
        static SDL_Surface *load_image( const std::string& filename );
        static GLuint bind_texture ( SDL_Surface* surface );

    };
}

#endif // WORLDOBJECT_H_INCLUDED
