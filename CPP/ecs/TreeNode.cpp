/*
* Brief:	树节点类源文件
* Author:	SherlockHsu
* Date:		2018-03-22
*/

#include "TreeNode.h"

TreeNode::TreeNode(int  num , int value , int layer , TreeNode* Parent )
{
	_num = num;
	_value = value;
	_layer = layer;
	_parent = Parent;
}

void TreeNode::SetParent(TreeNode* node) {
	_parent = node;
}

void TreeNode::InsertChildren(TreeNode* node) {
	TreeNode* p = node;
	_children.push_back(p);
}