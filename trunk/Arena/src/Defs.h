#ifndef _H_DEFS_H
#define _H_DEFS_H

#include "../../common/Quadtree/Node.hxx"
#include "../../common/Quadtree/Geometry.hxx"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;



typedef Quadtree::Node<int,4,int> QTNode;
typedef Quadtree::Geometry::Vector<int> QTVector;


#endif
