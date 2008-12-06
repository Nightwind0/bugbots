#include "Node.hxx"
#include "Definitions.hxx"
#include "Iterator.hxx"

using namespace Quadtree;


template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
Node::Node(Node *pParent, const Rect &quad)
        :m_pParent(pParent),m_pTopleft(NULL),m_pTopright(NULL),
         m_pBottomleft(NULL),m_pBottomright(NULL),m_quad(quad)
{
	// We might want to start out with all nodes in existance
    if(auto_split && calculate_depth() < max_depth)
		split();
}
 
/**
 * @brief Creates a new node
 * @param pParent Vectorer to the parent of this node
 * @param max_depth The maximum depth this tree should build to
 * @param node_depth The depth this node will exist at
 * @param auto_split Whether to automatically split
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
Node::~Node()
{
    delete m_pTopleft;
    delete m_pTopright;
    delete m_pBottomleft;
    delete m_pBottomright;
}

template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
Scalar Node::GetMaxObjectRadius() const
{
	Scalar radius = m_quad.GetDiagonalRadius();
	return radius + (radius * max_object_radius);
}
/**
 * @brief Add an object to the appropriate node at or below
 * this node
 *
 * @param bounds The rectangle in which the object currently
 * exists
 * @param t the object
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::Add(const Circle &bounds,const T &t)
{
	Scalar radius = bounds.GetRadius();
	uint cur_depth = calculate_depth();
	Scalar myradius = GetMaxObjectRadius();
	if(cur_depth == max_depth || radius / (Scalar)2 > myradius)
	{
		// Too big for any children, add it to me
		if(bounds.GetRadius() <= myradius)
			add_specific(bounds.GetCenter(),t);
		else
			assert(0 && "Tried to add an object that was too big.");
	}
	else
	{
		eQuadrant equad = which_quad(bounds.GetCenter());
		NodePtr & ptr = which_child(equad);
		if(ptr == NULL)
		{
			create_child(ptr,equad);
		}
		assert(ptr != NULL);
		// Mmm... recursivey
		ptr->Add(bounds,t);
	}
}
        
/**
 * @brief Remove the object that exists at or below this node
 *
 * @param center Where the object exists
 * @param it iterator to the object
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::Remove(const Circle &bounds,const T &t)
{
	
}

/**
 * @brief Whether a Vector falls within the bounds of this quad
 * @note the parent node should know this without asking
 * based on which child we are.
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
bool Node::InBounds(const Vector &vector)const
{
	Scalar halfsize = m_quad.GetSize() / (Scalar)2;

	if(abs<Scalar>(vector.GetX() - m_quad.GetCenter().GetX()) <= halfsize
		&& abs<Scalar>(vector.GetY() - m_quad.GetCenter().GetY()) <= halfsize)
	{
		return true;
	}

	return false;
}

/**
 * @brief Determine if a circle intersects the bounding circle
 *
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
bool Node::Intersects(const Circle &circle) const
{
	Circle mybounds = calculate_bounds();
	return mybounds.Intersects(circle);
}
        
/**
 * @brief Calls the traverser on each object within the view circle
 *
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::Traverse(Traverser<T> &traverser, const Circle &circle)
{
	// First, my objects.
	for(ObjectContainer::iterator it = m_objects.begin();
		it != m_objects.end(); it++)
	{
		// Visit and determine if I should stop traversal
		if(!traverser->Visit(*it))
			return false;
	}
	bool stop = false;
	// Now, see if this intersects any of my children
	if(m_pTopLeft && m_pTopLeft->Intersects(circle))
		stop = m_pTopLeft->Traverse(traverser,circle);
	if(!stop && m_pTopRight && m_pTopRight->Intersects(circle))
		stop = m_pTopRight->Traverse(traverser,circle);
	if(!stop && m_pBottomLeft && m_pBottomLeft->Intersects(circle))
		stop = m_pBottomLeft->Traverse(traverser,circle);
	if(!stop && m_pBottomRight && m_pBottomRight->Intersects(circle))
		stop = m_pBottomRight->Traverse(traverser,circle);

	return stop;
}

        
/**
 * @brief Calls the traverser on all objects of this node and below
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::TraverseAll(Traverser<T> &traverser)
{
}
                
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
Scalar Node::calculate_diagonal_radius()const
{
	const static double sqrt2 = std::sqrt(2.0);
	return (Scalar)((m_quad.GetSize() * sqrt2) / (Scalar2)2);
}

/// Recursively climbs tree to find out my depth
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
uint Node::calculate_depth()const
{
	// this COULD be made into a for loop.. but it should be fine, since we won't be very deep
	if(m_pParent == NULL)
	   return 0;
	else
		return 1 + m_pParent->calculate_depth();

}

/// Find my boundaries
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
Circle Node::calculate_bounds()const
{
	return Circle(m_quad.GetCenter(),GetMaxObjectRadius());
}

/**
 * @brief Add an object to this node
 * @param p The Vector at which the object currently exists
 * @param t A Vectorer to the object
 */
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::add_specific(Vector p, const T &t)
{
	m_objects.push_back(t);
}
/// Remove by iterator from this node
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::remove_specific(const ObjectContainer::iterator &it)
{
}


