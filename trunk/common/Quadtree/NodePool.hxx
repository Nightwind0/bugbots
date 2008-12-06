#ifndef QUADTREE_NODE_POOL_H
#define QUADTREE_NODE_POOL_H

#include <list>
#include "Geometry.hxx"

namespace Quadtree
{
	template <class T,int max_depth,class Scalar,int max_object_radius,bool auto_split>
	class NodePool
	{
	public:
		typedef Node<T,max_depth,Scalar,max_object_radius,auto_split> NodeType;
		NodePool(){}
		~NodePool();

		virtual NodeType * GetNode(NodeType *pParent, const Geometry::Square<Scalar> &);
		virtual void Return(NodeType *pNode); // Required to accept NULL, as a no-op
		/// Delete any spare nodes
		virtual void Prune();
	private:
		std::list<NodeType*> m_pool;
	};

	template <class T,int max_depth,class Scalar,int max_object_radius,bool auto_split>
	NodePool::~NodePool()
	{
		Prune();
	}


	template <class T,int max_depth,class Scalar,int max_object_radius,bool auto_split>
	NodePool::Prune()
	{
		for(std::list<NodeType*>::iterator it = m_pool.begin();
			it != m_pool.end(); it++)
		{
			delete *it;
		}

		m_pool.clear();
	}

	template <class T,int max_depth,class Scalar,int max_object_radius,bool auto_split>
	NodePool::GetNode(NodeType *pParent, const Geometry::Square<Scalar> &quad)
	{
		if(m_pool.empty())
		{
			return new NodeType(pParent,quad);
		}
		else
		{
			NodeType * node = m_pool.front();
			node->Clear(pParent,quad);
			return node;
		}
	}
	template <class T,int max_depth,class Scalar,int max_object_radius,bool auto_split>
	void NodePool::Return(NodeType *pNode)
	{
		if(pNode != NULL)
		{
			m_pool.push_back(pNode);
		}
	}
}


#endif