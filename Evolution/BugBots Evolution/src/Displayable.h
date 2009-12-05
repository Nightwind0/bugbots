#ifndef DISPLAYABLE_H_INCLUDED
#define DISPLAYABLE_H_INCLUDED

#include <string>
#include "SDL/SDL_opengl.h"

class SDL_Surface;

namespace BugBots
{

    class Displayable
    {
    public:
        Displayable();
        virtual ~Displayable();

        virtual void Draw(float x, float y, float scale=1.0f, float rotation = 0.0f)=0;
    private:
        GLuint texture;
    protected:
        static SDL_Surface *load_image( const std::string& filename );
        static GLuint bind_texture ( SDL_Surface* surface );


    };

}


#endif // DISPLAYABLE_H_INCLUDED
