# -*- coding:utf-8 -*-
import pandas as pd
import datetime as datetime
import numpy as np
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
    for i in range(maxdiff):
        if i == 0:
            temp_df = df
        else:
            temp_df = temp_df.diff(1)
            temp_df.dropna(inplace=True)
        p_value = adfuller(temp_df.values)[1]
        if p_value < 0.01:
            break
    return temp_df, i

def data_recover(raw_data, df_diff_roll, diff_size, roll_size):
    values = raw_data.values

    ###差分复原
    values_diff = np.append(values[-diff_size:], df_diff_roll)
    for i in range(len(df_diff_roll)):
        values_diff[i + diff_size] = values_diff[i] + values_diff[i + diff_size]

    ###移位平均复原
    values_roll = np.append(values[-roll_size + 1:], values_diff[diff_size:])
    for i in range(len(df_diff_roll)):
        values_roll[i + roll_size - 1] = values_roll[i + roll_size - 1] * roll_size - sum(values_roll[i : roll_size - 2])
    return values_roll[roll_size - 1 :]



if __name__ == '__main__':
    file_input = pd.read_csv('/home/qiujiawei/huaweiruantiao/code/data/csv/flavor15', encoding='utf-8', index_col='date')
    file_input.index = pd.to_datetime(file_input.index)
    # pd.show_versions()
    # np.show_config()
    test_data = file_input['count']


    # 数据预处理
    # ts_log = np.log(ts)
    # rol_mean = ts.rolling(window=12).mean()
    roll_size = 7
    rol_mean = test_data.rolling(window=roll_size).mean()
    # rol_mean = ts_log.rolling(window=12).mean()
    rol_mean.dropna(inplace=True)
    ts_diff, diff_size = best_diff(rol_mean)
    # ts_diff_1 = rol_mean.diff(7)
    # ts_diff_1.dropna(inplace=True)
    # ts_diff_2 = ts_diff_1.diff(1)
    # ts_diff_2.dropna(inplace=True)

    #模型预测
    start = '2015-06-01'
    end = '2015-06-07'
    # delt = datetime.timedelta(7)
    arma = myarma(ts_diff[0 : -7], 8, start, end)
    arma.find_best_model()
    predict1 = arma.properModel.predict()
    # print predict1
    # print arma.predict[1]
    predict_data = data_recover(predict1, arma.predict[0], diff_size, roll_size)
    # predict_data = data_recover(test_data, arma.predict[0], diff_size, roll_size)
    print test_data[-7 : ].values
    print predict_data
    print sum(predict_data)
    print sum([i if i > 0 else 0 for i in predict_data])

