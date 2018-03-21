#pragma once
#include "TreeNode.h"
#include <list>

/*
һά���õĶ��ر����滮����
Input:	����Ʒ���ã�����Ʒ��ֵ������Ʒ���ޣ��滮�������Ʒ���������������
Outp:	�滮���ܼ�ֵ
*/
int knapsack_limitnum(int *W, int *V, int *N, int *res, int n, int C);

/*
��ά���õĶ��ر����滮����
Input:	����Ʒ����1������Ʒ����2������Ʒ��ֵ������Ʒ���ޣ��滮�������Ʒ�����������1���ޣ�����2����
Outp:	�滮���ܼ�ֵ
*/
int knapsack_limitnum2W(int *W1, int* W2, int *V, int *N, int *res, int n, int C1, int C2);

/*
��ά���ö�ά��ֵ�Ķ��ر����滮����
Input:	����Ʒ����1������Ʒ����2������Ʒ��ֵ1������Ʒ��ֵ2������Ʒ���ޣ��滮�������Ʒ�����������1���ޣ�����2����
Outp:	�滮���ܼ�ֵ
*/
int knapsack_limitnum2W2V(int *W1, int* W2, int *V1, int* V2, int *N, int *res, int n, int C1, int C2);

/*
��������������
*/
void buildTree(std::list<TreeNode*>* listT, TreeNode* tn, int i, int j, int k, int* W1, int* W2, int *V1, int* V2, int *N, int*** f);