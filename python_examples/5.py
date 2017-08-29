#!/usr/bin/python
import numpy as np
import random
import collections
import matplotlib.pyplot as plt
import math
import scipy.stats

mu,sig = 0,1

def p_func(x):
    return (1/(math.sqrt(2*math.pi*sig**2)))*(math.e**(-((x-mu)**2)/(2*sig**2)))

#proposed distribution
def q_func(scale):
	return np.random.uniform(-1,1)*scale
    #return np.random.exponential(beta)

def metrohast(M):
    X = []
    beta = 5.
    xt = beta
    for i in range(M):
        #aj = q_func(beta)
        aj = q_func(1)
        #c = q_func_pdf(xt, beta) / q_func_pdf(aj, beta)
        c = 1
        alpha = min(1., (p_func(aj) / p_func(xt)) * c)
        if random.random() <= alpha:
            xt = aj
        X.append(xt)
    return X

def draw(S):
	n, bins, patches = plt.hist(S, 100, normed=1, facecolor='b', alpha=0.2)
	#bins = np.arange(-5, 5, 0.001)
	plt.plot(bins, [p_func(x) for x in bins], color='r')
	plt.show()

def main():
    X = metrohast(10000)
    #print X   
    draw(X)

if __name__ == '__main__':
    main()
