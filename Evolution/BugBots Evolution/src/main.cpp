#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "SDL/SDL_opengl.h"

#include <string>
#include "SDL/SDL_image.h"
#include "Quadtree/Quadtree.hxx"
#include "BugBot.h"
#include "Constants.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_DEPTH = 32;

/* Rotation about a point
if (rotation)
{
	glTranslatef(point.x, point.y, 0);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	glTranslatef(-point.x, -point.y, 0);
}
*/


bool initGL()
{
    glClearColor( 0x0, 0x0, 0x0, 0 );

    glEnable( GL_TEXTURE_2D );

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Set projection
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //If there was any errors
    if ( glGetError() != GL_NO_ERROR )
    {
        return false;
    }
    //If everything initialized
    return true;
}


int main ( int argc, char** argv )
{
    Quadtree::Geometry::Vector<double> center(0.0f,0.0f);
    Quadtree::Geometry::Square<double> world(center,1000.0f);
    Quadtree::Node<int> tree(NULL,world);
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // *new*

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH,
                                           SDL_OPENGL);
    if ( !screen )
    {
        printf("Unable to set 800x600x32 video: %s\n", SDL_GetError());
        return 1;
    }

    //Initialize OpenGL
    if ( initGL() == false )
    {
        return false;
    }

    BugBots::BugBot::LoadResources();

    BugBots::BugBot blue,red;
    blue.SetAlignment ( BugBots::BLUE_TEAM );
    red.SetAlignment ( BugBots::RED_TEAM );


#if USE_SDL_DRAW
    // centre the bitmap on screen
    SDL_Rect dstrect;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;
    dstrect.w <<= 2;
#endif
    // program main loop
    bool done = false;
    int rotation = 0;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
            {
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                break;
            }
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE
        blue.Draw(50,50);
        red.Draw(200,80);


#if USE_SDL_DRAW
        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
        // draw bitmap
        SDL_BlitSurface(bmp, 0, screen, &dstrect);
#else


#endif
        // DRAWING ENDS HERE
#if !(USE_SDL_DRAW)
        // finally, update the screen :)
        SDL_GL_SwapBuffers();
#else
        SDL_Flip(screen);
#endif
        glClear( GL_COLOR_BUFFER_BIT );
    } // end main looop



    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
