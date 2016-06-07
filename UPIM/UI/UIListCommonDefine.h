#ifndef UILISTCOMMONDEFINE_HPP
#define UILISTCOMMONDEFINE_HPP

#include <math.h>

namespace DuiLib
{
	struct FriendListItemInfo
	{
		bool folder;					// 是否是文件夹
		bool empty;						// 是否为空
		CDuiString id;					// id,可能会添加自定义标识：GROUP$001或者DEPT$002
		CDuiString numid;				// 纯ID,只包含用户ID
		CDuiString Parentid;			// 父ID
		CDuiString logo;				// 图标
		CDuiString nick_name;			// 昵称
		CDuiString showname;			// 在列表中显示的名称,可能是id/name/name(id)
		CDuiString description;			// 签名
		CDuiString groupnote;			// 群公告

		int type;						// EUSERTYPE,区分用户的具体类型，添加联系人的时候复用EADDIDTYPE(是否已经为群成员)
		int	roomtype;					// 群类型
		int rusertype;					// 群聊用户类型,区分用户是(客户/服务人员)
		int nodetype;					// 节点类型,在消息管理器中区分节点(个人/群组)
		int nstate;						// 用户在线状态
		bool bRefreshUserPic;			// 是否刷新形象,登录成功后只刷新一次

		CDuiString Lastword;			// 最后聊天记录
		CDuiString GoodAt;				// 擅长品种
		int	lastwordtime;				// 最后聊天记录时间
		bool isvip;						// 是否是vip
		bool hasright;					// 是否开通权限
		int totlenum;					// 总人数
		int onlinenum;					// 在线人数

		std::vector<RECEIVED_MSG> *pMsg;// 消息集合
		FriendListItemInfo()
		{
			folder = false;
			empty = false;
			id = _T("");
			numid = _T("");
			Parentid = _T("");
			logo = _T("");
			nick_name = _T("");
			showname = _T("");
			description = _T("");
			groupnote = _T("");

			type = 0;
			roomtype = 0;
			rusertype = 0;
			nodetype = 0;
			nstate = 0;
			bRefreshUserPic = false;

			Lastword = _T("");
			lastwordtime = 0;
			isvip = false;
			totlenum = 0;
			onlinenum = 0;

			pMsg = NULL;
		}
		// numid 是唯一的  只要numid一致  那么久认为他们相同 
		BOOL operator==(const FriendListItemInfo &other ) const
		{
			if( numid != other.numid )
				return false ; 
			else 
				return true ; 
		}
	};
	typedef vector<FriendListItemInfo>::iterator FriendList_ITER;

	struct NodeData
	{
		int level_;
		bool folder_;
		bool child_visible_;
		bool has_child_;
		CDuiString text_;
		CDuiString value;
		CDuiString id;
		CDuiString numid;				// 纯ID,只包含用户ID
		CDuiString Parentid;			// 父ID
		CDuiString nickname;

		// IM2.0
		CDuiString Lastword;			// 最后聊天记录
		CDuiString GoodAt;				// 擅长品种
		int	lastwordtime;				// 最后聊天记录时间
		bool isvip;						// 是否是vip
		bool hasright;					// 是否开通权限
		int totlenum;					// 总人数
		int onlinenum;					// 在线人数

		int type;						// EUSERTYPE
		int rusertype;
		int nodetype;
		CListContainerElementUI* list_elment_;


		NodeData& operator=(NodeData nodeData_)
		{
			if( list_elment_)
			{
				delete list_elment_ ; 
				list_elment_ = NULL ;
			}
			if( this != &nodeData_)
			{
				this->level_=nodeData_.level_;
				this->folder_=nodeData_.folder_;
				this->child_visible_=nodeData_.child_visible_;
				this->has_child_=nodeData_.has_child_;
				this->text_=nodeData_.text_;
				this->value=nodeData_.value;
				this->id=nodeData_.id;
				this->numid=nodeData_.numid;				// 纯ID,只包含用户ID
				this->Parentid=nodeData_.Parentid;			// 父ID
				this->nickname=nodeData_.nickname;

				// IM2.0	// IM2.0
				this->Lastword=nodeData_.Lastword;			// 最后聊天记录
				this->GoodAt=nodeData_.GoodAt;				// 擅长品种
				this->lastwordtime=nodeData_.lastwordtime;				// 最后聊天记录时间
				this->isvip=nodeData_.isvip;						// 是否是vip
				this->hasright=nodeData_.hasright;					// 是否开通权限
				this->totlenum=nodeData_.totlenum;					// 总人数
				this->onlinenum=nodeData_.onlinenum;					// 在线人数

				this->type=nodeData_.type;						// EUSERTYPE
				this->rusertype=nodeData_.rusertype;
				this->nodetype=nodeData_.nodetype;

				this->list_elment_ = nodeData_.list_elment_ ;  
			}
			return *this ; 
		}
	};

	double CalculateDelay(double state);

	class Node
	{
	public:
		Node();
		explicit Node(NodeData t);
		Node(NodeData t, Node* parent);
		~Node();
		NodeData& data();
		int num_children() const;
		Node* child(int i);
		Node* parent();
		bool folder() const;
		bool has_children() const;
		void add_child(Node* child);
		void remove_child(Node* child);
		Node* get_last_child();
		void SortChildren() ; 
		std::vector<Node*> GetChildList() ;
		void SetChildList(std::vector<Node*> vecChildList) ;
		Node& operator=( Node &node) ; 
	private:
		void set_parent(Node* parent);

	private:
		typedef std::vector<Node*>	Children;
		Children	children_;
		Node*		parent_;
		NodeData    data_;
	};
	int SortByName(Node*pData1 ,Node* pData2 ) ; 
} // DuiLib

#endif // UILISTCOMMONDEFINE_HPP