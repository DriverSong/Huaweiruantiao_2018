#include "distribution.h"
#include "knapsack.h"

#include <iostream>

int distribution(int sumCPU, int sumMEM, int numFla, std::string target, int* vecFlaCPU, int* vecFlaMEM, int* vecFlaPre, int res[MAX_PHY][MAX_FLAVOR])
{
	// 判断value
	int *V1, *V2, *W, *W_CPU, *W_MEM;
	int C, numPHY;
	int vecFla[MAX_FLAVOR];
	for (int i = 0; i < numFla; i++)
		vecFla[i] = vecFlaPre[i];
	if (target == "CPU")
	{
		V1 = vecFlaCPU;
		V2 = vecFlaMEM;
	}
	else
	{
		V1 = vecFlaMEM;
		V2 = vecFlaCPU;
	}
	// 根据物理服务器资源选择背包算法
	numPHY = 0;
	// 一维费用的多重背包
	if (sumCPU * 4 <= sumMEM || sumCPU >= sumMEM)
	{
		if (sumCPU * 4 <= sumMEM)
		{
			W = vecFlaCPU;
			C = sumCPU;
		}
		else
		{
			W = vecFlaMEM;
			C = sumMEM;
		}
		while (!isEnough(sumCPU, sumMEM, numFla, vecFlaCPU, vecFlaMEM, vecFla))
		{
			knapsack_limitnum(W, V1, vecFla, res[numPHY], numFla, C);
			for (int i = 0; i < numFla; i++)
				vecFla[i] -= res[numPHY][i];
			numPHY++;
		}
		for (int i = 0; i < numFla; i++)
			res[numPHY][i] = vecFla[i];

	}
	// 二维费用的多重背包
	else
	{
		W_CPU = vecFlaCPU;
		W_MEM = vecFlaMEM;
		while (!isEnough(sumCPU, sumMEM, numFla, vecFlaCPU, vecFlaMEM, vecFla))
		{
			knapsack_limitnum2W2V(W_CPU, W_MEM, V1, V2, vecFla, res[numPHY], numFla, sumCPU, sumMEM);
			//knapsack_limitnum2W(W_CPU, W_MEM, V1, vecFla, res[numPHY], numFla, sumCPU, sumMEM);
			for (int i = 0; i < numFla; i++)
				vecFla[i] -= res[numPHY][i];
			numPHY++;
		}
		for (int i = 0; i < numFla; i++)
			res[numPHY][i] = vecFla[i];

	}
	numPHY++;
	return numPHY;
}

bool isEnough(int sumCPU, int sumMEM, int numFla, int * vecFlaCPU, int * vecFlaMEM, int * vecFla)
{
	int numCPU, numMEM;
	numCPU = 0;
	numMEM = 0;
	for (int i = 0; i < numFla; i++)
	{
		numCPU += vecFla[i] * vecFlaCPU[i];
		numMEM += vecFla[i] * vecFlaMEM[i];
	}
	// std::cout << "Test:\t" << numCPU << "\t" << numMEM << std::endl;
	return numCPU <= sumCPU&&numMEM <= sumMEM;
}
