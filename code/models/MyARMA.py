# -*- coding:utf-8 -*-
import pandas as pd
import datetime as datetime
import sys
from statsmodels.tsa.arima_model import ARMA
from copy import deepcopy
from statsmodels.tsa.stattools import adfuller

class myarma:
    def __init__(self, test_data, max_index, start, end):
        self.test_data = test_data #跑模型数据
        self.max_index = int(max_index) #p、q最大值
        self.start = pd.to_datetime(start) #预测开始时间
        self.end = pd.to_datetime(end) #预测结束时间
        self.bic = sys.maxint #bic指标
        self.predict = None #预测结果
        self.properModel = None #训练模型
        self.resid = None #残差
        self.p = 0
        self.q = 0

    def find_best_model(self):
        for p in range(self.max_index):
            # print p
            for q in range(self.max_index):
                # print q
                # print p,q,self.bic
                model = ARMA(self.test_data, order=(p, q))
                try:
                    # results_ARMA = model.fit()
                    results_ARMA = model.fit(disp=-1, method='css')
                except:
                    # print 'a'
                    continue
                # results_ARMA = model.fit(disp=-1, method='css')
                bic = results_ARMA.bic
                # print bic
                # print 'bic:',bic,'self.bic:',self.bic
                if bic < self.bic:
                    self.p = p
                    self.q = q
                    self.properModel = results_ARMA
                    self.bic = bic
                    self.resid = deepcopy(self.properModel.resid)
                    self.predict = deepcopy(self.properModel.forecast((self.end - self.start).days + 1))
        print 'p : ', self.p, ' q : ',self.q


def best_diff(df, maxdiff = 8):
    temp_df = None
    p_set = {}
    for i in range(maxdiff):
        if i == 0:
            temp_df = df
        else:
            temp_df = temp_df.diff(1)
            temp_df.dropna(inplace=True)
        p_value = adfuller(temp_df.values)[1]
        if p_value < 0.01:
            break
    #print i   
    return temp_df, i

if __name__ == '__main__':
    #file_input = pd.read_csv('/home/qiujiawei/huaweiruantiao/code/data/csv/flavor1', encoding='utf-8', index_col='date')
    file_input = pd.read_csv('data/csv/flavor8_test', encoding='utf-8', index_col='date')
    file_input.index = pd.to_datetime(file_input.index)
    # pd.show_versions()
    # np.show_config()
    # print  df.index
    test_data = file_input['count']

    # 数据预处理
    # ts_log = np.log(ts)
    # rol_mean = ts.rolling(window=12).mean()
    rol_mean = test_data.rolling(window=7).mean()
    # rol_mean = ts_log.rolling(window=12).mean()
    rol_mean.dropna(inplace=True)
    # print rol_mean.values
    ts_diff, max_diff = best_diff(rol_mean)
    print max_diff
    # ts_diff_1 = rol_mean.diff(7)
    # ts_diff_1.dropna(inplace=True)
    # ts_diff_2 = ts_diff_1.diff(1)
    # ts_diff_2.dropna(inplace=True)

    #模型预测
    start = '2015-06-01'
    end = '2015-06-07'
    # delt = datetime.timedelta(7)
    arma = myarma(ts_diff, 8, start, end)
    arma.find_best_model()
    predict_week = [None]
    predict_week = arma.predict[0]
    test_data = test_data.values
    rol_mean = rol_mean.values
    week = [None]*7
    #print week[:2]
    
    week[0] = (predict_week[0]+rol_mean[-1])*7-sum(test_data[-6:]) 
    if week[0] < 0:
        week[0] = 0
    week[1] = (predict_week[1]+(sum(test_data[-6:])+sum(week[:1]))/7)*7-(sum(test_data[-5:])+sum(week[:1])) 
    if week[1] < 0:
        week[1] = 0
    week[2] = (predict_week[2]+(sum(test_data[-5:])+sum(week[:2]))/7)*7-(sum(test_data[-4:])+sum(week[:2])) 
    if week[2] < 0:
        week[2] = 0
    week[3] = (predict_week[3]+(sum(test_data[-4:])+sum(week[:3]))/7)*7-(sum(test_data[-3:])+sum(week[:3])) 
    if week[3] < 0:
        week[3] = 0
    week[4] = (predict_week[4]+(sum(test_data[-3:])+sum(week[:4]))/7)*7-(sum(test_data[-2:])+sum(week[:4])) 
    if week[4] < 0:
        week[4] = 0
    week[5] = (predict_week[5]+(sum(test_data[-2:])+sum(week[:5]))/7)*7-(sum(test_data[-1:])+sum(week[:5])) 
    if week[5] < 0:
        week[5] = 0
    week[6] = (predict_week[6]+(sum(test_data[-1:])+sum(week[:6]))/7)*7-(sum(week[:6])) 
    if week[6] < 0:
        week[6] = 0
    print sum(week)
    


