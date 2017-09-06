#-*-coding:UTF-8-*-

from scipy.cluster.vq import vq, kmeans,whiten
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from features import mfcc
import scipy.cluster.hierarchy as sch

from SimpleHMM.HMM import *
from SimpleHMM.TEST import *
#from scikits.talkbox.features import mfcc

def draw(data, index, title, xlength):
	plt.figure()
	plt.plot(data[index, :], color='steelblue')
	plt.title(title)
	plt.xlim(0, xlength)
	plt.xlabel('Time (samples)')
	plt.ylabel('Amplitude (signed 16 bit)')

def main():
	print "main test"
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
	#mfcc_features = np.zeros((len(filenames),13))
	for n,file in enumerate(filenames):
		_, d = wavfile.read(file)
		print "(%d)Size: %d" % (n, d.shape[0])
		data[n, :d.shape[0]] = d
		#Rate=8000
		mfcc_features = mfcc(d, 8000)
		print mfcc_features.shape
	draw(data, 0,'Timeseries example for %s'% "Apple", 3500)
	#audio/s1/12_01.wav

	(rate,sig) = wavfile.read("audio/apple/apple10.wav")
	print rate
    #mfcc_feat = 42x13
	mfcc_feat = mfcc(sig,rate)
	draw(mfcc_feat, 0,'Timeseries example for %s'% "MFCC", 15)
	#Cal len
	#plt.savefig('plot_dendrogram.png')
	#cluster= sch.fcluster(Z, t=1, 'inconsistent')
	#datas=whiten(mfcc_feat)
	centroid=kmeans(mfcc_feat,5)[0]
	#print datas.shape
	label=vq(mfcc_feat,centroid)[0] 
	print label
	print mfcc_feat.shape
	print centroid.shape
	
	
	
	#print cluster
	#print mfcc_feat.shape
	#print disMat
	#plt.show()
	N = 3
	M = 5
	pi = np.array( np.ones((1,N))/N, dtype=np.float)
	A = np.array( np.ones((N,N))/N, dtype=np.float)
	B = np.array( np.ones((N,M))/M, dtype=np.float)
                    
	hmm = HMM(A,B,pi)
	hmm.printhmm()
	L = len(label)
	print label
	#T = len(O[0])  # 
	#alpha = np.zeros((T,hmm.N),np.float)
	#beta = np.zeros((T,hmm.N),np.float)
	#gamma = np.zeros((T,hmm.N),np.float)
	#hmm.BaumWelch(L,T,O,alpha,beta,gamma,1000)
 

if __name__ == "__main__":
    print "main"
    main()