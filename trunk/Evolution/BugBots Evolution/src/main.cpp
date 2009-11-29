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

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_DEPTH = 32;

SDL_Surface *load_image( const std::string& filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    //Load the image using SDL_Image
    loadedImage = IMG_Load( filename.c_str() );
    //If the image loaded
    if ( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        //Free the old image
        SDL_FreeSurface( loadedImage );
    }
    //Return the optimized image
    return optimizedImage;
}

GLuint bind_texture ( SDL_Surface* surface )
{
    GLenum texture_format;
    GLuint texture;
    // Check that the image's width is a power of 2
    if ( (surface->w & (surface->w - 1)) != 0 )
    {
        printf("warning: image.bmp's width is not a power of 2\n");
    }

    // Also check if the height is a power of 2
    if ( (surface->h & (surface->h - 1)) != 0 )
    {
        printf("warning: image.bmp's height is not a power of 2\n");
    }

    // get the number of channels in the SDL surface
    GLint nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4)     // contains an alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    }
    else if (nOfColors == 3)     // no alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }
    else
    {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
    }

    // Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &texture );

    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, texture );

    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                  texture_format, GL_UNSIGNED_BYTE, surface->pixels );


    // Free the SDL_Surface only if it was successfully created
    if ( surface )
    {
        SDL_FreeSurface( surface );
    }

    return texture;
}


bool initGL()
{
    glClearColor( 0, 0, 0, 0 );

    glEnable( GL_TEXTURE_2D );

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

    // load an image
    SDL_Surface* bmp = load_image("res/icon/bb_blue.png");
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    GLuint texture = bind_texture(bmp);

#if USE_SDL_DRAW
    // centre the bitmap on screen
    SDL_Rect dstrect;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;
    dstrect.w <<= 2;
#endif
    // program main loop
    bool done = false;
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

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

#if USE_SDL_DRAW
        // draw bitmap
        SDL_BlitSurface(bmp, 0, screen, &dstrect);
#else
        glBindTexture( GL_TEXTURE_2D, texture );

        glBegin( GL_QUADS );
        //Top-left vertex (corner)
        glTexCoord2i( 0, 0 );
        glVertex3f( 100, 100, 0.0f );

        //Bottom-left vertex (corner)
        glTexCoord2i( 1, 0 );
        glVertex3f( 116, 100, 0 );

        //Bottom-right vertex (corner)
        glTexCoord2i( 1, 1 );
        glVertex3f( 116, 116, 0 );

        //Top-right vertex (corner)
        glTexCoord2i( 0, 1 );
        glVertex3f( 100, 116, 0 );
        glEnd();
#endif
        // DRAWING ENDS HERE
#if !(USE_SDL_DRAW)
        // finally, update the screen :)
        SDL_GL_SwapBuffers();
#else
        SDL_Flip(screen);
#endif
    } // end main looop


    glDeleteTextures( 1, &texture );

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
