#include "Game.h"
#include "Defs.h"
#include "Clump.h"
#include "GameObject.h"
#include "MainBrain.h"
#include "Utilities.h"
#include <sys/time.h>
#include <list>
#include <iostream>
#include "BugBot.h"

using namespace BugBots;

class ScannerPredicate 
{
public:
    ScannerPredicate(const QTCircle& area):m_area(area){}
    bool operator()(shared_ptr<GameObject> pObject){
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
    Scanner(std::list<shared_ptr<GameObject> > &bucket):m_bucket(bucket){
    }
    
    virtual bool Visit(shared_ptr<BugBots::GameObject> object,const BugBots::QTNode* node){
	m_bucket.push_back(object);
	return true;
    }
private:
    std::list<shared_ptr<GameObject> >& m_bucket;
};

class GameScanner : public QTNode::OurVisitor
{
public:
  GameScanner(std::vector<std::list<shared_ptr<GameObject>>>& buckets):m_buckets(buckets){
  }
    
  virtual bool Visit(shared_ptr<BugBots::GameObject> object,const BugBots::QTNode* node){
    TeamObject* teamobj = dynamic_cast<TeamObject*>(object.get());
    if(teamobj){
      m_buckets[teamobj->GetTeam()].push_back(object);
    }else{
      m_buckets[_TEAM_COUNT_].push_back(object);
    }
    return true;
  }
private:
  std::vector<std::list<shared_ptr<GameObject>>>& m_buckets;
};



#if 0 
 class Updater : public QTNode::OurVisitor
 {
 public:
   virtual bool Visit(shared_ptr<BugBots::GameObject> object,const BugBots::QTNode* node){
 	object->Update(object);
 	return true;
     }
 };
#endif

template <class T>
class Drawer : public QTNode::OurVisitor
{
public:
    typedef void (T::*DrawFunctor)(BugBots::Color, int, int);
    Drawer(T* pApp,DrawFunctor functor):m_pApp(pApp),m_functor(functor){
        
    }
    
    virtual bool Visit(shared_ptr<BugBots::GameObject> object,const BugBots::QTNode* node){
        BugBots::Color color = object->GetColor();
        BugBots::QTVector pos = Game::WorldToView(object->GetPos());
        (m_pApp->*m_functor)(color, pos.GetX(),pos.GetY());
        return true;
    }
private:
    T* m_pApp;
    DrawFunctor m_functor;
};




template <class T>
class Counter : public QTNode::OurVisitor
{
public:
    typedef void (T::*DrawFunctor)(BugBots::Color, int, int);
    Counter(T* pApp,DrawFunctor functor):m_pApp(pApp),m_functor(functor),m_red(0),m_blue(0){
        
    }
    
    virtual bool Visit(shared_ptr<BugBots::GameObject> object,const BugBots::QTNode* node)
    {
        shared_ptr<BugBot> pBB = std::tr1::dynamic_pointer_cast<BugBot>(object);
        if (pBB && !pBB->HasFlag(BugBot::DEAD))
        {
            switch (pBB->GetTeam())
            {
                case TEAM_RED:
                    m_red++;
                    break;
                case TEAM_BLUE:
                    m_blue++;
                    break;
                default:
                    break;
            }
        }
        return true;
    }
    
    void Draw()
    {
        if (m_blue == 0 && m_red == 0)
        {
            return;
        }
        else
        {
            float count = m_red + m_blue;
            int i = 0;
            float pRed = ((float)m_red)/count;
            float pBlue = ((float)m_blue)/count;

            while(i < (int)(pBlue * SCREEN_WIDTH))
            {
                (m_pApp->*m_functor)(Utilities::DefaultTeamColor(TEAM_BLUE),i,SCREEN_HEIGHT);
                i++;
            }
            while (i <= SCREEN_WIDTH)
            {
                (m_pApp->*m_functor)(Utilities::DefaultTeamColor(TEAM_RED),i,SCREEN_HEIGHT);
                i++;
            }
        }

    }
    
private:
    T* m_pApp;
    DrawFunctor m_functor;
    int m_red;
    int m_blue;
};

template <class T>
class NodeDrawer : public QTNode::OurNodeVisitor
{
public:
    typedef void (T::*DrawFunctor)(BugBots::Color, int, int, int);
    NodeDrawer(T* pApp,DrawFunctor functor):m_pApp(pApp),m_functor(functor){
		
    }
    
