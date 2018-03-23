#include "predict.h"
#include "distribution.h"
#include "prediction.h"

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

	std::stringstream ss;						// 字符串流
	//std::ofstream ss;							// 输出文件流
	std::string serial, flaName, time, target;	// 虚拟机ID，虚拟机规格，创建时间，优化资源维度名
	std::string str, strResult;					// 临时str，结果字符串
	Date date, dateFirst, dateLast;				// 当前日期，data中最早日期，data中最晚日期
	int sumCPU, sumMEM, sumHD,					// 物理服务器CPU核数，内存大小（GB），硬盘大小（GB）
		numFla, period, sumDate, indxDate,		// Flavor数，周期长度，总日期数，日期序号
		vCPU, vMEM, numPHY, numFlaValid;		// CPU核数，内存大小（MB），物理服务器数目,有效Flavor数

	std::map<std::string, int> mapFlaIndx;		// Flavor名map
	std::string arrFlaName[MAX_FLAVOR];			// Flavor名数组
	int arrFlaCPU[MAX_FLAVOR];					// 各Flavor对应CPU核数数组
	int arrFlaMEM[MAX_FLAVOR];					// 各Flavor对应内存数组
	int arrFlaPre[MAX_FLAVOR];				// 各Flavor预测结果
	int res[MAX_PHY][MAX_FLAVOR];				// 分配结果数组

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
	ss >> dateFirst >> time;
	ss << info[7 + numFla];
	ss >> dateLast >> time;
	ss.clear();
	period = dateLast - dateFirst;

	// 分析data中的日期
	ss << data[0];
	ss >> serial >> flaName >> dateFirst >> time;
	ss << data[data_num - 1];
	ss >> serial >> flaName >> dateLast >> time;
	ss.clear();
	sumDate = dateLast - dateFirst + 1;

	// vecData初始化
	std::vector<std::vector<double>> vecData(numFla, std::vector<double>(sumDate, 0));

	// 统计各flavor每周数目
	indxDate = 0;
	for (int i = 0; i < data_num; i++)
	{
		ss << data[i];
		ss >> serial >> flaName >> date >> time;
		if (mapFlaIndx.find(flaName) == mapFlaIndx.end())
			continue;
		indxDate = date - dateFirst;
		vecData[mapFlaIndx[flaName]][indxDate]++;
	}
	ss.clear();

	//// fstream输出flavor统计表
	//ofs.open("vecFlavor.txt");
	//for (int i = 0; i < numFla; i++)
	//{
	//	ofs << arrFlaName[i] << ':' << '\t';
	//	for (int j = 0; j < sumDate; j++)
	//		ofs << vecData[i][j] << '\t';
	//	ofs << std::endl;
	//}
	//ofs.close();

	// 预测
	//for (int i = 0; i < numFla; i++)
	//	arrFlaPre[i] = arrData[i][numPeriod - 1];
	PredictAll(vecData, period, numFla, arrFlaPre);

	// 计算有效Flavor数
	numFlaValid = 0;
	for (int i = 0; i < numFla; i++)
		numFlaValid += arrFlaPre[i];


	// 分配
	for (int i = 0; i < MAX_PHY; i++)
		for (int j = 0; j < numFla; j++)
			res[i][j] = 0;
	numPHY = distribution(sumCPU, sumMEM, numFla, target, arrFlaCPU, arrFlaMEM, arrFlaPre, res);

	// 输出结果
	//ss << numFlaValid << std::endl;
	//for (int i = 0; i < numFla; i++)
	//	ss << arrFlaName[i] << " " << arrFlaPre[i] << std::endl;
	//ss << std::endl;
	//ss << numPHY << std::endl;
	//for (int i = 0; i < numPHY; i++)
	//{
	//	ss << i + 1;
	//	for (int j = 0; j < numFla; j++)
	//		if (res[i][j] != 0)
	//			ss << " " << arrFlaName[j] << " " << res[i][j];
	//	ss << std::endl;
	//}
	//getline(ss, str);
	//getline(ss, strResult);
	//strResult += '\n';
	//while (getline(ss, str))
	//{
	//	strResult += str;
	//	strResult += '\n';
	//}
	//strResult.pop_back();

	strResult += std::to_string(numFlaValid);
	strResult += '\n';
	for (int i = 0; i < numFla; i++)
	{
		strResult += arrFlaName[i];
		strResult += ' ';
		strResult += std::to_string(arrFlaPre[i]);
		strResult += '\n';
	}
	strResult += '\n';
	strResult += std::to_string(numPHY);
	strResult += '\n';
	for (int i = 0; i < numPHY; i++)
	{
		strResult += std::to_string(i + 1);
		for (int j = 0; j < numFla; j++)
			if (res[i][j] != 0)
			{
				strResult += ' ';
				strResult += arrFlaName[j];
				strResult += ' ';
				strResult += std::to_string(res[i][j]);
			}
		strResult += '\n';
	}

	// 需要输出的内容
	char * result_file = (char *)"17\n\n0 8 0 20";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(strResult.c_str(), filename);

	return;
}