// Creates children nodes, and places its nodes within them
// Unless max_depth is met
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::split()
{
	create_child(m_pTopLeft, calculate_quad(ETOPLEFT));
	create_child(m_pTopRight, calculate_quad(ETOPRIGHT));
	create_child(m_pBottomLeft, calculate_quad(EBOTTOMLEFT));
	create_child(m_pBottomRight, calculate_quad(EBOTTOMRIGHT));
}
template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
Node::NodePtr & Node::which_child(Node::eQuadrant quad)
{
	switch(quad)
	{
	case ETOPLEFT:
		return m_pTopLeft;
	case ETOPRIGHT:
		return m_pTopRight;
	case EBOTTOMLEFT:
		return m_pBottomLeft;
	case EBOTTOMRIGHT:
		return m_pBottomRight;
	default:
		assert(0);
		return m_pTopLeft; // arbitrary
	};
}

template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
Node::eQuadrant Node::which_quad(const Vector &center)
{
	bool left = true;
	bool top = true;

	if(center.GetX() > m_quad.GetX())
		left = false; // it's right of my center
	if(center.GetY() > m_quad.GetY())
		top = false; // it's below center

	if(top && left) return ETOPLEFT;
	else if(top && !left) return ETOPRIGHT;
	else if(!top && left) return EBOTTOMLEFT;
	else return EBOTTOMRIGHT; 
}


template <class T,unsigned int max_depth, class Scalar, float max_object_radius, bool auto_split>
void Node::create_child(NodePtr &ptr, Node::eQuadrant quad)
{
	ptr = new Node<T,max_depth,Scalar,max_object_radius,auto_split>(this,calculate_quad(quad));
}

Node::Square Node::calculate_quad(eQuadrant quad)
{
	Scalar halfsize = m_quad.GetSize() / (Scalar)2;
	Scalar quartersize = halfsize / (Scalar)2;
	switch(quad)
	{
	default:
		assert(0); //fallthrough, to quell compiler
	case ETOPLEFT:
		return Square(Vector(m_quad.GetCenter().GetX() - quartersize,
			m_quad.GetCenter().GetY() - quartersize), halfsize);
	case ETOPRIGHT:
		return Square(Vector(m_quad.GetCenter().GetX() + quartersize,
			m_quad.GetCenter().GetY() - quartersize), halfsize);
	case EBOTTOMLEFT:
		return Square(Vector(m_quad.GetCenter().GetX() - quartersize,
			m_quad.GetCenter().GetY() + quartersize,halfsize);
	case EBOTTOMRIGHT:
		return Square(Vector(m_quad.GetCenter().GetX() + quartersize,
			m_quad.GetCenter().GetY() + quartersize,halfsize);
	}
}