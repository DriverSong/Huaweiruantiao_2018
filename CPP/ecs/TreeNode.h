#pragma once
#include<list>

struct TreeNode
{
	int _num;							// ����
	int _value;							// ��ֵ��
	int _layer;							// ����
	TreeNode* _parent;					//���ڵ�
	std::list<TreeNode*> _children;		//�ӽڵ�
	TreeNode(int, int, int, TreeNode*);	//���캯��
	void SetParent(TreeNode*);			//���ø��ڵ�
	void InsertChildren(TreeNode*);		//�����ӽڵ�
};