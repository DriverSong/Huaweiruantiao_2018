#pragma once
#include <string>

#define MAX_FLAVOR 15
#define MAX_PHY 10

/*
���������亯��
Input:	CPU�������ޣ��ڴ����ޣ�GB����flavor�����������Ż�ά�ȣ�
		��flavorCPU��������flavor�ڴ��С����flavorԤ������������
Output:	�����������Ŀ
*/
int distribution(int sumCPU, int sumMEM, int numFla, std::string target,
	int* vecFlaCPU, int* vecFlaMEM, int* vecFlaPre, int res[MAX_PHY][MAX_FLAVOR]);

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
int knapsack_limitnum2D(int *W1, int* W2, int *V, int *N, int *res, int n, int C1, int C2);

/*
�ж��Ƿ���Է���һ�����������
*/
bool isEnough(int sumCPU, int sumMEM, int numFla, int* vecFlaCPU, int* vecFlaMEM, int* vecFla);