    virtual bool Visit(const BugBots::QTNode* node){
		BugBots::Color color = Utilities::CreateColor(0.1,0.1,0.1);
		BugBots::QTVector pos = Game::WorldToView(node->GetSquare().GetCenter());
		int size = node->GetSquare().GetSize();
		(m_pApp->*m_functor)(color, pos.GetX(),pos.GetY(),size);
		return true;
    }
private:
	T* m_pApp;
	DrawFunctor m_functor;
};

Game::Game():m_quadtree(BugBots::QTNode::Square(BugBots::QTVector(0,0),QUADTREE_SIZE)),m_drawNodes(false){
}

Game::~Game(){
}

void Game::add_game_object(shared_ptr<GameObject> pObject)
{
  std::unique_lock<std::mutex> lock(m_qt_mutex);
    QTCircle circle(pObject->GetPos(),kGameObjectRadius);
    m_quadtree.Add(circle,pObject);
}

void Game::move_game_object(shared_ptr<BugBots::GameObject> pObject, const QTVector& new_pos)
{
  std::unique_lock<std::mutex> lock(m_qt_mutex);
    m_quadtree.MoveObject(pObject,QTCircle(pObject->GetPos(),kGameObjectRadius),QTCircle(new_pos,kGameObjectRadius));
//   m_quadtree.Remove(QTCircle(pObject->GetPos(),kGameObjectRadius),pObject);
//   m_quadtree.Add(QTCircle(new_pos,kGameObjectRadius),pObject);
    pObject->SetPos(new_pos);
}

void Game::remove_game_object(shared_ptr<BugBots::GameObject> pObject)
{
  std::unique_lock<std::mutex> lock(m_qt_mutex);
    m_quadtree.Remove(QTCircle(pObject->GetPos(),kGameObjectRadius),pObject);
}

void Game::traverse_circle(const QTCircle& circle, QTNode::OurVisitor& visitor )
{
  std::unique_lock<std::mutex> lock(m_qt_mutex);
    m_quadtree.Traverse(visitor,circle);
}

void Game::scan_area(const BugBots::QTCircle& circle, std::list<shared_ptr<GameObject> > & bucket )
{
    Scanner scanner(bucket);
    ScannerPredicate predicate(circle);
    //m_quadtree.Traverse(scanner,circle);
    std::unique_lock<std::mutex> lock(m_qt_mutex);
    m_quadtree.Traverse(scanner,circle,predicate);
}


bool Game::OnInit(){
	m_screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT + 1, 0, SDL_HWSURFACE | SDL_DOUBLEBUF  ); 
	srand(time(NULL));
	shared_ptr<GameObject> blue_brain = shared_ptr<GameObject>(new MainBrain(TEAM_BLUE));
	shared_ptr<GameObject> red_brain = shared_ptr<GameObject>(new MainBrain(TEAM_RED));
	shared_ptr<GameObject> clump1 = shared_ptr<GameObject>(new Clump());
	shared_ptr<GameObject> clump2 = shared_ptr<GameObject>(new Clump());
	for(int t=0;t<kThreadCount;t++){
	  // add_bifs(m_interpreters[t]);
	}
	blue_brain->SetPos(Utilities::RandomPosition());
	red_brain->SetPos(Utilities::RandomPosition());
	clump1->SetPos(Utilities::RandomPosition());
	clump2->SetPos(Utilities::RandomPosition());
	add_game_object(blue_brain);
	add_game_object(red_brain);
	add_game_object(clump1);
	add_game_object(clump2);
	m_last_obj_count = 4;
	m_paused = false;
	m_drawNodes = false;
	m_last_ticks = SDL_GetTicks();
	return true;
}

void Game::OnLoop(){
  static int loop_count = 0;
  static long total_ticks = 0;
  ++loop_count;
    if(!m_paused){
      int start_ticks = SDL_GetTicks();
      std::vector<std::list<shared_ptr<GameObject>>> buckets(_TEAM_COUNT_+1);
	GameScanner scanner(buckets);
	m_quadtree.TraverseAll(scanner);

#if 1	
	assert ( kThreadCount == _TEAM_COUNT+1 );
	std::thread threads[kThreadCount];
	for(int t=0;t<kThreadCount;t++){
	  threads[t] = std::thread([this,&buckets,t](){
	      this->game_object_update_thread(buckets[t]);
	    });
	}

	for(int t=0;t<kThreadCount;t++){
	  threads[t].join();
	}

#else
	for(std::vector<shared_ptr<GameObject> >::iterator iter = objects.begin();
	    iter != objects.end(); iter++)
	    {
	      (*iter)->Update(*iter, m_interpreters[0]);
	    }
#endif
	int end_ticks = SDL_GetTicks();
	total_ticks += end_ticks - start_ticks;
	if(loop_count % 300 == 0){
	  std::cout << "Update took " << end_ticks - start_ticks << " ticks." << std::endl;
	}
    }
	
}

