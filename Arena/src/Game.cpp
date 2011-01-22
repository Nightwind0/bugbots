#include "Game.h"
#include "Defs.h"
#include "GameObject.h"
#include "MainBrain.h"
#include <sys/time.h>

using namespace BugBots;


class Updater : public Quadtree::Visitor<BugBots::GameObject*,BugBots::QTNode>
{
public:
    virtual bool Visit(BugBots::GameObject* object,const BugBots::QTNode* node){
	object->Update();
	return true;
    }
};

template <class T>
class Drawer : public Quadtree::Visitor<BugBots::GameObject*,BugBots::QTNode>
{
public:
    typedef void (T::*DrawFunctor)(BugBots::Color, int, int);
    Drawer(T* pApp,DrawFunctor functor):m_pApp(pApp),m_functor(functor){
	
    }
    
    virtual bool Visit(BugBots::GameObject* object,const BugBots::QTNode* node){
	BugBots::Color color = object->GetColor();
	BugBots::QTVector pos = object->GetPos();
	(m_pApp->*m_functor)(color, pos.GetX(),pos.GetY());
	return true;
    }
private:
   T* m_pApp;
   DrawFunctor m_functor;
};

Game::Game():m_quadtree(BugBots::QTNode::Square(BugBots::QTVector(0,0),800)){
}

Game::~Game(){
}

void Game::add_game_object(GameObject* pObject)
{
    QTCircle circle(pObject->GetPos(),0);
    m_quadtree.Add(circle,pObject);
}

bool Game::OnInit(){
	m_screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF  ); 
	srand(time(NULL));
	MainBrain * brain = new MainBrain();
	brain->SetPos(QTVector(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT));
	add_game_object(brain);
	return true;
}

void Game::OnLoop(){
    Updater updater;
    m_quadtree.TraverseAll(updater);
}

void Game::OnRender(){
	// Lock surface if needed
  if (SDL_MUSTLOCK(m_screen)) 
    if (SDL_LockSurface(m_screen) < 0) 
      return;

  // Ask SDL for the time in milliseconds
  int tick = SDL_GetTicks();

  // Draw to screen
  Drawer<Game> drawer(this,&Game::DrawPixel);
  m_quadtree.TraverseAll(drawer);

  // Unlock if needed
  if (SDL_MUSTLOCK(m_screen)) 
    SDL_UnlockSurface(m_screen);

  // Tell SDL to update the whole screen
  SDL_UpdateRect(m_screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
  SDL_Flip(m_screen);
}

void Game::OnMouseMove(int x, int y){
}

void Game::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
}

void Game::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode){
  if(sym == SDLK_ESCAPE)
    OnExit();
}

void Game::OnMouseFocus(){
}

void Game::OnMouseBlur(){
}

void Game::OnMinimize(){
}

void Game::OnRestore(){
}

void Game::OnResize(int w, int h){
}

void Game::OnUserEvent (int type, int code, void* data1, void* data2){
}



void Game::OnExpose(void){
}


void Game::OnMouseDown (MouseButton button, int x, int y){
}

void Game::OnMouseUp   (MouseButton button, int x, int y){
}


void Game::OnExit (void){
	Quit();
}


void Game::OnCleanup (void){
}



App * GetApp(){
	static Game game;
	return &game;
}
