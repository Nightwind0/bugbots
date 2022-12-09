#ifndef _H_APP_H_
#define _H_APP_H_

#include <SFML/Graphics.hpp>

class App{
public:
	App();
	virtual ~App(){}

	void ProcessEvent(sf::Event const &event);
	int  Run();

	virtual bool OnInit();
	virtual void OnLoop();
	virtual void OnRender();
	virtual void OnMouseMove(int x, int y);
	virtual void OnKeyDown(sf::Event const &);
	virtual void OnKeyUp(sf::Event const&);
	virtual void OnMouseFocus();
	virtual void OnMouseBlur();
	virtual void OnMinimize();
	virtual void OnRestore();
	virtual void OnResize(int w, int h);
	virtual void OnUserEvent (int type, int code, void* data1, void* data2);
	virtual bool OnRawEvent(sf::Event const &);
	virtual void OnExpose(void);


	enum MouseButton{
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};

	virtual void OnMouseDown (MouseButton button, int x, int y);
	virtual void OnMouseUp   (MouseButton button, int x, int y);

	virtual void OnExit (void);
	virtual void OnCleanup (void);
	
	virtual void Log(const char*str);

protected:
	void Quit();
	void DrawCircle(sf::RenderTarget &target, sf::Color const &color, int x, int y);
	void DrawSquare(sf::RenderTarget&,sf::Color const &color, int x, int y, int size);
	virtual sf::RenderWindow &GetScreen()=0;
private:

	bool m_running;
	int m_ticks;

};

App* GetApp();

#define LOG(x) GetApp()->Log(x)

#endif