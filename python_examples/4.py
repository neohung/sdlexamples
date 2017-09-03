#!/usr/bin/python
import numpy as np
#import random
#import collections
import matplotlib.pyplot as plt
#import math
#import scipy.stats
from scipy.io import wavfile
import scipy
from numpy.lib.stride_tricks import as_strided

#Short Time Fourier Transform (STFT)
def stft(x, fftsize=64, overlap_pct=.5):   
    #Modified from http://stackoverflow.com/questions/2459295/stft-and-istft-in-python
    hop = int(fftsize * (1 - overlap_pct))
    w = scipy.hanning(fftsize + 1)[:-1]    
    raw = np.array([np.fft.rfft(w * x[i:i + fftsize]) for i in range(0, len(x) - fftsize, hop)])
    return raw[:, :(fftsize // 2)]

#Peak detection using the technique described here: http://kkjkok.blogspot.com/2013/12/dsp-snippets_9.html 
def peakfind(x, n_peaks, l_size=3, r_size=3, c_size=3, f=np.mean):
    win_size = l_size + r_size + c_size
    shape = x.shape[:-1] + (x.shape[-1] - win_size + 1, win_size)
    strides = x.strides + (x.strides[-1],)
    xs = as_strided(x, shape=shape, strides=strides)
    def is_peak(x):
        centered = (np.argmax(x) == l_size + int(c_size/2))
        l = x[:l_size]
        c = x[l_size:l_size + c_size]
        r = x[-r_size:]
        passes = np.max(c) > np.max([f(l), f(r)])
        if centered and passes:
            return np.max(c)
        else:
            return -1
    r = np.apply_along_axis(is_peak, 1, xs)
    top = np.argsort(r, None)[::-1]
    heights = r[top[:n_peaks]]
    #Add l_size and half - 1 of center size to get to actual peak location
    top[top > -1] = top[top > -1] + l_size + int(c_size / 2.)
    return heights, top[:n_peaks]

def main():
    filenames = [ "audio/apple/apple01.wav",
                  "audio/apple/apple02.wav",
                  "audio/apple/apple03.wav",
                  "audio/apple/apple04.wav",
                  "audio/apple/apple05.wav",
                  "audio/apple/apple06.wav",
                  "audio/apple/apple07.wav",
                  "audio/apple/apple08.wav",
                  "audio/apple/apple09.wav",
                  "audio/apple/apple10.wav",
                  ]
    data = np.zeros((len(filenames), 32000))
    for n,file in enumerate(filenames):
        _, d = wavfile.read(file)
        print "(%d)Size: %d" % (n, d.shape[0])
        data[n, :d.shape[0]] = d
    print('Total have %d data:' % data.shape[0])
    plt.plot(data[0, :], color='steelblue')
    plt.title('Timeseries example for %s'% "Apple01")
    plt.xlim(0, 3500)
    plt.xlabel('Time (samples)')
    plt.ylabel('Amplitude (signed 16 bit)')
    plt.figure()
    log_freq = 20 * np.log(np.abs(stft(data[0, :])) + 1)
    #print(log_freq.shape)
    plt.imshow(log_freq, cmap='gray', interpolation=None)
    plt.xlabel('Freq (bin)')
    plt.ylabel('Time (overlapped frames)')
    plt.ylim(log_freq.shape[1])
    plt.title('PSD of %s example'% "Apple01")
    plt.figure()
    #Plot dim [1x32] in index=15
    index = 2
    plot_data = np.abs(stft(data[0, :]))[index, :]
    plt.plot(plot_data, color='steelblue')
    #plt.plot(fp, fv, 'x', color='darkred')
    plt.title('Peak location example')
    plt.xlabel('Frequency (bins)')
    plt.ylabel('Amplitude')

    values, locs = peakfind(plot_data, n_peaks=6)
    fp = locs[values > -1]
    fv = values[values > -1]
    print fv
    print fp
    plt.plot(fp, fv, 'x', color='darkred')
    #print plot_data.shape
    obs = np.zeros((6, data.shape[0]))
    for r in range(data.shape[0]):
        _, t = peakfind(data[r, :], n_peaks=6)
        obs[:, r] = t.copy()
    print obs
    #plt.plot(obs[0,index], fv, 'x', color='darkred')
    plt.show()

if __name__ == '__main__':
    main()
