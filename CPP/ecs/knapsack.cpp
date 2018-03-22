#include "knapsack.h"
#include <iostream>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define min3D(a,b,c) (((a) < (b)) ? (min(a,c)) : (min(b,c)))

static int max_value = 0;
static TreeNode* max_value_node;

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

int knapsack_limitnum2W(int *W1, int* W2, int *V, int *N, int *res, int n, int C1, int C2)
{
	int value = 0;
	// 定义价值矩阵
	int ***f = new int**[n];
	for (int i = 0; i < n; i++)
	{
		f[i] = new int*[C1 + 1];
		for (int j = 0; j < C1 + 1; j++)
			f[i][j] = new int[C2 + 1];
	}
	// 价值矩阵置零
	for (int i = 0; i < n; i++)
		for (int j = 0; j < C1 + 1; j++)
			for (int k = 0; k < C2 + 1; k++)
				f[i][j][k] = 0;
	// 价值矩阵底层初始化
	for (int i = 1; i < C1 + 1; i++)
		for (int j = 1; j < C2 + 1; j++)
		{
			int count = min3D(N[0], i / W1[0], j / W2[0]);
			f[0][i][j] = count * V[0];
		}
	// 计算价值矩阵
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
	// 计算获得最高价值的路径
	value = f[n - 1][C1][C2];
	int i = n - 1;
	int j = C1;
	int k = C2;
	while (i)
	{
		int count = min3D(N[i], j / W1[i], k / W2[i]);
		for (int c = count; c > 0; c--)
		{
			// std::cout << f[i][j][k] << " " << f[i - 1][j - W1[i] * c][k - W2[i] * c] << " " << V[i] << std::endl;
			if (f[i][j][k] == (f[i - 1][j - W1[i] * c][k - W2[i] * c] + c*V[i]))
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
		for (int j = 0; j < C1 + 1; j++) {
			delete[] f[i][j];
		}
		delete[] f[i];
	}
	delete[] f;

	return value;
}

int knapsack_limitnum2W2V(int *W1, int* W2, int *V1, int* V2, int *N, int *res, int n, int C1, int C2)
{
	int value1 = 0;
	// 定义价值矩阵
	int ***f = new int**[n];
	for (int i = 0; i < n; i++)
	{
		f[i] = new int*[C1 + 1];
		for (int j = 0; j < C1 + 1; j++)
			f[i][j] = new int[C2 + 1];
	}
	// 价值矩阵置零
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < C1 + 1; j++)
			for (int k = 0; k < C2 + 1; k++)
				f[i][j][k] = 0;
	}

	// 价值矩阵底层初始化
	for (int i = 1; i < C1 + 1; i++)
		for (int j = 1; j < C2 + 1; j++)
		{
			int count = min3D(N[0], i / W1[0], j / W2[0]);
			f[0][i][j] = count * V1[0];
		}
	// 计算价值矩阵
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
						int temp = f[i - 1][j - W1[i] * c][k - W2[i] * c] + c*V1[i];
						if (temp >= f[i][j][k])
							f[i][j][k] = temp;
					}
				}
			}
	}

	// 计算获得最高价值的路径
	value1 = f[n - 1][C1][C2];
	int i = n - 1;
	int j = C1;
	int k = C2;

	TreeNode* root = new TreeNode(0, 0, 0, nullptr);

	std::list<TreeNode*> listT;
	listT.push_back(root);
	max_value = 0;
	max_value_node = nullptr;
	buildTree(&listT, root, i, j, k, W1, W2, V1, V2, N, f);

	TreeNode* tn = max_value_node;
	while (tn->_layer != 0)
	{
		res[tn->_layer - 1] = tn->_num;
		tn = tn->_parent;
	}

	for (auto it : listT)
		delete it;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < C1 + 1; j++) {
			delete[] f[i][j];
		}
		delete[] f[i];
	}
	delete[] f;

	return value1;
}

void buildTree(std::list<TreeNode*>* listT, TreeNode * parent, int i, int j, int k, int * W1, int * W2, int *V1, int* V2, int *N, int *** f)
{
	// 判断是否到达叶子节点
	if (i > 0)
	{
		int count = min3D(N[i], j / W1[i], k / W2[i]);
		bool exist = false;	// 判断节点是否存在
		for (int c = count; c > 0; c--)
		{
			if (f[i][j][k] == (f[i - 1][j - W1[i] * c][k - W2[i] * c] + c*V1[i]))
			{
				TreeNode* tn = new TreeNode(c, parent->_value + V2[i] * c, i + 1, parent);
				listT->push_back(tn);
				parent->InsertChildren(tn);
				buildTree(listT, tn, i - 1, j - c*W1[i], k - c*W2[i], W1, W2, V1, V2, N, f);
				exist = true;
			}
		}
		if (!exist)
			buildTree(listT, parent, i - 1, j, k, W1, W2, V1, V2, N, f);
	}
	else
	{
		int value;
		TreeNode* tn;
		if (f[0][j][k] != 0)
		{
			value = parent->_value + f[0][j][k] / V1[0] * V2[0];
			tn = new TreeNode(f[0][j][k] / V1[0], value, i + 1, parent);
			listT->push_back(tn);
			parent->_children.push_back(tn);
		}
		else
		{
			tn = parent;
			value = parent->_value;
		}
		// 判断叶子节点的value是否大于当前最大值
		if (value > max_value)
		{
			max_value = value;
			max_value_node = tn;
		}
	}
	return;
}
