#include "predict.h"
#include "distribution.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

#define MAX_PERIOD 30
#include "Date.h"

//��Ҫ��ɵĹ��������
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
	// ��Ҫ���������
	// char * result_file = (char *)"17\n\n0 8 0 20";

	std::stringstream ss;						// �ַ�����
	std::ofstream ofs;							// ����ļ���
	std::string serial, flaName, time, target;	// �����ID���������񣬴���ʱ�䣬�Ż���Դά����
	Date date, dateFirst, dateLast,				// ��ʱ���ڱ�����data���������ڣ�data����������
		dateBegin, dateEnd;						// �����������ڣ�һ��ѭ������������
	int sumCPU, sumMEM, sumHD,					// ���������CPU�������ڴ��С��GB����Ӳ�̴�С��GB��
		dateSpanTrain, dateSpanSum,				// ѵ��ʱ���ȣ���ʱ����
		numFla, numPeriod, indxPeriod,			// Flavor�������������������
		vCPU, vMEM, numPHY, numFlaValid;		// CPU�������ڴ��С��MB���������������Ŀ,��ЧFlavor��

	std::map<std::string, int> mapFlaIndx;		// Flavor��map
	std::string arrFlaName[MAX_FLAVOR];
	int arrFlaCPU[MAX_FLAVOR];
	int arrFlaMEM[MAX_FLAVOR];
	int arrFlaPre[MAX_FLAVOR];
	int arrData[MAX_FLAVOR][MAX_PERIOD];
	int res[MAX_PHY][MAX_FLAVOR];

	// ��ȡinfo
	ss << info[0];
	ss >> sumCPU >> sumMEM >> sumHD;
	ss << info[2];
	ss >> numFla;
	for (int i = 0; i < numFla; i++)
	{
		ss << info[i + 3];
		ss >> flaName >> vCPU >> vMEM;
		arrFlaName[i] = flaName;
		arrFlaCPU[i] = vCPU;
		arrFlaMEM[i] = vMEM / 1024;
		mapFlaIndx.insert(std::make_pair(flaName, i));
	}
	ss << info[4 + numFla];
	ss >> target;
	ss << info[6 + numFla];
	ss >> dateBegin >> time;
	ss << info[7 + numFla];
	ss >> dateEnd >> time;
	ss.clear();
	dateSpanTrain = dateEnd - dateBegin;

	// ��flavorÿ����Ŀ��ά�����ʼ��	
	for (int i = 0; i < MAX_FLAVOR; i++)
	{
		for (int j = 0; j < MAX_PERIOD; j++)
			arrData[i][j] = 0;
	}

	// ����data�е�����
	ss << data[0];
	ss >> serial >> flaName >> dateFirst >> time;
	ss << data[data_num - 1];
	ss >> serial >> flaName >> dateLast >> time;
	ss.clear();
	dateSpanSum = dateLast - dateFirst + 1;
	dateBegin = dateFirst + dateSpanSum%dateSpanTrain;
	dateEnd = dateBegin + dateSpanTrain;

	// ͳ�Ƹ�flavorÿ����Ŀ
	indxPeriod = 0;
	for (int i = 0; i < data_num; i++)
	{
		ss << data[i];
		ss >> serial >> flaName >> date >> time;
		if (date < dateBegin || mapFlaIndx.find(flaName) == mapFlaIndx.end())
			continue;
		while (date >= dateEnd)
		{
			indxPeriod++;
			dateEnd += dateSpanTrain;
		}
		arrData[mapFlaIndx[flaName]][indxPeriod]++;
	}
	ss.clear();
	numPeriod = indxPeriod + 1;

	// fstream���flavorͳ�Ʊ�
	ofs.open("vecFlavor.txt");
	for (int i = 0; i < numFla; i++)
	{
		ofs << arrFlaName[i] << ':' << '\t';
		for (int j = 0; j < numPeriod; j++)
			ofs << arrData[i][j] << '\t';
		ofs << std::endl;
	}
	ofs.close();

	// Ԥ��
	for (int i = 0; i < numFla; i++)
		arrFlaPre[i] = arrData[i][numPeriod - 1];

	// ������ЧFlavor��
	numFlaValid = 0;
	for (int i = 0; i < numFla; i++)
		if (arrFlaPre[i] != 0)
			numFlaValid++;


	// ����
	for (int i = 0; i < MAX_PHY; i++)
		for (int j = 0; j < numFla; j++)
			res[i][j] = 0;
	numPHY = distribution(sumCPU, sumMEM, numFla, target, arrFlaCPU, arrFlaMEM, arrFlaPre, res);

	// ������
	ofs.open(filename);
	ofs << numFlaValid << std::endl;
	for (int i = 0; i < numFla; i++)
		if (arrFlaPre[i] != 0)
			ofs << arrFlaName[i] << " " << arrFlaPre[i] << std::endl;
	ofs << std::endl;
	ofs << numPHY << std::endl;
	for (int i = 0; i < numPHY; i++)
	{
		ofs << i + 1;
		for (int j = 0; j < numFla; j++)
			if (res[i][j] != 0)
				ofs << " " << arrFlaName[j] << " " << res[i][j];
		ofs << std::endl;
	}


	// ֱ�ӵ�������ļ��ķ��������ָ���ļ���(ps��ע���ʽ����ȷ�ԣ�����н⣬��һ��ֻ��һ�����ݣ��ڶ���Ϊ�գ������п�ʼ���Ǿ�������ݣ�����֮����һ���ո�ָ���)
	//write_result(strResult.c_str(), filename);

	return;
}
