#include "SDL/SDL.h"
#include "Defs.h"
#include "Quadtree/Node.hxx"
#include "Quadtree/Geometry.hxx"


using Quadtree::Node;

typedef Quadtree::Node<int,4,int> QTNode;
typedef Quadtree::Geometry::Vector<int> QTVector;

int main( int argc, char* args[] ) { 
	//Start SDL 
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1){
		return 1;
	}
	QTNode tree(NULL,QTNode::Square(QTVector(0,0),800));
	SDL_Surface *screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 
	//Quit SDL 
	SDL_Quit(); 
	return 0; 
} 