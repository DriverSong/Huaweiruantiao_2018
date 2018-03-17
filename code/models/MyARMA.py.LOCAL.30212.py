# -*- coding:utf-8 -*-
import pandas as pd
import datetime as datetime
import numpy as np
import sys
from statsmodels.tsa.arima_model import ARMA
from copy import deepcopy
from statsmodels.tsa.stattools import adfuller
import matplotlib.pyplot as plt

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
                    self.predict = deepcopy(self.properModel.forecast((self.end - self.start).days + 1))
        print 'p : ', self.p, ' q : ',self.q


def best_diff(df, maxdiff = 8):
    temp_df = None
    diff_size = 0
    for i in range(maxdiff):
        if i == 0:
            temp_df = df
            diff_size = i
        else:
            temp_df = df.diff(i)
            temp_df.dropna(inplace=True)
            diff_size = i
        print adfuller(temp_df.values)
        p_value = adfuller(temp_df.values)[1]
        print '---------'
        print p_value
        if p_value < 0.01:
            break
    # print 'diff_size: ', diff_size
    return temp_df, diff_size

def data_recover(raw_data, roll_mean, df_diff_roll, diff_size, roll_size):
    raw_values = raw_data.values
    roll_values = roll_mean.values

    ###差分复原
    if diff_size > 0:
        values_diff = np.append(roll_values[-diff_size:], df_diff_roll)
        # print 'diff复原前：', values_diff
        for i in range(len(df_diff_roll)):
            values_diff[i + diff_size] = values_diff[i] + values_diff[i + diff_size]
    else:
        values_diff = df_diff_roll

    # print 'diff复原后：', values_diff[diff_size :]

    ###移位平均复原
    if roll_size > 1:
        values_roll = np.append(raw_values[-roll_size + 1:], values_diff[diff_size:])
        # print '移动平均复原前', values_roll
        for i in range(len(df_diff_roll)):
            values_roll[i + roll_size - 1] = values_roll[i + roll_size - 1] * roll_size - sum(values_roll[i : roll_size - 2])
    else:
        values_roll = values_diff

    return values_roll[roll_size - 1 :]

def preprocessing(test_data, roll_size, start, end, diff_flag):
    predict_size = (pd.to_datetime(end) - pd.to_datetime(start)).days + 1
    raw_data = test_data[-predict_size:]
    # print raw_data
    test_data1 = test_data[0: -predict_size]
    # print '最后一个训练数据：', test_data1[-1 :]
    # 数据预处理
    # 移动平均
    roll_mean = test_data1.rolling(window=roll_size).mean()
    roll_mean.dropna(inplace=True)
    print 'roll_mean:', roll_mean.values

    # 差分
    ts_diff = roll_mean
    diff_size = 0
    if diff_flag == 1:
        ts_diff, diff_size = best_diff(roll_mean)
    print 'diff:', ts_diff.values

    return test_data1, raw_data, roll_mean, ts_diff, diff_size, predict_size

if __name__ == '__main__':
    file_input = pd.read_csv('/home/qiujiawei/huaweiruantiao/code/data/csv/flavor15', encoding='utf-8', index_col='date')
    file_input.index = pd.to_datetime(file_input.index)
    # pd.show_versions()
    # np.show_config()
    test_data = file_input['count']

    start = '2015-05-25'
    end = '2015-05-31'
    roll_size = 6
    diff_flag = 1

    test_data1, raw_data, roll_mean, ts_diff, diff_size, predict_size = preprocessing(test_data, roll_size, start, end, diff_flag)
    #模型预测
    # delt = datetime.timedelta(7)
    arma = myarma(ts_diff, 8, start, end)
    arma.find_best_model()
    print '预测复原前：', arma.predict[0]
    predict_data = data_recover(test_data1, roll_mean, arma.predict[0], diff_size, roll_size)
    # predict_data = data_recover(test_data[0 : -predict_size], roll_mean, arma.predict[0], diff_size, roll_size)
    print '移动平均复原后（预测数据）：', predict_data
    print '原始数据：', raw_data.values

    # print sum(predict_data)
    # print sum([i if i > 0 else 0 for i in predict_data])

