#!/usr/bin/python
#coding =utf-8

import numpy as np
import matplotlib.pyplot as plt
import math

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
		transition_probability = [[0.7, 0.3],
		                          [0.4, 0.6]]
		emission_probability = [[0.5, 0.4, 0.1],
		                        [0.1, 0.3, 0.6]]
		start_probability = [0.6, 0.4]
		self.A = transition_probability
		self.B = emission_probability
		self.pi = start_probability

	def forward(self, obs_seq):
		self.T = len(obs_seq)
		#print 'forward o-> {0}' . format(obs_seq)
		#x[T][N]
		self.x = np.array( np.zeros((self.T,self.N)), dtype=np.float)
		#t=1:
		for i in range (0,self.N,1):
			self.x[0][i] = self.pi[i] * self.B[i][obs_seq[0]]

		for t in range (1,self.T,1):
			for i in range (0,self.N,1):
				temp = 0
				for j in range (0,self.N,1):
					temp += self.x[t-1][j]*self.A[j][i] 
				self.x[t][i] = temp * self.B[i][obs_seq[t]]				
		#print 'x: {0}' . format(self.x)
		result = 0
		#Use t-1 to reference 
		for i in range (0,self.N,1):
			result += self.x[self.T-1][i]
		#print "forward result: %f" % result
		return result

	def backward(self, obs_seq):
		self.T = len(obs_seq)
		#print 'backward: o-> {0}' . format(obs_seq)
		self.y = np.array( np.zeros((self.T,self.N)), dtype=np.float)
		#t=T-1:		
		for i in range (0,self.N,1):
			self.y[self.T-1][i] = 1
		t = self.T-2
		while (t >=0):
			for i in range (0,self.N,1):
				for j in range (0,self.N,1):
					self.y[t][i] += self.y[t+1][j] * self.A[i][j] * self.B[j][obs_seq[t+1]]	
			t-=1
		#Consider O1 and Pi
		#print 'y: {0}' . format(self.y)
		result = 0
		#Use t-1 to reference 
		for i in range (0,self.N,1):
			result += self.y[0][i] * self.pi[i] * self.B[i][obs_seq[0]]	
		#print "backward result: %f" % result
		return result
	#給定obs_seq在時刻t看到obs_seq(t)的前提下, 預估obs_seq(t)屬於狀態s的機率
	def get_state_probability_in_time(self,obs_seq,t,s):
		self.forward(obs_seq)
		self.backward(obs_seq)
		time = t
		probability_from_forward = self.x[time][s]
		probability_from_backward = self.y[time][s]
		val = probability_from_forward * probability_from_backward
		#print probability_from_forward
		#print probability_from_backward
		total = 0
		for i in range (0,self.N,1):
			total += self.x[t][i] * self.y[time][i]
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
		
def main():
	print "%s" % (u"Start")
	test = SimpleHMM()
	o = [test.observations['dizzy'],
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
	#print "%s: %f" % (test.ylabel[0], test.get_state_probability_in_time(o,len(o)-1,test.states[test.ylabel[0]]))
	#print "%s: %f" % (test.ylabel[1], test.get_state_probability_in_time(o,len(o)-1,test.states[test.ylabel[1]]))
	print "viterbi: {0}" . format(test.viterbi(o)) 
	print "%s" % (u"End")

if __name__ == '__main__':
    main()
