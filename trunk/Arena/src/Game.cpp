#include "Game.h"
#include "Defs.h"
#include "GameObject.h"
#include "MainBrain.h"
#include <sys/time.h>
#include <list>

using namespace BugBots;

class ScannerPredicate 
{
public:
    ScannerPredicate(const QTCircle& area):m_area(area){}
    bool operator()(GameObject* pObject){
	QTCircle objCircle(pObject->GetPos(),kGameObjectRadius);
	if(objCircle.Intersects(m_area))
	    return true;
	else return false;	    
    }
private:
    QTCircle m_area;
};

class Scanner : public QTNode::OurVisitor
{
public:
    Scanner(std::list<GameObject*> &bucket):m_bucket(bucket){
    }
    
    virtual bool Visit(BugBots::GameObject* object,const BugBots::QTNode* node){
	m_bucket.push_back(object);
	return true;
    }
private:
    std::list<GameObject*>& m_bucket;
};


// class Updater : public QTNode::OurVisitor
// {
// public:
//     virtual bool Visit(BugBots::GameObject* object,const BugBots::QTNode* node){
// 	object->Update();
// 	return true;
//     }
// };

template <class T>
class Drawer : public QTNode::OurVisitor
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
    QTCircle circle(pObject->GetPos(),kGameObjectRadius);
    m_quadtree.Add(circle,pObject);
}

void Game::move_game_object(BugBots::GameObject* pObject, const QTVector& new_pos)
{
    m_quadtree.MoveObject(pObject,QTCircle(pObject->GetPos(),kGameObjectRadius),QTCircle(new_pos,kGameObjectRadius));
//   m_quadtree.Remove(QTCircle(pObject->GetPos(),kGameObjectRadius),pObject);
//   m_quadtree.Add(QTCircle(new_pos,kGameObjectRadius),pObject);
    pObject->SetPos(new_pos);
}

void Game::remove_game_object(BugBots::GameObject* pObject)
{
    m_quadtree.Remove(QTCircle(pObject->GetPos(),kGameObjectRadius),pObject);
}

void Game::traverse_circle(const QTCircle& circle, QTNode::OurVisitor& visitor )
{
    m_quadtree.Traverse(visitor,circle);
}

void Game::scan_area(const BugBots::QTCircle& circle, std::list<GameObject*> & bucket )
{
    Scanner scanner(bucket);
    ScannerPredicate predicate(circle);
    //m_quadtree.Traverse(scanner,circle);
    m_quadtree.Traverse(scanner,circle,predicate);
}


bool Game::OnInit(){
	m_screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF  ); 
	m_quadtree.Split();
	srand(time(NULL));
	MainBrain * blue_brain = new MainBrain(TEAM_BLUE);
	MainBrain * red_brain = new MainBrain(TEAM_RED);
	blue_brain->SetPos(QTVector(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT));
	red_brain->SetPos(QTVector(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT));
	add_game_object(blue_brain);
	add_game_object(red_brain);
	m_paused = false;
	return true;
}

void Game::OnLoop(){
    if(!m_paused){
	std::list<GameObject*> objects;
	Scanner scanner(objects);
	m_quadtree.TraverseAll(scanner);
	
	
	for(std::list<GameObject*>::iterator iter = objects.begin();
	    iter != objects.end(); iter++)
	    {
		(*iter)->Update();
	    }
    }
	
}

void Game::OnRender(){
	// Lock surface if needed
  if (SDL_MUSTLOCK(m_screen)) 
    if (SDL_LockSurface(m_screen) < 0) 
      return;

  // Ask SDL for the time in milliseconds
  int tick = SDL_GetTicks();

  SDL_FillRect( m_screen, &m_screen->clip_rect, SDL_MapRGB( m_screen->format, 0x0, 0x0, 0x0 ) ); 
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
  if(sym == SDLK_ESCAPE || sym ==SDLK_q)
     OnExit();
  else if(sym == SDLK_p || sym == SDLK_SPACE)
      m_paused = !m_paused;
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
