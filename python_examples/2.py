#!/usr/bin/python
#coding =utf-8

import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.io import wavfile

class SimpleHMM:
	def __init__(self):
		print "%s" % (u"Init HMM")
		self.ylabel = ('Healthy', 'Fever')
		self.states = {self.ylabel[0]:0, self.ylabel[1]:1}
		self.xlabel = ('normal', 'cold', 'dizzy')
		self.observations = {self.xlabel[0]:0, self.xlabel[1]:1, self.xlabel[2]:2}
		self.N = len(self.states)
		self.M = len(self.observations)
		#Define Model
		"""
		transition_probability = [[0.7, 0.3],
		                          [0.4, 0.6]]
		emission_probability = [[0.5, 0.4, 0.1],
		                        [0.1, 0.3, 0.6]]
		start_probability = [0.6, 0.4]
		"""
		transition_probability= [[0.8125,0.1875],[0.2,0.8]]
		emission_probability = [[0.875,0.125],[0.25,0.75]]
		start_probability = [0.5,0.5]
		self.N = len(transition_probability)
		self.M = len(emission_probability[0])
		
		self.A = transition_probability
		self.B = emission_probability
		self.pi = start_probability

	def forward(self, obs_seq):
		self.T = len(obs_seq)
		#print 'forward o-> {0}' . format(obs_seq)
		#x[T][N]
		self.alpha = np.array( np.zeros((self.T,self.N)), dtype=np.float)
		#t=1:
		for i in range (0,self.N,1):
			self.alpha[0][i] = self.pi[i] * self.B[i][obs_seq[0]]

		result = np.array( np.zeros((self.T)), dtype=np.float)
		for t in range (1,self.T,1):
			#because t = 1 ~ T-1, so result start from t-1
			result[t-1] = 0
			for i in range (0,self.N,1):
				temp = 0
				for j in range (0,self.N,1):
					temp += self.alpha[t-1][j]*self.A[j][i] 
				self.alpha[t][i] = temp * self.B[i][obs_seq[t]]
				result[t-1] += self.alpha[t][i]

		logresult = 0
		for t in range (0,self.T-1,1):
			logresult += np.log(result[t])
		return logresult
		#print 'x: {0}' . format(self.alpha)
		#result = 0
		#Use t-1 to reference 
		#for i in range (0,self.N,1):
		#	result += self.alpha[self.T-1][i]
		#print "forward result: %f" % result
		#return np.log(result)

	def backward(self, obs_seq):
		self.T = len(obs_seq)
		#print 'backward: o-> {0}' . format(obs_seq)
		self.beta = np.array( np.zeros((self.T,self.N)), dtype=np.float)
		#t=T-1:		
		for i in range (0,self.N,1):
			self.beta[self.T-1][i] = 1
		t = self.T-2
		while (t >=0):
			for i in range (0,self.N,1):
				for j in range (0,self.N,1):
					self.beta[t][i] += self.beta[t+1][j] * self.A[i][j] * self.B[j][obs_seq[t+1]]	
			t-=1
		#Consider O1 and Pi
		#print 'y: {0}' . format(self.y)
		result = 0
		#Use t-1 to reference 
		for i in range (0,self.N,1):
			result += self.beta[0][i] * self.pi[i] * self.B[i][obs_seq[0]]	
		#print "backward result: %f" % result
		return result
	#給定obs_seq在時刻t看到obs_seq(t)的前提下, 預估obs_seq(t)屬於狀態s的機率
	def get_state_probability_in_time(self,obs_seq,t,s):
		self.forward(obs_seq)
		self.backward(obs_seq)
		time = t
		probability_from_forward = self.alpha[time][s]
		probability_from_backward = self.beta[time][s]
		val = probability_from_forward * probability_from_backward
		#print probability_from_forward
		#print probability_from_backward
		total = 0
		for i in range (0,self.N,1):
			total += self.alpha[t][i] * self.beta[time][i]
		return val/total

	def viterbi(self, obs_seq):
		#print "test"
		self.T = len(obs_seq)
		#print 'forward o-> {0}' . format(obs_seq)
		#x[T][N]
		self.z = np.array( np.zeros((self.T,self.N)), dtype=np.float)
		self.zpath = np.array( np.zeros((self.T,self.N)), dtype=np.float)
		#t=1:
		for i in range (0,self.N,1):
			self.z[0][i] = self.pi[i] * self.B[i][obs_seq[0]]
			self.zpath[0][i] = -1
		for t in range (1,self.T,1):
			for i in range (0,self.N,1):
				temp = 0
				maxium = self.z[t-1][0]*self.A[0][i] 
				max_state = 0
				# state1 compare with state0 first
				for j in range (1,self.N,1):
					temp = self.z[t-1][j]*self.A[j][i] 
					if (maxium < temp):
						maxium = temp
						max_state = j
						#print "(%d,%d)Found new max %f" % (t,i,maxium)
					self.z[t][i] = maxium * self.B[i][obs_seq[t]]	
					self.zpath[t][i] = max_state
		
		max_probability = self.z[self.T-1][0]
		#print "max_probability=%f" % max_probability
		last_state=[0]
		temp_state = 0
		for i in range (0,self.N,1):
			temp = self.z[self.T-1][i]
			if (max_probability < temp):
				max_probability = temp
				#print "Found new max_probability=%f" % max_probability
				last_state[0] = i
				temp_state = i
		t=self.T-1
		#self.d[t][p],t时刻状态为p的时候，t-1时刻的状态
		while(t>=1):
			last_state.append(self.zpath[t][temp_state])
			t=t-1

		#last_state = [1,2,3,4,5,6,7,8,9,10]
		#print 'last_state: {0}' . format(last_state)
		self.viterbi_state_path = ['o']
		#state_path[0]填入last_state最後一個
		self.viterbi_state_path[0]=int(last_state[len(last_state)-1])
		#扣掉last_state最後一個,倒序填入state_path
		state_path_num=len(last_state)-2
		while state_path_num>=0:
			self.viterbi_state_path.append(int(last_state[state_path_num]))
			state_path_num-=1
		#print 'state_path: {0}' . format(self.viterbi_state_path)
		#print "max_probability=%f" % max_probability
		return self.viterbi_state_path

	def computeGamma(self):
		#print "computeGamma"
		self.gamma = np.array( np.zeros((self.T,self.N)), dtype=np.float)
		for t in range (0,self.T,1):
			denominator = 0.0
			for i in range (0,self.N,1):
				self.gamma[t][i] = self.alpha[t][i] * self.beta[t][i]
				denominator +=  self.gamma[t][i]
			for i in range (0,self.N,1):
				self.gamma[t][i] /= denominator
	
	def computeXi(self, obs_seq):
		#print "computeXi"
		self.xi = np.array( np.zeros((self.T,self.N,self.N)), dtype=np.float)
		for t in range (0,self.T-1,1):
			denominator = 0.0
			for i in range (0,self.N,1):
				for j in range (0,self.N,1):
					self.xi[t][i][j] = self.alpha[t][i] * self.beta[t+1][j] * self.A[i][j] * self.B[j][obs_seq[t*1]]
					denominator +=  self.xi[t][i][j]
			for i in range (0,self.N,1):
				for j in range (0,self.N,1):
					self.xi[t][i][j] /= denominator


	def BaumWelch(self, O):
		print "BaumWelch"
		L = len(O)
		newpi = np.array( np.zeros((self.N)), dtype=np.float)
		newA = np.array( np.zeros((self.N,self.N)), dtype=np.float)
		newB = np.array( np.zeros((self.N,self.M)), dtype=np.float)
		denominator_newA = np.array( np.zeros((self.N)), dtype=np.float)
		denominator_newB = np.array( np.zeros((self.N)), dtype=np.float)
		Count = 0
		prev_logp = 0
		prev_delta = 10e-70
		ratio = 1
		while True:
			Count+=1
			# E - step, cal alpha, beta by obs_seq, A, B and pi, 
			for l in range (0,L,1):
				logp = self.forward(O[l])
				self.backward(O[l])
				self.computeGamma()
				self.computeXi(O[l])		
				for i in range (0,self.N,1):
					newpi[i] += self.gamma[0][i]
					#0~T-2 --> A, 0~T-1 --> B
					for t in range (0,self.T-1,1):
						denominator_newA[i] += self.gamma[t][i]
						denominator_newB[i] += self.gamma[t][i]
					denominator_newB[i] += self.gamma[self.T-1][i]
					#0~T-1
					for j in range (0,self.N,1):
						for t in range (0,self.T-1,1):
							newA[i][j] += self.xi[t][i][j]
					#
					for k in range (0,self.M,1):
						for t in range (0,self.T-1,1):
							if (O[l][t] == k):
								newB[i][k] += self.gamma[t][i]

			# M - step, recal A, B, pi
			for i in range (0,self.N,1):
				self.pi[i] = 0.001*(1/self.N) + 0.999*(newpi[i]/L)
				for j in range (0,self.N,1):
					self.A[i][j] =  0.001*(1/self.N) + 0.999*(newA[i][j]/denominator_newA[i])
					newA[i][j] = 0

				for k in range (0,self.M,1):
					self.B[i][k] =  0.001*(1/self.M) + 0.999*(newB[i][k]/denominator_newB[i])
					newB[i][k] = 0

				denominator_newA[i] = 0
				denominator_newB[i] = 0
				newpi[i] = 0


			#print logp
			delta = logp - prev_logp
			prev_logp = logp
			if (Count > 1):
				ratio = delta / prev_delta
				print "r=%f" % ratio
				#print logp
			prev_delta = delta


			#newA = np.array( np.zeros((self.N,self.N)), dtype=np.float)
			#newB = np.array( np.zeros((self.N,self.M)), dtype=np.float)
			#denominator_newA = np.array( np.zeros((self.N,self.N)), dtype=np.float)
			#denominator_newB = np.array( np.zeros((self.N,self.M)), dtype=np.float)
			if (Count > 3000):
				print self.pi
				print self.A
				print self.B
				
				break
			if ((ratio >= 0) and (ratio < 0.01)):
				print "Count=%d" % Count
				print self.pi
				print self.A
				print self.B
				break
		"""
		while True:
			# E - step
			for l in range (0,L,1):
				self.forward(O[0])
				self.backward(O[0])
				self.computeGamma()
				#self.computeXi(O[l])
				for i in range (0,self.N,1):
					self.pi[i] = 0
				print O[l]
			
			break
		"""
		
