#pragma once
#include <string>

#define MAX_FLAVOR 15
#define MAX_PHY 127

/*
���������亯��
Input:	CPU�������ޣ��ڴ����ޣ�GB����flavor�����������Ż�ά�ȣ�
		��flavorCPU��������flavor�ڴ��С����flavorԤ������������
Output:	�����������Ŀ
*/
int distribution(int sumCPU, int sumMEM, int numFla, std::string target,
	int* vecFlaCPU, int* vecFlaMEM, int* vecFlaPre, int res[MAX_PHY][MAX_FLAVOR]);

/*
�ж��Ƿ���Է���һ�����������
*/
bool isEnough(int sumCPU, int sumMEM, int numFla, int* vecFlaCPU, int* vecFlaMEM, int* vecFla);