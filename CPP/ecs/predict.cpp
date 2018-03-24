#include "predict.h"
#include "distribution.h"
#include "prediction.h"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

#define MAX_PERIOD 30
#include "Date.h"

//ä½ è¦å®Œæˆçš„åŠŸèƒ½æ€»å…¥å£
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{

<<<<<<< HEAD
	std::stringstream ss;						// ×Ö·û´®Á÷
	//std::ofstream ss;							// Êä³öÎÄ¼şÁ÷
	std::string serial, flaName, time, target;	// ĞéÄâ»úID£¬ĞéÄâ»ú¹æ¸ñ£¬´´½¨Ê±¼ä£¬ÓÅ»¯×ÊÔ´Î¬¶ÈÃû
	std::string str, strResult;					// ÁÙÊ±str£¬½á¹û×Ö·û´®
	Date date, dateFirst, dateLast;				// µ±Ç°ÈÕÆÚ£¬dataÖĞ×îÔçÈÕÆÚ£¬dataÖĞ×îÍíÈÕÆÚ
	int sumCPU, sumMEM, sumHD,					// ÎïÀí·şÎñÆ÷CPUºËÊı£¬ÄÚ´æ´óĞ¡£¨GB£©£¬Ó²ÅÌ´óĞ¡£¨GB£©
		numFla, period, sumDate, indxDate,		// FlavorÊı£¬ÖÜÆÚ³¤¶È£¬×ÜÈÕÆÚÊı£¬ÈÕÆÚĞòºÅ
		vCPU, vMEM, numPHY, numFlaValid;		// CPUºËÊı£¬ÄÚ´æ´óĞ¡£¨MB£©£¬ÎïÀí·şÎñÆ÷ÊıÄ¿,ÓĞĞ§FlavorÊı

	std::map<std::string, int> mapFlaIndx;		// FlavorÃûmap
	std::string arrFlaName[MAX_FLAVOR];			// FlavorÃûÊı×é
	int arrFlaCPU[MAX_FLAVOR];					// ¸÷Flavor¶ÔÓ¦CPUºËÊıÊı×é
	int arrFlaMEM[MAX_FLAVOR];					// ¸÷Flavor¶ÔÓ¦ÄÚ´æÊı×é
	int arrFlaPre[MAX_FLAVOR];				// ¸÷FlavorÔ¤²â½á¹û
	int res[MAX_PHY][MAX_FLAVOR];				// ·ÖÅä½á¹ûÊı×é

	// ¶ÁÈ¡info
=======
	std::stringstream ss;						// å­—ç¬¦ä¸²æµ
	std::string serial, flaName, time, target;	// è™šæ‹ŸæœºIDï¼Œè™šæ‹Ÿæœºè§„æ ¼ï¼Œåˆ›å»ºæ—¶é—´ï¼Œä¼˜åŒ–èµ„æºç»´åº¦å
	std::string str, strResult;					// ä¸´æ—¶strï¼Œç»“æœå­—ç¬¦ä¸²
	Date date, dateFirst, dateLast;				// å½“å‰æ—¥æœŸï¼Œdataä¸­æœ€æ—©æ—¥æœŸï¼Œdataä¸­æœ€æ™šæ—¥æœŸ
	int sumCPU, sumMEM, sumHD,					// ç‰©ç†æœåŠ¡å™¨CPUæ ¸æ•°ï¼Œå†…å­˜å¤§å°ï¼ˆGBï¼‰ï¼Œç¡¬ç›˜å¤§å°ï¼ˆGBï¼‰
		numFla, period, sumDate, indxDate,		// Flavoræ•°ï¼Œå‘¨æœŸé•¿åº¦ï¼Œæ€»æ—¥æœŸæ•°ï¼Œæ—¥æœŸåºå·
		vCPU, vMEM, numPHY, numFlaValid;		// CPUæ ¸æ•°ï¼Œå†…å­˜å¤§å°ï¼ˆMBï¼‰ï¼Œç‰©ç†æœåŠ¡å™¨æ•°ç›®,æœ‰æ•ˆFlavoræ•°

	std::map<std::string, int> mapFlaIndx;		// Flavoråmap
	std::string arrFlaName[MAX_FLAVOR];			// Flavoråæ•°ç»„
	int arrFlaCPU[MAX_FLAVOR];					// å„Flavorå¯¹åº”CPUæ ¸æ•°æ•°ç»„
	int arrFlaMEM[MAX_FLAVOR];					// å„Flavorå¯¹åº”å†…å­˜æ•°ç»„
	int arrFlaPre[MAX_FLAVOR];					// å„Flavoré¢„æµ‹ç»“æœ
	int res[MAX_PHY][MAX_FLAVOR];				// åˆ†é…ç»“æœæ•°ç»„

	// è¯»å–info
>>>>>>> 9997cb89f8c56b437b542f2a7b19ba1a913fa56e
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

	// åˆ†ædataä¸­çš„æ—¥æœŸ
	ss << data[0];
	ss >> serial >> flaName >> dateFirst >> time;
	ss << data[data_num - 1];
	ss >> serial >> flaName >> dateLast >> time;
	ss.clear();
	sumDate = dateLast - dateFirst + 1;

	// vecDataåˆå§‹åŒ–
	std::vector<std::vector<double>> vecData(numFla, std::vector<double>(sumDate, 0));

	// ç»Ÿè®¡å„flavoræ¯å‘¨æ•°ç›®
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

	// é¢„æµ‹
	PredictAll(vecData, period, numFla, arrFlaPre);

	// è®¡ç®—æœ‰æ•ˆFlavoræ•°
	numFlaValid = 0;
	for (int i = 0; i < numFla; i++)
		numFlaValid += arrFlaPre[i];

	// åˆ†é…
	for (int i = 0; i < MAX_PHY; i++)
		for (int j = 0; j < numFla; j++)
			res[i][j] = 0;
	numPHY = distribution(sumCPU, sumMEM, numFla, target, arrFlaCPU, arrFlaMEM, arrFlaPre, res);

<<<<<<< HEAD
	// Êä³ö½á¹û
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

=======
	// ç”Ÿæˆç»“æœå­—ç¬¦ä¸²
>>>>>>> 9997cb89f8c56b437b542f2a7b19ba1a913fa56e
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

<<<<<<< HEAD
	// ĞèÒªÊä³öµÄÄÚÈİ
	char * result_file = (char *)"17\n\n0 8 0 20";
=======
	//// éœ€è¦è¾“å‡ºçš„å†…å®¹
	//char * result_file = (char *)"17\n\n0 8 0 20";
>>>>>>> 9997cb89f8c56b437b542f2a7b19ba1a913fa56e

	// ç›´æ¥è°ƒç”¨è¾“å‡ºæ–‡ä»¶çš„æ–¹æ³•è¾“å‡ºåˆ°æŒ‡å®šæ–‡ä»¶ä¸­(psè¯·æ³¨æ„æ ¼å¼çš„æ­£ç¡®æ€§ï¼Œå¦‚æœæœ‰è§£ï¼Œç¬¬ä¸€è¡Œåªæœ‰ä¸€ä¸ªæ•°æ®ï¼›ç¬¬äºŒè¡Œä¸ºç©ºï¼›ç¬¬ä¸‰è¡Œå¼€å§‹æ‰æ˜¯å…·ä½“çš„æ•°æ®ï¼Œæ•°æ®ä¹‹é—´ç”¨ä¸€ä¸ªç©ºæ ¼åˆ†éš”å¼€)
	write_result(strResult.c_str(), filename);

	return;
}