def main():
	print "%s" % (u"Start")
	test = SimpleHMM()
	"""
	o1 = [test.observations['dizzy'],
		 test.observations['normal'],
		 test.observations['normal'],
		 test.observations['normal'],
		 test.observations['normal'],
		 test.observations['dizzy'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['dizzy']]

	o2 = [test.observations['normal'],
		 test.observations['normal'],
		 test.observations['normal'],
		 test.observations['normal'],
		 test.observations['normal'],
		 test.observations['dizzy'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['cold'],
		 test.observations['normal']]

	O = [ o1 , o2]
	"""
	O = [[1,0,0,1,1,0,0,0,0],
        [1,1,0,1,0,0,1,1,0],
        [0,0,1,1,0,0,1,1,1]]
	#print "%s: %f" % (test.ylabel[0], test.get_state_probability_in_time(o,len(o)-1,test.states[test.ylabel[0]]))
	#print "%s: %f" % (test.ylabel[1], test.get_state_probability_in_time(o,len(o)-1,test.states[test.ylabel[1]]))
	#print "viterbi: {0}" . format(test.viterbi(o)) 
	#test.computeGamma(o)
	#test.computeXi(o)
	#test.BaumWelch(O)
	_, d = wavfile.read("audio/apple/apple01.wav")
	print d.shape[0]
	print "%s" % (u"End")

if __name__ == '__main__':
    main()
