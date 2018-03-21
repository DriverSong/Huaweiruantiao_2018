#pragma once
#include "TreeNode.h"
#include <list>

/*
一维费用的多重背包规划函数
Input:	各物品费用，各物品价值，各物品上限，规划结果，物品类别数，费用上限
Outp:	规划后总价值
*/
int knapsack_limitnum(int *W, int *V, int *N, int *res, int n, int C);

/*
二维费用的多重背包规划函数
Input:	各物品费用1，各物品费用2，各物品价值，各物品上限，规划结果，物品类别数，费用1上限，费用2上限
Outp:	规划后总价值
*/
int knapsack_limitnum2W(int *W1, int* W2, int *V, int *N, int *res, int n, int C1, int C2);

/*
二维费用二维价值的多重背包规划函数
Input:	各物品费用1，各物品费用2，各物品价值1，各物品价值2，各物品上限，规划结果，物品类别数，费用1上限，费用2上限
Outp:	规划后总价值
*/
int knapsack_limitnum2W2V(int *W1, int* W2, int *V1, int* V2, int *N, int *res, int n, int C1, int C2);

/*
构造树迭代函数
*/
void buildTree(std::list<TreeNode*>* listT, TreeNode* tn, int i, int j, int k, int* W1, int* W2, int *V1, int* V2, int *N, int*** f);