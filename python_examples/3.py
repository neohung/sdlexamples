#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as pl
import math

import random
import collections


def metrosamp(ITER):
    P = {0: 0.2, 1: 0.8}
    Q = {1: 0, 0: 1}
    X = []
    xt = 0
    for i in range(ITER):
        xtp1 = Q[xt]
        alpha = min(1., P[xtp1] / P[xt])
        # 1 --> 0 : a = 0.25
        # 1 --> 1 : 1-0.25 = 0.75
        # 0 --> 1 : a = 1
        
        #0-1
        if random.random() <= alpha:
            xt = xtp1
        X.append(xt)
    return X


def count(X):
    counter = collections.Counter(X)
    for key in counter:
        print key, counter[key] 


def main():
	X = metrosamp(10)
	print X   
	count(X)

if __name__ == '__main__':
    main()