#!/usr/bin/python
#coding =utf-8
import numpy as np
import matplotlib.pyplot as plt
import math

class SimpleHMM:
	def draw_from(self, probs):
		return np.where(np.random.multinomial(1,probs) == 1)[0][0]
		#return np.where(np.random.multinomial(1,probs) == 1)[0][0]
	
	def __init__(self):
		print "%s" % (u"HMM: init")
		states = ['Healthy', 'Fever']
		observation = ['normal', 'cold', 'dizzy']
		start_probability = [0.6, 0.4]
		transition_probability =[ [0.7, 0.3],[0.4, 0.6],]
		emission_probability = [[0.5, 0.4, 0.1],[0.1, 0.3, 0.6],]
		#print observation['Healthy']
		#print self.draw_from(self.start_probability)
		T=10
		observations = np.zeros(T, dtype=int)
		
		print self.draw_from(start_probability)
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
				temp=0
				for k in range(self.m):
					# 計算T=j-1時狀態k轉移至狀態i的機率
					temp = temp + self.x[j-1][k] * self.A[k][i]
				self.x[j][i] =  temp * self.B[i][self.o[j]]
		print u"self.x機率："
		print self.x

		result=0
		for i in range(self.m):
			#print self.x[self.t-1][i]
			result=result+self.x[self.t-1][i]
		print u"序列為0,1,0機率：%f" % (result)
		
	def backward(self):
		print "%s" % ("HMM: backward")
		self.y = np.array(np.zeros((self.t,self.m)), dtype=np.float)
		for i in range(self.m):
			self.y[self.t-1][i]=1
		j=self.t-2
		while(j>=0):
			for i in range(self.m):
				for k in range(self.m):
					self.y[j][i]+=self.A[i][k]*self.B[k][self.o[j+1]]*self.y[j+1][k]
			j=j-1

		print u"self.y機率："
		print self.y

		result=0
		for i in range(self.m):
			print self.pi[i]*self.B[i][self.o[0]]*self.y[0][i]
			result=result+self.pi[i]*self.B[i][self.o[0]]*self.y[0][i]
		print u"序列為0,1,0機率：%f" % (result)

	def viterbi(self):
		print "%s" % ("HMM: viterbi")

	#t時刻時狀態為p的機率
	def get_stateprobability(self,t,p):
		temp=self.x[t-1][p-1]*self.y[t-1][p-1]
		print "temp=%f" % temp
		total=0
		for i in range(self.m):
			total=total+self.x[t-1][i]*self.y[t-1][i]
		print "total=%f" % total
		print temp/total

def main():
	print "%s" % (u"開始")
	test = SimpleHMM()
	#test.fordward()
	#test.backward()
	#test.get_stateprobability(3,3)
	print "%s" % (u"結束")

if __name__ == '__main__':
    main()
