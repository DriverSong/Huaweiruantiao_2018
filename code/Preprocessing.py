from io import open
from collections import OrderedDict
from Variable import *


def select_flavor(input_file):
    for i in range(15):
        count = OrderedDict()
        output_file = "data/changed/flavor" + str(i + 1)
        print output_file
        with open(output_file, "a") as outfile:
            with open(input_file) as f:
                lines = f.readlines()
                for line in lines:
                    colums = line.split()
                    date = colums[2]
                    # if colums[1] == "flavor" + str(i + 1):
                    #     outfile.write(line)
                    date = date_change(date)
                    if date not in count.keys():
                        [year, month, day] = date.split('-')
                        for days in range(months[int(month)]):
                            count[year + "-" + month + "-" + str(days + 1)] = 0
                        if colums[1] == "flavor" + str(i + 1):
                            count[date] = 1
                    else:
                        if colums[1] == "flavor" + str(i + 1):
                            count[date] = count[date] + 1
                for key, value in count.items():
                    # print ' '.join([key, str(value)])
                    outfile.write(' '.join([key, str(value)]) + '\n')

def date_change(date):
    [year, month, day] = date.split('-')
    return year + '-' + month + '-' + str(int(day))

select_flavor("data/raw/data_2015_1.txt")
select_flavor("data/raw/data_2015_2.txt")
select_flavor("data/raw/data_2015_3.txt")
select_flavor("data/raw/data_2015_4.txt")
select_flavor("data/raw/data_2015_5.txt")
# select_flavor("data/raw/data_2015_12.txt")
# select_flavor("data/raw/data_2016_1.txt")