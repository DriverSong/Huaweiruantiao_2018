# -*- coding:utf-8 -*-
from io import open
from collections import OrderedDict
from Variable import *
import pandas as pd
import os


def select_flavor(input_file):
    CONST = 0
    for i in range(15):
        count = OrderedDict()
        output_file = "data/changed/flavor" + str(i + 1)    #存储路径[flavor1, flavor2,...]
        print output_file
        with open(output_file, "a") as outfile:
            with open(input_file) as f:
                lines = f.readlines()
                for line in lines:
                    colums = line.split()   #colums = [id, flavor_i, y-m-d, h-m-s]
                    date = colums[2]
                    # if colums[1] == "flavor" + str(i + 1):
                    #     outfile.write(line)

                    date = date_change(date)    #去除day之前的0,如将1995-01-02变为1995-01-2

                    if date not in count.keys():
                        [year, month, day] = date.split('-')
                        for days in range(months[int(month)]):  #months[i] = 第i个月的天数
                            count[year + "-" + month + "-" + str(days + 1)] = CONST #初始化为0.00001,不取0是为了后续log操作
                        if colums[1] == "flavor" + str(i + 1):
                            count[date] = 1
                    else:
                        if colums[1] == "flavor" + str(i + 1):
                            count[date] = count[date] + 1

                save_as_csv(count, "flavor" + str(i + 1))

                for key, value in count.items():
                    # print ' '.join([key, str(value)])
                    outfile.write(' '.join([key, str(value)]) + '\n')

def date_change(date):
    #去除day之前的0,如将1995-01-02变为1995-01-2
    [year, month, day] = date.split('-')
    return year + '-' + month + '-' + str(int(day))

def save_as_csv(dictionary, flavor):
    if os.path.exists('data/csv/' + flavor):
        # print 'a'
        read = pd.read_csv('data/csv/' + flavor)
        date = dictionary.keys()
        count = dictionary.values()
        # date_column = pd.Series(date, name = 'date')
        # count_column = pd.Series(count, name = 'count')
        # combination =  pd.concat([date_column, count_column], axis = 1)
        save = pd.DataFrame({'date': date, 'count': count})
        combination = read.append(save, ignore_index = False)
        combination.to_csv('data/csv/' + flavor, index = False, encoding = 'utf-8')

    else:
        date = dictionary.keys()
        count = dictionary.values()
        # date_column = pd.Series(date, name = 'date')
        # count_column = pd.Series(count, name = 'count')
        # combination =  pd.concat([date_column, count_column], axis = 1)
        save = pd.DataFrame({'date' : date, 'count' : count})
        save.to_csv('data/csv/' + flavor, index = False, encoding = 'utf-8')

select_flavor("data/raw/data_2015_1.txt")
select_flavor("data/raw/data_2015_2.txt")
select_flavor("data/raw/data_2015_3.txt")
select_flavor("data/raw/data_2015_4.txt")
select_flavor("data/raw/data_2015_5.txt")
# select_flavor("data/raw/data_2015_12.txt")
# select_flavor("data/raw/data_2016_1.txt")