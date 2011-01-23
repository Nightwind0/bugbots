#ifndef _H_DEFS_H
#define _H_DEFS_H

#include "../../common/Quadtree/Node.hxx"
#include "../../common/Quadtree/Geometry.hxx"
#include <tr1/memory>

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int SCREEN_BPP = 32;

namespace BugBots {
    
class GameObject;
typedef Quadtree::RootNode<GameObject*,3,int> QTRootNode;
typedef Quadtree::Node<GameObject*,3,int> QTNode;
typedef Quadtree::Geometry::Vector<int> QTVector;
typedef Quadtree::Geometry::Circle<int> QTCircle;

struct Color
{
    Color(unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia):r(ir),g(ig),b(ib),a(ia){}
    Color(){}
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

template <typename T> struct SharedPtr { typedef std::tr1::shared_ptr<T> Type; };

// Enum for now, later will likely be class
enum Team {
    TEAM_BLUE,
    TEAM_RED
};

const int kGameObjectRadius = 1;


};

#endif
