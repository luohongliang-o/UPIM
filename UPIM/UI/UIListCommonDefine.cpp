#include "stdafx.h"
#include "UIListCommonDefine.h"

namespace DuiLib
{

	double CalculateDelay(double state)
	{
		return pow(state, 2);
	}

	void Node::set_parent(Node* parent)
	{
		parent_ = parent;
	}

	///////////////////////////////////////////////////////////////////////
	//作为底层的节点  本身不具备排序功能  因为节点本身没有是否在线标志位，
	//上层是通过m_vtRoomClient中的节点按照在线和姓名二维排序，然后往每个FriendUIList
	//进行插入节点  第一次进来实际是按照一个静态顺序进行插入，插入时已经有序；
	//所以 重新插入排序的时候 还是要参考节点在m_vtRoomClient中的位置 
	// wuchao add at 2016-03-29 
	///////////////////////////////////////////////////////////////////////
	void Node::SortChildren() 
	{
		//sort(children_.begin() ,children_.end() ,SortByName) ; 
	}

	/////////////////////////////////////////////////////////////////////////
	//wuchao add at 2016-03-29
	// 获取父节点的孩子节点列表
	/////////////////////////////////////////////////////////////////////////
	std::vector<Node*> Node::GetChildList()
	{
		return children_ ;
	}
	/////////////////////////////////////////////////////////////////////////
	//wuchao add at 2016-03-29
	// 设置孩子节点  
	/////////////////////////////////////////////////////////////////////////
	void Node::SetChildList(std::vector<Node*> vecChildList) 
	{
		for (int i = 0; i < num_children(); ++i)
		{
			children_[i] = vecChildList[i]; 
		}
		//children_ = vecChildList ; 
	}
	////////////////////////////////////////////////////////////////////////
	//wuchao add at 2016-03-29 
	////////////////////////////////////////////////////////////////////////
	//Node& Node::operator=( Node &node_)
	//{
	//	if( this != &node_ )
	//	{
	//		this->parent_   = node_.parent() ; 
	//		this->children_ = (*(node_.GetChildList())) ; 
	//		this->data_     = node_.data() ; 
	//	}
	//	return (*this) ; 
	//}
	Node::Node()
		: parent_(NULL)
	{}

	Node::Node(NodeData t)
		: data_(t)
		, parent_(NULL)
	{}

	Node::Node(NodeData t, Node* parent)
		: data_ (t)
		, parent_ (parent)
	{}

	Node::~Node() 
	{
		for (int i = 0; i < num_children(); ++i)
			delete children_[i]; 
	}

	NodeData& Node::data()
	{
		return data_;
	}

	int Node::num_children() const
	{
		return static_cast<int>(children_.size());
	}

	Node* Node::child(int i)
	{
		return children_[i];
	}

	Node* Node::parent()
	{
		return ( parent_);
	}

	bool Node::has_children() const
	{
		return num_children() > 0;
	}

	bool Node::folder() const
	{
		return data_.folder_;
	}

	void Node::add_child(Node* child)
	{
		child->set_parent(this); 
		children_.push_back(child); 
	}


	void Node::remove_child(Node* child)
	{
		Children::iterator iter = children_.begin();
		for (; iter < children_.end(); ++iter)
		{
			if (*iter == child) 
			{
				children_.erase(iter);
				return;
			}
		}
	}

	Node* Node::get_last_child()
	{
		if (has_children())
		{
			return child(num_children() - 1)->get_last_child();
		}
		return this;
	}


	int SortByName(Node*pData1 ,Node* pData2 ) 
	{
		return strcmp( pData1->data().nickname ,pData2->data().nickname) < 0  ; 
	}

}