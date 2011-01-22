#ifndef _H_DEFS_H
#define _H_DEFS_H

#include "../../common/Quadtree/Node.hxx"
#include "../../common/Quadtree/Geometry.hxx"
#include <tr1/memory>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

namespace BugBots {
    
class GameObject;
typedef Quadtree::RootNode<GameObject*,4,int> QTRootNode;
typedef Quadtree::Node<GameObject*,4,int> QTNode;
typedef Quadtree::Geometry::Vector<int> QTVector;
typedef Quadtree::Geometry::Circle<int> QTCircle;

struct Color
{
    char r;
    char g;
    char b;
    char a;
};

template <typename T> struct SharedPtr { typedef std::tr1::shared_ptr<T> Type; };

};

#endif
