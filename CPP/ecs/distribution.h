#pragma once
#include <string>

#define MAX_FLAVOR 15
#define MAX_PHY 127

/*
服务器分配函数
Input:	CPU核数上限，内存上限（GB），flavor类型数，待优化维度，
		各flavorCPU核数，各flavor内存大小，各flavor预测结果，分配结果
Output:	物理服务器数目
*/
int distribution(int sumCPU, int sumMEM, int numFla, std::string target,
	int* vecFlaCPU, int* vecFlaMEM, int* vecFlaPre, int res[MAX_PHY][MAX_FLAVOR]);

/*
判断是否可以放入一个物理服务器
*/
bool isEnough(int sumCPU, int sumMEM, int numFla, int* vecFlaCPU, int* vecFlaMEM, int* vecFla);