#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as pl
import math

#def f(x):
#    return x
#01111,01111,01111
mu,sig = 0,1

def f(x):
    return (1/(math.sqrt(2*math.pi*sig**2)))*(math.e**(-((x-mu)**2)/(2*sig**2)))


def main():
    N = 100000
    x = np.arange(N,dtype=np.float)

    x[0] = 0.2
    for i in range(0, N-1):
        # Generate 0~1 np.random.random_sample()
        #x_next = np.random.random_sample()
        #xt+1 = x_next, extract from a uniform distribution
        #f(xt+1) 
        x_next = np.random.uniform(-1,1)
        #if np.random.random_sample() < min(1, f(x_next)/f(x[i])):
        #p(xt) = f(x[i])
        #p(aj) = f(x_next)
        if np.random.uniform(-1,1) < min(1, f(x_next)/f(x[i])):
            x[i+1] = x_next
        else:
            x[i+1] = x[i]

    pl.hist(x, bins=50, color='blue')
    pl.show()

if __name__ == '__main__':
    main()