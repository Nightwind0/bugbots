#include "SDL/SDL.h"
#include "Defs.h"
#include "App.h"

void render(SDL_Surface * screen){

}


int main( int argc, char* args[] ) { 
	//Start SDL 

	App * app = GetApp();

	QTNode tree(NULL,QTNode::Square(QTVector(0,0),800));

	return app->Run();
} 