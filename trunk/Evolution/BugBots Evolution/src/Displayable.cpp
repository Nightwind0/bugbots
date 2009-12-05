#include "Displayable.h"
#include "SDL/SDL_image.h"

using BugBots::Displayable;


Displayable::Displayable(){
}

Displayable::~Displayable(){
}

SDL_Surface *Displayable::load_image( const std::string& filename )
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

GLuint Displayable::bind_texture ( SDL_Surface* surface )
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
