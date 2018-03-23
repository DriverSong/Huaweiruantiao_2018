#include <vector>
#include "cstdio"
#include <fstream>
#include "iostream"
#include "ARIMAModel.h"
#include <numeric>

std::vector<int> PredictAll(std::vector<std::vector<double> > AlldataArray,int Predays)
{
    int predict[15];
    int actual[15];
    std::vector<int> respredict;
    for(int n=0; n<15; n++)
    {
        int weekPredict = 0.0;
        //dataArray:单组虚拟机训练数据
        //LastWeek:单组虚拟机训练数据的最后七位数据
        std::vector<double> dataArray;
        std::vector<double> LastWeek;
        //逐个读取15台虚拟机的数据到函数内的一维向量内
        dataArray.assign(AlldataArray[n].begin(),AlldataArray[n].end());
        //取原始数据的最后7位作为检验，并且求和
        //LastWeek.assign(dataArray.end()-Predays,dataArray.end());
        //int SumLastWeek=accumulate(LastWeek.begin(),LastWeek.end(),0);
        //去除原始数据最后7位作为训练数据 
        //dataArray.erase(dataArray.end()-Predays,dataArray.end());
        for (int i = 0; i < Predays; i++)
        {
            //生成arima模型
            ARIMAModel* arima = new ARIMAModel(dataArray);
            //周期:period
            //modelCnt：多次预测参数，只取一次
            //cnt:对应的计时器变量
            int period = 7; 
            int modelCnt=1;
            int cnt=0; 
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
                sumPredict += ((double)tmpPredict[k])/(double)cnt;
            }
            //返回sumpredict的四舍五入值
            int predictsingle = (int)std::round(sumPredict); 
            if (predictsingle<0)
            {
                predictsingle = 0;
            }
            dataArray.push_back(double(predictsingle));
            weekPredict += predictsingle;
        }
        //打印并比较预测值与真实值差距
        //std::cout<<"week of Exact:"<<SumLastWeek<<std::endl;
        std::cout<<"week of Predict"<<n+1<<"="<<weekPredict<<std::endl;
        //std::cout<<"n:"<<n+1<<std::endl;
        //actual[n] = SumLastWeek;
        predict[n] = weekPredict;
    }
    for(int i=0;i<15;i++)
    {
        respredict.push_back(predict[i]);     
    }
    return respredict;
}
int main(){
    //打开所有data/flavor%d.txt文件读取到dataArrays二维数组中
    int actual[15];
    char str[18]="data/flavor%d.txt";
    std::vector<std::vector<double> > AlldataArray(15);
    for(int n=0; n<15; n++)
    {
        sprintf(str,"data/flavor%d.txt",n+1);
        std::string test;
        std::ifstream in(str);
        while (std::getline(in, test))
        {   
            AlldataArray[n].push_back(stod(test));
        }
    }
    //预测函数
    PredictAll(AlldataArray,7);
    //std::cout<<"week of Predict:"<<predict<<std::endl;  
}