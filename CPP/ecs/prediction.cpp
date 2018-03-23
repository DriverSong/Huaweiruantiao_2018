#include "prediction.h"

void PredictAll(std::vector<std::vector<double> >& AlldataArray, int period, int numFla, int* predict)
{
	// std::vector<int> respredict;
	for (int n = 0; n < 15; n++)
	{
		int weekPredict = 0.0;
		//dataArray:单组虚拟机训练数据
		//LastWeek:单组虚拟机训练数据的最后七位数据
		std::vector<double> dataArray;
		//逐个读取15台虚拟机的数据到函数内的一维向量内
		dataArray.assign(AlldataArray[n].begin(), AlldataArray[n].end());
		for (int i = 0; i < period; i++)
		{
			//生成arima模型
			ARIMAModel* arima = new ARIMAModel(dataArray);
			//周期:period
			//modelCnt：多次预测参数，只取一次
			//cnt:对应的计时器变量
			int modelCnt = 1;
			int cnt = 0;
			std::vector<std::vector<int> > list;
			std::vector<int> tmpPredict(modelCnt);

			for (int k = 0; k < modelCnt; ++k)
			{
				//bestModel[0], bestModel[1]分别是p,q的参数，长度为modelCnt
				std::vector<int> bestModel = arima->getARIMAModel(period, list, (k == 0) ? false : true);

				if (bestModel.size() == 0)
				{
					tmpPredict[k] = (int)dataArray[dataArray.size() - period];
					cnt++;
					break;
				}
				else
				{
					//根据p,q进行一步预测
					int predictDiff = arima->predictValue(bestModel[0], bestModel[1], period);
					//对预测值进行反差分处理
					tmpPredict[k] = arima->aftDeal(predictDiff, period);
					cnt++;
				}
				//std::cout<<bestModel[0]<<" "<<bestModel[1]<<std::endl;
				list.push_back(bestModel);
			}

			double sumPredict = 0.0;
			//计算模型预测周期内的均值
			for (int k = 0; k < cnt; ++k)
			{
				sumPredict += ((double)tmpPredict[k]) / (double)cnt;
			}
			//返回sumpredict的四舍五入值
			int predictsingle = (int)std::round(sumPredict);
			dataArray.push_back(double(predictsingle));
			weekPredict += predictsingle;
		}
		if (weekPredict < 0)
		{
			weekPredict = 0;
		}
		//打印并比较预测值与真实值差距
		predict[n] = weekPredict;
	}
}