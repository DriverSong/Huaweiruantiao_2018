#include "prediction.h"

void PredictAll(std::vector<std::vector<double> >& AlldataArray, int period, int numFla, int* predict)
{
	// std::vector<int> respredict;
	for (int n = 0; n < 15; n++)
	{
		int weekPredict = 0.0;
		//dataArray:���������ѵ������
		//LastWeek:���������ѵ�����ݵ������λ����
		std::vector<double> dataArray;
		//�����ȡ15̨����������ݵ������ڵ�һά������
		dataArray.assign(AlldataArray[n].begin(), AlldataArray[n].end());
		for (int i = 0; i < period; i++)
		{
			//����arimaģ��
			ARIMAModel* arima = new ARIMAModel(dataArray);
			//����:period
			//modelCnt�����Ԥ�������ֻȡһ��
			//cnt:��Ӧ�ļ�ʱ������
			int modelCnt = 1;
			int cnt = 0;
			std::vector<std::vector<int> > list;
			std::vector<int> tmpPredict(modelCnt);

			for (int k = 0; k < modelCnt; ++k)
			{
				//bestModel[0], bestModel[1]�ֱ���p,q�Ĳ���������ΪmodelCnt
				std::vector<int> bestModel = arima->getARIMAModel(period, list, (k == 0) ? false : true);

				if (bestModel.size() == 0)
				{
					tmpPredict[k] = (int)dataArray[dataArray.size() - period];
					cnt++;
					break;
				}
				else
				{
					//����p,q����һ��Ԥ��
					int predictDiff = arima->predictValue(bestModel[0], bestModel[1], period);
					//��Ԥ��ֵ���з���ִ���
					tmpPredict[k] = arima->aftDeal(predictDiff, period);
					cnt++;
				}
				//std::cout<<bestModel[0]<<" "<<bestModel[1]<<std::endl;
				list.push_back(bestModel);
			}

			double sumPredict = 0.0;
			//����ģ��Ԥ�������ڵľ�ֵ
			for (int k = 0; k < cnt; ++k)
			{
				sumPredict += ((double)tmpPredict[k]) / (double)cnt;
			}
			//����sumpredict����������ֵ
			int predictsingle = (int)std::round(sumPredict);
			dataArray.push_back(double(predictsingle));
			weekPredict += predictsingle;
		}
		if (weekPredict < 0)
		{
			weekPredict = 0;
		}
		//��ӡ���Ƚ�Ԥ��ֵ����ʵֵ���
		predict[n] = weekPredict;
	}
}