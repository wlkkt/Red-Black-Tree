#pragma once

#include <iostream>
#include<vector>

using namespace std;

enum Colour
{
	RED,
	BLACK
};

template<class K, class V>
struct RBTreeNode
{
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	pair<K, V> _kv;
	Colour _col;

	RBTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _col(RED)
	{}
};



template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			_root->_col = BLACK;//规定插入的根结点颜色为黑色
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}

		cur = new Node(kv);
		cur->_col = RED; // 规定新增节点的颜色是红色
		if (parent->_kv.first < kv.first)
		{
			parent->_right = cur;
		}
		else
		{
			parent->_left = cur;
		}
		cur->_parent = parent;

		// 当parent指向的结点不为空，且parent指向的结点的颜色不为黑色就继续向上调整（因为cur和parent均为红可能是由新增和向上调整导致的，因此无论是这两种情况的哪一种，
		// 只有当cur和parent均不为红时才结束循环）
		while (parent && parent->_col == RED)
		{
			
			Node* grandfather = parent->_parent;//通过父亲找到爷爷，父亲的父亲信息是爷爷

			//红黑树的关键点在于判断叔叔的情况
			if (parent == grandfather->_left)//如果父亲是爷爷的左孩子，那么叔叔就只能是爷爷的右孩子
			//这里if中的叔叔一直都是爷爷的右孩子，不过还会存在为红、为黑、不存在的三种情况
			{
				Node* uncle = grandfather->_right;

				
				// 如果叔叔存在且叔叔的颜色是红色，则直接将叔叔和父亲的颜色都变为黑色，将爷爷的颜色变为红色
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					// 防止修改颜色导致上方出现新的两个连续的红色结点，需要移动指针继续向上处理
					cur = grandfather;//cur指向爷爷结点
					parent = cur->_parent;//parent指向的是cur的父结点（cur向上走，parent也要向上走）
				}
				else // 叔叔不存在，或者存在且为黑
				{
					if (cur == parent->_left)//如果cur是父亲的左孩子，进行LL右单，旋转后将父亲的颜色变为黑，将爷爷的颜色变为红
					{
						//     g  
						//   p   u
						// c 
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else
					{
						//      g  
						//   p     u
						//      c 
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}



			else//如果父亲结点是爷爷结点的右孩子，那么叔叔结点就是爷爷结点的左孩子
			{
				Node* uncle = grandfather->_left;
				// 叔叔存在且为红，-》变色即可
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					// 继续往上处理
					cur = grandfather;
					parent = cur->_parent;
				}
				else // 叔叔不存在，或者存在且为黑
				{
					// 情况二：叔叔不存在或者存在且为黑
					// 旋转+变色
					//      g
					//   u     p
					//            c
					if (cur == parent->_right)
					{
						RotateL(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else
					{
						//		g
						//   u     p
						//      c
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}
		}

		_root->_col = BLACK;//不管最后整棵红黑树的根结点是什么颜色（因为向上调整可能会导致根结点颜色变为红色，也有可能根结点也颜色不变）
		//我们这里都直接重新将根结点的颜色设置为黑色，这样就不需要再在循环中去判断是否到达根结点
		return true;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		subL->_right = parent;

		Node* ppNode = parent->_parent;
		parent->_parent = subL;

		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}

			subL->_parent = ppNode;
		}
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		subR->_left = parent;
		Node* ppNode = parent->_parent;

		parent->_parent = subR;

		if (parent == _root)
		{
			_root = subR;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppNode->_right == parent)
			{
				ppNode->_right = subR;
			}
			else
			{
				ppNode->_left = subR;
			}
			subR->_parent = ppNode;
		}
	}

	//对用户暴露的中序遍历红黑树接口
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	//判断红黑树是否平衡
	bool IsBalance()
	{
		//首先根结点颜色得为黑
		if (_root->_col == RED)
		{
			return false;
		}

		int refNum = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_col == BLACK)
			{
				++refNum;
			}

			cur = cur->_left;
		}

		return Check(_root, 0, refNum);
	}

private:
	bool Check(Node* root, int blackNum, const int refNum)
	{
		if (root == nullptr)
		{
			//cout << blackNum << endl;
			if (refNum != blackNum)
			{
				cout << "存在黑色节点的数量不相等的路径" << endl;
				return false;
			}

			return true;
		}

		if (root->_col == RED && root->_parent->_col == RED)
		{
			cout << root->_kv.first << "存在连续的红色节点" << endl;
			return false;
		}

		if (root->_col == BLACK)
		{
			blackNum++;
		}

		return Check(root->_left, blackNum, refNum)
			&& Check(root->_right, blackNum, refNum);
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

private:
	Node* _root = nullptr;
};


//测试红黑树能否正常使用
void TestRBTree1()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14,8, 3, 1, 10, 6, 4, 7, 14, 13 };
	RBTree<int, int> t1;
	for (auto e : a)
	{
		if (e == 10)
		{
			int i = 0;
		}

		// 1、先看是插入谁导致出现的问题
		// 2、打条件断点，画出插入前的树
		// 3、单步跟踪，对比图一一分析细节原因
		t1.Insert({e,e});

		std::cout << "Insert:" << e << "->" << t1.IsBalance() << std::endl;
	}

	t1.InOrder();

	std::cout << t1.IsBalance() << std::endl;
}

//测试红黑树对百万数据处理所需时间
void TestRBTree2()
{
	const int N = 1000000;
	vector<int> v;
	v.reserve(N);
	srand(time(0));

	for (size_t i = 0; i < N; i++)
	{
		v.push_back(rand() + i);
		//cout << v.back() << endl;
	}

	size_t begin2 = clock();
	RBTree<int, int> t;
	for (auto e : v)
	{
		t.Insert(make_pair(e, e));
		//cout << "Insert:" << e << "->" << t.IsBalance() << endl;
	}
	size_t end2 = clock();

	std::cout << t.IsBalance() << std::endl;
}