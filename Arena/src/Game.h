#ifndef _H_BBA_GAME_H_
#define _H_BBA_GAME_H_

#include "App.h"
#include "Defs.h"
#include <mutex>
#include <thread>
#include "SteelInterpreter.h"

using namespace BugBots;

class Game : public App{
public:
	Game();
	virtual ~Game();
	virtual bool OnInit();
	virtual void OnLoop();
	virtual void OnRender();
	virtual void OnMouseMove(int x, int y);
	virtual void OnKeyDown(sf::Event const& event);
	virtual void OnKeyUp(sf::Event const& event);
	virtual void OnMouseFocus();
	virtual void OnMouseBlur();
	virtual void OnMinimize();
	virtual void OnRestore();
	virtual void OnResize(int w, int h);
	virtual void OnUserEvent (int type, int code, void* data1, void* data2);
	virtual void OnExpose(void);

	virtual void OnMouseDown (MouseButton button, int x, int y);
	virtual void OnMouseUp   (MouseButton button, int x, int y);

	virtual void OnExit (void);
	virtual void OnCleanup (void);
	
	static BugBots::QTVector WorldToView(const BugBots::QTVector& pos); 
	static BugBots::QTVector ViewToWorld(int x, int y);
	
	std::ostream& log();
protected:
	virtual sf::RenderWindow &GetScreen() {
		return m_screen;
	}
	void add_game_object(shared_ptr<BugBots::GameObject> pObject);
	void move_game_object(shared_ptr<BugBots::GameObject> pObject, const BugBots::QTVector& new_pos);
	void remove_game_object(shared_ptr<BugBots::GameObject> pObject);
	void traverse_circle(const BugBots::QTCircle& circle, BugBots::QTNode::OurVisitor& visitor );
	void scan_area(const BugBots::QTCircle& circle, std::list<shared_ptr<BugBots::GameObject> > & bucket );
	void game_object_update_thread(const std::list<shared_ptr<BugBots::GameObject>>& objects);
	void add_bifs(Steel::SteelInterpreter*);
private:
	friend class BugBots::GameObject;
	void DrawPixel(BugBots::Color color, int x, int y){
        sf::Color const c(color.r, color.g, color.b, color.a);
	    App::DrawPixel(m_screen,c,x,y);
	}
	void DrawSquare(BugBots::Color color, int x, int y, int size){
        sf::Color const c(color.r, color.g, color.b, color.a);
	    App::DrawSquare(m_screen,c,x,y,size);
	}	
	sf::RenderWindow m_screen;
	bool m_paused;
	bool m_drawNodes;
	int m_last_obj_count;
	std::mutex m_qt_mutex;
	BugBots::QTRootNode m_quadtree;
};

#endif
