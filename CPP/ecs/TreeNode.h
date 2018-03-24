/*
* Brief:	树节点类头文件
* Author:	SherlockHsu
* Date:		2018-03-22
*/

#pragma once
#include<list>

struct TreeNode
{
	int _num;							// 数量
	int _value;							// 价值量
	int _layer;							// 层数
	TreeNode* _parent;					//父节点
	std::list<TreeNode*> _children;		//子节点
	TreeNode(int, int, int, TreeNode*);	//构造函数
	void SetParent(TreeNode*);			//设置父节点
	void InsertChildren(TreeNode*);		//插入子节点
};