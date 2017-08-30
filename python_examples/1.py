#!/usr/bin/python
#coding =utf-8
import numpy as np
import matplotlib.pyplot as plt
import math

class SimpleHMM:
	def __init__(self):
		print "%s" % ("HMM: init")
		#假設觀察有2種離散觀察值0跟1 有3種隱藏狀態 
		#轉移矩陣Ａ 3x3 , A[i][j]表示狀態i轉移至狀態j的機率
		self.A = np.array([(0.5, 0.2, 0.3), (0.3,0.5,0.2), (0.2,0.3,0.5)])
		#觀察矩陣B 3x2 , B[i][j]表示在狀態i時看到觀察值j的機率
		self.B = np.array([(0.5,0.5),(0.4,0.6),(0.7,0.3)])
		#起始矩陣pi, pi[i][j]表示從狀態i出發的機率
		self.pi = np.array([(0.2),(0.4),(0.4)])
		#T=1,2,3時間觀察到的序列
		self.o = [0,1,0]
		#Ｔ時間序列個數
		self.t=len(self.o)
		#狀態集合個數
		self.m=len(self.A)
		#離散觀察值集合個數
		self.n=len(self.B[0])

	def fordward(self):
		print "%s" % ("HMM: forward")
		#x 3x3, x[i][j]表示時間i時為狀態j的機率
		self.x = np.array(np.zeros((self.t,self.m)),dtype=np.float)
		#初始化
		for i in range(self.m):
			self.x[0][i] = self.pi[i] * self.B[i][self.o[0]] 
		"""
		[[ 0.1   0.16  0.28]
 		 [ 0.    0.    0.  ]
 		 [ 0.    0.    0.  ]]
		"""
		#從1開始算
		for j in range(1, self.t):
			for i in range(self.m):
				self.x[j][i] = self.x[j-1][i]

		print self.x
	def backward(self):
		print "%s" % ("HMM: backward")

	def viterbi(self):
		print "%s" % ("HMM: viterbi")


def main():
	print "%s" % ("開始")
	test = SimpleHMM()
	test.fordward()
	#x = np.arange(10,dtype=np.float)
    #for i in range(0, N-1):
        #x_next = np.random.uniform(-1,1)
        
    #plt.hist(x, bins=50, color='blue')
    #pl.show()
	print "%s" % ("結束")

if __name__ == '__main__':
    main()