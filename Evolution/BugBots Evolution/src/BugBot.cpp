#include "BugBot.h"
#include "SDL/SDL_opengl.h"

using BugBots::BugBot;


std::map<BugBot::eSprite,GLuint> BugBot::m_textures;

void BugBot::LoadResources(){
    SDL_Surface* blue = load_image("res/icon/bb_blue.png");
    SDL_Surface* red = load_image("res/icon/bb_red.png");


// CRAAAAAAAAAP: Have to do this only once, duh
    m_textures[ BUGBOT_BLUE ] = bind_texture ( blue );
    m_textures[ BUGBOT_RED ] = bind_texture ( red );
}

BugBot::BugBot(){
}

BugBot::~BugBot(){
}

void BugBot::HandleCollision ( WorldObject &other ){
}

void BugBot::Draw(float x, float y, float scale, float rotation)
{

    glPushMatrix();
    if(m_alignment == BLUE_TEAM){
        glBindTexture( GL_TEXTURE_2D, m_textures [ BUGBOT_BLUE ] );
    }else if(m_alignment == RED_TEAM){
        glBindTexture( GL_TEXTURE_2D, m_textures [ BUGBOT_RED ] );
    }
    glTranslatef(x,y,0);
    glRotatef ( rotation + 180, 0.0f, 0.0f, 1.0f );
    //glTranslatef(-16,-16,0);

    GLfloat width, height;
    width = 16.0f * scale;
    height = 16.0f * scale;



    glBegin( GL_QUADS );
    //Top-left vertex (corner)
    glTexCoord2i( 0, 0 );
    glVertex3f( -width * 0.5f, height*0.5f, 0.0f );

    //Bottom-left vertex (corner)
    glTexCoord2i( 0, 1 );
    glVertex3f( - width * 0.5f, - height * 0.5f, 0.0f );

    //Bottom-right vertex (corner)
    glTexCoord2i( 1, 1 );
    glVertex3f( width * 0.5f, - height * 0.5f, 0.0f );

    //Top-right vertex (corner)
    glTexCoord2i( 1, 0 );
    glVertex3f( width * 0.5f, height * 0.5f, 0.0f );
    glEnd();

    glPopMatrix();

}
