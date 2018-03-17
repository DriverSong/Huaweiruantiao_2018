#pragma once
#include <string>

#define MAX_FLAVOR 15
#define MAX_PHY 10

/*
服务器分配函数
Input:	CPU核数上限，内存上限（GB），flavor类型数，待优化维度，
		各flavorCPU核数，各flavor内存大小，各flavor预测结果，分配结果
Output:	物理服务器数目
*/
int distribution(int sumCPU, int sumMEM, int numFla, std::string target,
	int* vecFlaCPU, int* vecFlaMEM, int* vecFlaPre, int res[MAX_PHY][MAX_FLAVOR]);

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
int knapsack_limitnum2D(int *W1, int* W2, int *V, int *N, int *res, int n, int C1, int C2);

/*
判断是否可以放入一个物理服务器
*/
bool isEnough(int sumCPU, int sumMEM, int numFla, int* vecFlaCPU, int* vecFlaMEM, int* vecFla);