#include "predict.h"
#include "distribution.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

#define MAX_PERIOD 30
#include "Date.h"

//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
	// 需要输出的内容
	// char * result_file = (char *)"17\n\n0 8 0 20";

	std::stringstream ss;						// 字符串流
	std::ofstream ofs;							// 输出文件流
	std::string serial, flaName, time, target;	// 虚拟机ID，虚拟机规格，创建时间，优化资源维度名
	Date date, dateFirst, dateLast,				// 临时日期变量，data中最早日期，data中最晚日期
		dateBegin, dateEnd;						// 最早有用日期，一次循环中最晚日期
	int sumCPU, sumMEM, sumHD,					// 物理服务器CPU核数，内存大小（GB），硬盘大小（GB）
		dateSpanTrain, dateSpanSum,				// 训练时间跨度，总时间跨度
		numFla, numPeriod, indxPeriod,			// Flavor数，周期数，周期序号
		vCPU, vMEM, numPHY, numFlaValid;		// CPU核数，内存大小（MB），物理服务器数目,有效Flavor数

	std::map<std::string, int> mapFlaIndx;		// Flavor名map
	std::string arrFlaName[MAX_FLAVOR];
	int arrFlaCPU[MAX_FLAVOR];
	int arrFlaMEM[MAX_FLAVOR];
	int arrFlaPre[MAX_FLAVOR];
	int arrData[MAX_FLAVOR][MAX_PERIOD];
	int res[MAX_PHY][MAX_FLAVOR];

	// 读取info
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

	// 各flavor每周数目二维数组初始化	
	for (int i = 0; i < MAX_FLAVOR; i++)
	{
		for (int j = 0; j < MAX_PERIOD; j++)
			arrData[i][j] = 0;
	}

	// 分析data中的日期
	ss << data[0];
	ss >> serial >> flaName >> dateFirst >> time;
	ss << data[data_num - 1];
	ss >> serial >> flaName >> dateLast >> time;
	ss.clear();
	dateSpanSum = dateLast - dateFirst + 1;
	dateBegin = dateFirst + dateSpanSum%dateSpanTrain;
	dateEnd = dateBegin + dateSpanTrain;

	// 统计各flavor每周数目
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

	// fstream输出flavor统计表
	ofs.open("vecFlavor.txt");
	for (int i = 0; i < numFla; i++)
	{
		ofs << arrFlaName[i] << ':' << '\t';
		for (int j = 0; j < numPeriod; j++)
			ofs << arrData[i][j] << '\t';
		ofs << std::endl;
	}
	ofs.close();

	// 预测
	for (int i = 0; i < numFla; i++)
		arrFlaPre[i] = arrData[i][numPeriod - 1];

	// 计算有效Flavor数
	numFlaValid = 0;
	for (int i = 0; i < numFla; i++)
		if (arrFlaPre[i] != 0)
			numFlaValid++;


	// 分配
	for (int i = 0; i < MAX_PHY; i++)
		for (int j = 0; j < numFla; j++)
			res[i][j] = 0;
	numPHY = distribution(sumCPU, sumMEM, numFla, target, arrFlaCPU, arrFlaMEM, arrFlaPre, res);

	// 输出结果
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


	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	//write_result(strResult.c_str(), filename);

	return;
}
