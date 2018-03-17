#include "distribution.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define min3D(a,b,c) (((a) < (b)) ? (min(a,c)) : (min(b,c)))

int distribution(int sumCPU, int sumMEM, int numFla, std::string target, int* vecFlaCPU, int* vecFlaMEM, int* vecFlaPre, int res[MAX_PHY][MAX_FLAVOR])
{
	// 判断value
	int *V, *W, *W_CPU, *W_MEM;
	int C, numPHY;
	int vecFla[MAX_FLAVOR];
	for (int i = 0; i < numFla; i++)
		vecFla[i] = vecFlaPre[i];
	if (target == "CPU")
		V = vecFlaCPU;
	else
		V = vecFlaMEM;
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
			knapsack_limitnum(W, V, vecFla, res[numPHY], numFla, C);
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
			knapsack_limitnum2D(W_CPU, W_MEM, V, vecFla, res[numPHY], numFla, sumCPU, sumMEM);
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

int knapsack_limitnum(int *W, int *V, int *N, int *res, int n, int C)
{
	int value = 0;
	int **f = new int*[n];
	for (int i = 0; i < n; i++)
	{
		f[i] = new int[C + 1];
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < C + 1; j++)
			f[i][j] = 0;

	for (int y = 1; y < C + 1; y++)
	{
		int count = min(N[0], y / W[0]);
		f[0][y] = (y < W[0]) ? 0 : (count * V[0]);
	}

	for (int i = 1; i < n; i++)
	{
		for (int y = 1; y < C + 1; y++)
		{
			if (y < W[i])
			{
				f[i][y] = f[i - 1][y];
			}
			else {
				int count = min(N[i], y / W[i]);
				f[i][y] = f[i - 1][y];
				for (int k = 1; k <= count; k++)
				{
					int temp = f[i - 1][y - W[i] * k] + k*V[i];
					if (temp >= f[i][y])
						f[i][y] = temp;
				}
			}
		}
	}

	//for (int i = 0; i < n; i++)
	//{
	//	for (int y = 0; y < C + 1; y++)
	//		cout << f[i][y] << " ";
	//	cout << endl;
	//}

	value = f[n - 1][C];
	int j = n - 1;
	int y = C;
	while (j)
	{
		int count = min(N[j], y / W[j]);
		for (int k = count; k > 0; k--)
		{
			if (f[j][y] == (f[j - 1][y - W[j] * k] + k*V[j]))
			{
				res[j] = k;
				y = y - k*W[j];
				break;
			}
		}
		j--;
	}
	res[0] = f[0][y] / V[0];


	for (int i = 0; i < n; i++)
	{
		delete f[i];
		f[i] = 0;
	}
	delete[] f;
	f = 0;
	return value;
}

int knapsack_limitnum2D(int *W1, int* W2, int *V, int *N, int *res, int n, int C1, int C2)
{
	int value = 0;
	int ***f = new int**[n];
	for (int i = 0; i < n; i++)
	{
		f[i] = new int*[C1 + 1];
		for (int j = 0; j < C1 + 1; j++)
			f[i][j] = new int[C2 + 1];
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < C1 + 1; j++)
			for (int k = 0; k < C2 + 1; k++)
				f[i][j][k] = 0;

	for (int i = 1; i < C1 + 1; i++)
		for (int j = 1; j < C2 + 1; j++)
		{
			int count = min3D(N[0], i / W1[0], j / W2[0]);
			f[0][i][j] = count * V[0];
		}

	for (int i = 1; i < n; i++)
	{
		for (int j = 1; j < C1 + 1; j++)
			for (int k = 1; k < C2 + 1; k++)
			{
				if (j < W1[i] || k < W2[i])
				{
					f[i][j][k] = f[i - 1][j][k];
				}
				else
				{
					int count = min3D(N[i], j / W1[i], k / W2[i]);
					f[i][j][k] = f[i - 1][j][k];
					for (int c = 1; c <= count; c++)
					{
						int temp = f[i - 1][j - W1[i] * c][k - W2[i] * c] + c*V[i];
						if (temp >= f[i][j][k])
							f[i][j][k] = temp;
					}
				}
			}
	}

	//for (int i = 0; i < n; i++)
	//{
	//	for (int y = 0; y < C + 1; y++)
	//		cout << f[i][y] << " ";
	//	cout << endl;
	//}

	value = f[n - 1][C1][C2];
	int i = n - 1;
	int j = C1;
	int k = C2;
	while (i)
	{
		int count = min3D(N[i], j / W1[i], k / W2[i]);
		for (int c = count; c > 0; c--)
		{
			if (f[i][j][k] == (f[i - 1][j - W1[i] * c][k-W2[i]] + c*V[i]))
			{
				res[i] = c;
				j = j - c*W1[i];
				k = k - c*W2[i];
				break;
			}
		}
		i--;
	}
	res[0] = f[0][j][k] / V[0];


	for (int i = 0; i < n; i++) {
		for (int j = 0; j < C1+1; j++) {
			delete[] f[i][j];
		}
		delete[] f[i];
	}
	delete[] f;

	return value;
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
	return numCPU <= sumCPU&&numMEM <= sumMEM;
}
