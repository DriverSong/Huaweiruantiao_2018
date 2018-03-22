#include "TreeNode.h"

TreeNode::TreeNode(int  num = 0, int value = 0, int layer = 0, TreeNode* Parent = 0)
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