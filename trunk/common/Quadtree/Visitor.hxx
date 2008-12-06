
#ifndef QUADTREE_VISITOR_H
#define QUADTREE_VISITOR_H

namespace Quadtree
{
    template<class T>
    class Visitor
    {
    public:
		/**
		* @brief visit an object in the quadtree
		* @return Whether to continue traversal
		*/
        virtual bool Visit(T)=0;
    };
}

#endif

