# -*- coding:utf-8 -*-
import pandas as pd
import datetime as datetime
import numpy as np
import sys
from statsmodels.tsa.arima_model import ARMA
from copy import deepcopy
from statsmodels.tsa.stattools import adfuller
from collections import OrderedDict
import matplotlib.pyplot as plt

class myarma:
    def __init__(self, test_data, max_index, start, end):
        self.test_data = test_data #训练数据
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
            for q in range(self.max_index):
                model = ARMA(self.test_data, order=(p, q))
                try:
                    # results_ARMA = model.fit()
                    results_ARMA = model.fit(disp=-1, method='css')
                except:
                    continue
                # results_ARMA = model.fit(disp=-1, method='css')
                bic = results_ARMA.bic
                if bic < self.bic:
                    self.p = p
                    self.q = q
                    self.properModel = results_ARMA
                    self.bic = bic
                    self.resid = deepcopy(self.properModel.resid)
                    # self.predict = deepcopy(self.properModel.forecast((self.end - self.start).days + 1))
                    self.predict = [deepcopy(self.properModel.predict(start = self.start, end = self.end, dynamic = True))]

    def model_by_hand(self, p, q):
        self.p = p
        self.q = q
        model = ARMA(self.test_data, order=(p, q))
        results_ARMA = model.fit(disp=-1, method='css')
        self.properModel = results_ARMA
        self.bic = results_ARMA.bic
        self.resid = deepcopy(self.properModel.resid)
        # self.predict = deepcopy(self.properModel.forecast((self.end - self.start).days + 1))
        self.predict = [deepcopy(self.properModel.predict(start=self.start, end=self.end, dynamic=True))]

class my_diff:
    def __init__(self, df):
        self.df = df
        self.diff_i = []

    def best_diff(self, maxdiff = 8):
        #最大diff阶数为8
        temp_df = None
        self.diff_i = []    #self.diff_i初始化为[]
        for i in range(maxdiff):
            if i == 0:
                temp_df = self.df
                self.diff_i.append(temp_df) #添加原始数据
            else:
                temp_df = temp_df.diff(1)
                temp_df.dropna(inplace=True)
                self.diff_i.append(temp_df) #添加diff_i
            p_value = adfuller(temp_df.values)[1]
            if p_value < 0.01:
                break
        print 'diff_size: ', len(self.diff_i)

    def diff_by_hand(self ,diff_size):
        temp_df = self.df
        self.diff_i = []    #self.diff_i初始化为[]
        self.diff_i.append(temp_df) #添加原始数据
        for i in range(diff_size):
            temp_df = temp_df.diff(1)
            temp_df.dropna(inplace = True)
            self.diff_i.append(temp_df) #添加diff_i

def data_recover(df_diff_roll, raw_data, roll_size, diff_i):
    # df_diff_roll    ----    预测数据
    # raw_data         ----    训练用数据集（用于移位平均复原）
    # roll_size          ----    移位平均阶数
    # diff_i             ----    [roll_mean,diff_1,diff_2,....]
    raw_values = raw_data.values
    roll_values = diff_i[0].values #ts_diff
    diff_size = len(diff_i) - 1 #N-1次差分
    values_diff = df_diff_roll

    ###差分复原
    if diff_size > 0:
        for i in range(diff_size ,0 , -1): #总共有diff_size阶差分，需进行diff_size次差分复原
            temp_diff = diff_i[i - 1].values
            values_diff = np.append(temp_diff[-1], values_diff) #添加一位原始数据，成为[a(-1), a(0)-a(-1), a(1)-a(0),....]
            for j in range(len(values_diff) - 1):
                values_diff[j + 1] = values_diff[j + 1] + values_diff[j]
            values_diff = values_diff[1:]   #去除原始数据,成为[a(0), a(1), a(2),...]

    ###移位平均复原
    if roll_size > 1:
        values_roll = np.append(raw_values[-roll_size + 1:], values_diff)
        # print '移动平均复原前', values_roll
        for i in range(len(df_diff_roll)):
            values_roll[i + roll_size - 1] = values_roll[i + roll_size - 1] * roll_size - sum(values_roll[i : i + roll_size - 1])
    else:
        values_roll = values_diff

    return values_roll[roll_size - 1 :]

def preprocessing(test_data, roll_size, start, end, diff_flag):
    #test_data  ----    所有原始数据（需分割为测试数据与检验数据）
    #roll_size  ----    滑动平均窗口大小
    #start      ----    预测开始时间
    #end        ----    预测结束时间
    #diff_flag  ----    ==1： 自动选择最优差分阶数
    #                   ==0： 二阶差分

    predict_size = (pd.to_datetime(end) - pd.to_datetime(start)).days + 1
    raw_data = test_data[-predict_size:]
    # print raw_data
    test_data1 = test_data[0: -predict_size]

    # 数据预处理
    # 移动平均
    roll_mean = test_data1.rolling(window=roll_size).mean()
    roll_mean.dropna(inplace=True)

    # 差分
    diff = my_diff(roll_mean)
    if diff_flag == 1:
        diff.best_diff()
    else:
        diff.diff_by_hand(2)

    ts_diff = diff.diff_i #ts_diff = [roll_mean, diff_1, diff_2, ...]

    return test_data1, raw_data, ts_diff
    #test_data1 ----    训练用数据集
    #raw_data   ----    预测原始数据
    #ts_diff    ----    [roll_mean,diff_1,diff_2,....]

def predict_all():
    result_compare = OrderedDict()
    for i in range(15):
        file_input = pd.read_csv('/home/qiujiawei/huaweiruantiao/code/data/csv/flavor'+str(i+1), encoding='utf-8', index_col='date')
        file_input.index = pd.to_datetime(file_input.index)
        # pd.show_versions()
        # np.show_config()
        test_data = file_input['count']

        start = '2015-05-25'
        end = '2015-05-31'
        roll_size = 7
        diff_flag = 1
        # diff_flag     ----    ==1： 自动选择最优差分阶数
        #                       ==0： 二阶差分
        arma_flag = 1
        #arma_flag      ----    == 1 自动选择最优p、q值
        #               ----    == 0 p=0, q=7

        test_data1, raw_data, diff_i = preprocessing(test_data, roll_size, start, end, diff_flag)
        # test_data1 ----    训练用数据集
        # raw_data   ----    预测原始数据
        # diff_i    ----    [roll_mean,diff_1,diff_2,....]
        # predict_size ----      预测天数


        # 模型预测
        arma = myarma(diff_i[-1], 8, start, end)    #diff_i[-1] = diff_N
        if arma_flag == 1:
            arma.find_best_model()
        else:
            arma.model_by_hand(0, 7)


        predict_data = data_recover(arma.predict[0], test_data1 , roll_size, diff_i)
        #arma.predict[0]    ----    预测数据
        #test_data1         ----    训练用数据集（用于移位平均复原）
        #roll_size          ----    移位平均阶数
        #diff_i             ----    [roll_mean,diff_1,diff_2,....]

        # print sum(predict_data)
        result_compare[i + 1] = [sum(raw_data.values), sum([j if j > 0 else 0 for j in predict_data])]
    print result_compare
        # print sum(raw_data.values)
        # print sum([i if i > 0 else 0 for i in predict_data])

if __name__ == '__main__':
    predict_all()



