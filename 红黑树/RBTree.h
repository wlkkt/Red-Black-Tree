#pragma once

#include <iostream>
#include <assert.h>


// 结点颜色
enum Color 
{ 
	RED, 
	BLACK
};

// 红黑树节点的定义
template<class ValueType>
struct RBTreeNode
{
		RBTreeNode(const ValueType & data = ValueType()，Color color = RED)
			: _pLeft(nullptr), _pRight(nullptr), _pParent(nullptr)
			, _data(data), _color(color)
		{}
		RBTreeNode<ValueType>* _pLeft;		// 节点的左孩子
		RBTreeNode<ValueType>* _pRight;		// 节点的右孩子
		RBTreeNode<ValueType>* _pParent;	// 节点的双亲
		ValueType _data;				    // 节点的值域       
		Color _color;						// 节点的颜色        
};