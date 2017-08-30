#!/usr/bin/python
import numpy as np
import math
from numpy import linalg as la
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import time
import os

mu,sig,N = 0,1,10000
pts = []

def q(x):
    return (1/(math.sqrt(2*math.pi*sig**2)))*(math.e**(-((x-mu)**2)/(2*sig**2)))

def metropolis(N):
    r = np.zeros(1)
    p = q(r[0])
    pts = []
    
    for i in range(N):
        rn = r + np.random.uniform(-1,1)
        pn = q(rn[0])
        if pn >= p:
            p = pn
            r = rn
        else:
            u = np.random.rand()
            if u < pn/p:
                p = pn
                r = rn
        pts.append(r)
    
    pts = np.array(pts)
    return pts
    
def hist_plot(array):
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1,)
    ax.hist(array, bins=1000)    
    plt.title('')
    plt.xlabel('')
    plt.ylabel('')
    plt.grid(True)
    plt.savefig("test.png")
    plt.show()
   
def main():
    print "test [%s] %d" % ('this is 2' , len('test'))
    #N = 100000
    pts = metropolis(N)
    hist_plot(pts)
    programPause = raw_input("Press the <ENTER> key to continue...")


if __name__ == '__main__':
    main()
