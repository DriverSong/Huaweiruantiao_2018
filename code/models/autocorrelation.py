from io import open
import numpy as np
import pandas as pd
from statsmodels.tsa.stattools import acf
import matplotlib.pyplot as plt
# plt.style.use("seaborn-colorblind")

def autocorr_by_hand(x, lag):
    # Slice the relevant subseries based on the lag
    y1 = x[:(len(x)-lag)]
    y2 = x[lag:]
    # Subtract the subseries means
    sum_product = np.sum((y1-np.mean(y1))*(y2-np.mean(y2)))
    # Normalize with the subseries stds
    return sum_product / ((len(x) - lag) * np.std(y1) * np.std(y2))

def acf_by_hand(x, lag):
    # Slice the relevant subseries based on the lag
    y1 = x[:(len(x)-lag)]
    y2 = x[lag:]
    # Subtract the mean of the whole series x to calculate Cov
    sum_product = np.sum((y1-np.mean(x))*(y2-np.mean(x)))
    # Normalize with var of whole series
    return sum_product / ((len(x) - lag) * np.var(x))

def read_data(input_file):
    x = []
    with open(input_file) as f:
        lines = f.readlines()
        for line in lines:
            colums = line.split()
            x.append(int(colums[1]))
    return x


def main():
    x = read_data("/home/qiujiawei/huaweiruantiao/code/data/changed/flavor2")
    print x

    # x = np.linspace(0,100,101)
    # print x

    results = {}
    nlags = len(x)
    # nlags = 30
    results["acf_by_hand"] = [acf_by_hand(x, lag) for lag in range(nlags)]
    # results["autocorr_by_hand"] = [autocorr_by_hand(x, lag) for lag in range(nlags)]
    # results["autocorr"] = [pd.Series(x).autocorr(lag) for lag in range(nlags)]
    # results["acf"] = acf(x, unbiased=True, nlags=nlags-1)

    pd.DataFrame(results).plot(kind="bar", figsize=(10,5), grid=True)
    plt.xlabel("lag")
    plt.ylim([-1.2, 1.2])
    plt.ylabel("value")
    plt.show()

main()