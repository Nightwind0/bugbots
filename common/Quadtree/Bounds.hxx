#ifndef QUADTREE_BOUNDS_H
#define QUADTREE_BOUNDS_H

#include "Geometry.hxx"

namespace Quadtree{
    /**
     * @brief Represents the boundaries of an object
     *
     * Default implementation is AABB, no rotation.
     * (Axially-aligned Bounding Box)
     * It is possible to do circles as well, allowing rotations
     */
    class Bounds
    {
    public:
        Bounds();
        Bounds(const Rect &); // Convert from a rectangle
        virtual ~Bounds();

        virtual Point GetCenter()const;
        virtual Circle GetBoundingCircle()const;
        virtual Rect GetBoundingRect()const;
        virtual bool IsContainedBy(const Rect &r)const;
        virtual bool IsContainedBy(const Circle &c)const;
        virtual bool Intersects(const Rect &r)const;
        virtual bool Intersects(const Circle &c)const;

    private:
        Rect m_rect;
    };

    class CircleBounds : public Bounds
    {
    public:
        CircleBounds();
        CircleBounds(const Circle &c);
        virtual ~CircleBounds();

        virtual Point GetCenter()const;
        virtual Circle GetBoundingCircle()const;
        virtual Rect GetBoundingRect()const;
        virtual bool IsContainedBy(const Rect &r)const;
        virtual bool IsContainedBy(const Circle &c)const;
        virtual bool Intersects(const Rect &r)const;
        virtual bool Intersects(const Circle &c)const;
    private:
        Circle m_circle;
    };
}


#endif
