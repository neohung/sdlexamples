#!/usr/bin/python
import numpy as np
import random
import collections
import matplotlib.pyplot as plt
import math
import scipy.stats
from scipy.io import wavfile
import scipy
from numpy.lib.stride_tricks import as_strided
from features import mfcc
from features import logfbank
from hmmlearn import hmm

class HMMTrainer(object):
    def __init__(self, model_name='GaussianHMM', n_components=4, cov_type='diag', n_iter=1000):
        self.model_name = model_name
        self.n_components = n_components
        self.cov_type = cov_type
        self.n_iter = n_iter
        self.models = []

        if self.model_name == 'GaussianHMM':
            self.model = hmm.GaussianHMM(n_components=self.n_components, 
                    covariance_type=self.cov_type, n_iter=self.n_iter)
        else:
            raise TypeError('Invalid model type')

    # X is a 2D numpy array where each row is 13D
    def train(self, X):
        np.seterr(all='ignore')
        self.models.append(self.model.fit(X))

    # Run the model on input data
    def get_score(self, input_data):
        return self.model.score(input_data)

def main():
    print "A"
    filenames = [ "audio/s1/00_01.wav",
                  "audio/s1/01_01.wav",
                  "audio/s1/02_01.wav",
                  "audio/s1/03_01.wav",
                  "audio/s1/04_01.wav",
                  "audio/s1/05_01.wav",
                  "audio/s1/06_01.wav",
                  "audio/s1/07_01.wav",
                  "audio/s1/08_01.wav",
                  "audio/s1/09_01.wav",
                  "audio/s1/10_01.wav",
                  "audio/s1/11_01.wav",
                  "audio/s1/12_01.wav",
                  "audio/s1/13_01.wav",
                  "audio/s1/14_01.wav",
                  "audio/s1/15_01.wav",
                  ]
    data = np.zeros((len(filenames), 32000))
    for n,file in enumerate(filenames):
        _, d = wavfile.read(file)
        print "(%d)Size: %d" % (n, d.shape[0])
        data[n, :d.shape[0]] = d
    print('Total have %d data:' % data.shape[0])
    plt.plot(data[12, :], color='steelblue')
    plt.title('Timeseries example for %s'% "Apple01")
    plt.xlim(0, 3500)
    plt.xlabel('Time (samples)')
    plt.ylabel('Amplitude (signed 16 bit)')
    plt.figure()
    plt.show()
    #rate=10000
    #sig=4096x1
    (rate,sig) = wavfile.read("audio/s1/12_01.wav")
    #mfcc_feat = 53x13
    mfcc_feat = mfcc(sig,rate)
    #fbank_feat = 53x26
    fbank_feat = logfbank(sig,rate)
    print mfcc_feat.shape
    print fbank_feat.shape
    #print sig.shape[0]
    #N = 13
    #model = hmm.GaussianHMM(n_components=N, covariance_type="full")
    #model.transmat_ = np.ones((N, N)) / N
    #model.startprob_ = np.ones(N) / N
    #fit = model.fit(mfcc_feat.T)  
    #Z2 = remodel.predict(X)
    #print fit
    hmm_models = []
    hmm_trainer = HMMTrainer()
    hmm_trainer.train(mfcc_feat)
    for i in range(10):
        score = hmm_trainer.get_score(mfcc_feat[i,:])
        print score

if __name__ == '__main__':
    main()