void Game::OnRender(){
  static int frame_count = 0;
  ++frame_count;
	// Lock surface if needed
    if (SDL_MUSTLOCK(m_screen)) 
        if (SDL_LockSurface(m_screen) < 0) 
            return;
    
    // Ask SDL for the time in milliseconds
    int ticks = SDL_GetTicks();
    int time_diff = ticks - m_last_ticks;
    if(frame_count % 1000 == 0)
      std::cout << time_diff << "ms since last render" << std::endl;
    m_last_ticks = ticks;
    
    SDL_FillRect( m_screen, &m_screen->clip_rect, SDL_MapRGB( m_screen->format, 0x0, 0x0, 0x0 ) ); 
    // Draw Quadtree Nodes
    if(m_drawNodes)
    {
        NodeDrawer<Game> nodedrawer(this,&Game::DrawSquare);
        m_quadtree.TraverseNodes(nodedrawer);
    }
    // Draw to screen
    Drawer<Game> drawer(this,&Game::DrawPixel);
    m_quadtree.TraverseAll(drawer);
    
    Counter<Game> counter(this,&Game::DrawPixel);
    m_quadtree.TraverseAll(counter);
    counter.Draw();
    
    // Unlock if needed
    if (SDL_MUSTLOCK(m_screen)) 
        SDL_UnlockSurface(m_screen);
    
    // Tell SDL to update the whole screen
    SDL_UpdateRect(m_screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
    SDL_Flip(m_screen);
    SDL_Delay(0);
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
  else if(sym == SDLK_n)
      m_drawNodes = !m_drawNodes;
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
#ifndef NDEBUG
    BugBots::QTVector point = ViewToWorld(x,y);
    QTCircle circle(point,1.0);
    std::list<shared_ptr<GameObject> > contacts;
    scan_area(circle,contacts);
    std::cout << "---------------------------------" << std::endl;
    for(std::list<shared_ptr<GameObject> >::iterator iter= contacts.begin();
	iter != contacts.end(); iter++)
	{

	    shared_ptr<BugBots::BugBot> pBB = std::tr1::dynamic_pointer_cast<BugBots::BugBot>(*iter);
	    if(pBB)
	    {
		std::cout << "Bugbot at (" << pBB->GetPos().GetX() << ',' << pBB->GetPos().GetY() << ')' << std::endl;
		const char * team_name = pBB->GetTeam()==BugBots::TEAM_BLUE?"Blue":"Red";
		std::cout << "Team is " <<  team_name << std::endl;
		if(pBB->HasFlag(BugBot::DEAD))
		    std::cout << "(DEAD)" << std::endl;
		std::cout << *pBB;
	    }
	}
    std::cout << "---------------------------------" << std::endl;
#endif
}

void Game::OnMouseUp   (MouseButton button, int x, int y){
}


void Game::OnExit (void){
	Quit();
}


void Game::OnCleanup (void){
}

BugBots::QTVector Game::WorldToView(const BugBots::QTVector& pos)
{
    return BugBots::QTVector(pos.GetX() + SCREEN_WIDTH/2, pos.GetY() + SCREEN_HEIGHT/2);
}

BugBots::QTVector Game::ViewToWorld(int x, int y)
{
    return BugBots::QTVector(x - SCREEN_WIDTH/2,y - SCREEN_HEIGHT/2);
}

std::ostream& Game::log()
{
    return std::cout;
}

void Game::add_bifs(Steel::SteelInterpreter* pInterpreter)
{
}

void Game::game_object_update_thread(const std::list<shared_ptr<BugBots::GameObject>>& objects)
{
  for(auto it = objects.begin(); it != objects.end(); it++){
    (*it)->Update(*it);
  }
}


App * GetApp(){
	static Game game;
	return &game;
}
