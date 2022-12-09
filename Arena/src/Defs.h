#ifndef _H_DEFS_H
#define _H_DEFS_H

#include "../../common/Quadtree/Node.hxx"
#include "../../common/Quadtree/Geometry.hxx"


const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 1200;
const int SCREEN_BPP = 32;
const int QUADTREE_SIZE = 1600;

#define QUADTREE_DEPTH 7

namespace BugBots {
    
    
    
using std::shared_ptr;

    
class GameObject;
typedef Quadtree::RootNode<shared_ptr<GameObject>,QUADTREE_DEPTH,int> QTRootNode;
typedef Quadtree::Node<shared_ptr<GameObject>,QUADTREE_DEPTH,int> QTNode;
typedef Quadtree::Geometry::Vector<int> QTVector;
typedef Quadtree::Geometry::Circle<int> QTCircle;
typedef Quadtree::Geometry::Square<int> QTSquare;

struct Color
{
    Color(unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia):r(ir),g(ig),b(ib),a(ia){}
    Color(){}
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};


// Enum for now, later will likely be class
enum class Team {
    TEAM_BLUE,
    TEAM_RED,
    _TEAM_COUNT_
};

const int kGameObjectRadius = 1;


};

#endif
