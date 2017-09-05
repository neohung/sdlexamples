#!/usr/bin/python
import numpy as np
class simple_hmm:
	def __init__(self):
		self.states = ["box 1", "box 2", "box3"]
		self.n_states = len(self.states)
		self.observations = ["red", "white"]
		self.n_observations = len(self.observations)
		#pi dim=1xN=1x3
		self.startprob_ = np.array([0.2, 0.4, 0.4])
		#A dim=NxN=3x3
		self.transmat_ = np.array([
                                            [0.5, 0.2, 0.3],
                                            [0.3, 0.5, 0.2],
                                            [0.2, 0.3, 0.5]
                                            ])
		#B dim=NxM=3x2
		self.emissionprob_ = np.array([
                                            [0.5, 0.5],
                                            [0.4, 0.6],
                                            [0.7, 0.3]
                                            ])
		#seen=1xT or Tx1?
		self.seen = np.array([[0,1,0]]).T
	def forward(self, obs_seq):
		#print "forward: osb_seq-> {0}" . format(map(lambda x: self.observations[x], obs_seq.T[0]))
		self.n_obs_seq = obs_seq.shape[0]
		self.alpha = np.array( np.zeros((self.n_obs_seq,self.n_states)), dtype=np.float)
		#compute alpha[0,:]
		for i in range(0,self.n_states,1):
			self.alpha[0,i] = self.startprob_[i] * self.emissionprob_[i,obs_seq[0]]
		#compute alpha[1~T-1,:]
		for t in range(1,self.n_obs_seq,1):
			for i in range(0,self.n_states,1):
				temp = 0
				for j in range(0,self.n_states,1):
					temp += self.alpha[t-1,j] * self.transmat_[j,i]
					self.alpha[t,i] = temp * self.emissionprob_[i,obs_seq[t]]
		#print "alpha: {0}" . format(self.alpha)
		result = 0
		for i in range(0,self.n_states,1):
			result += self.alpha[t,i]
		#Result should be 0.13022
		#ln(0.13022) = np.log(0.13022) = -2.038545
		#print "Result: %f" % np.log(result)
		#return score(obs_seq)
		return np.log(result)

	def backward(self, obs_seq):
		#print "backward: osb_seq-> {0}" . format(map(lambda x: self.observations[x], obs_seq.T[0]))
		self.n_obs_seq = obs_seq.shape[0]
		self.beta = np.array( np.zeros((self.n_obs_seq,self.n_states)), dtype=np.float)
		for i in range(0,self.n_states,1):
			self.beta[self.n_obs_seq-1,i] = 1
		t = self.n_obs_seq-1
		#t=2,1
		while(t>0):
			for i in range(0,self.n_states,1):
				for j in range(0,self.n_states,1):
					self.beta[t-1,i] += self.beta[t,j] *  self.transmat_[i,j] * self.emissionprob_[j,obs_seq[t]]
			t -= 1
		#print "beta: {0}" . format(self.beta)
		#Result should be 0.13022
		#ln(0.13022) = np.log(0.13022) = -2.038545
		result = 0
		for i in range(0,self.n_states,1):
			result += self.beta[0,i] * self.startprob_[i] * self.emissionprob_[i,obs_seq[0]]
		#print np.log(result)
		return np.log(result)

	def viterbi(self, obs_seq):
		self.n_obs_seq = obs_seq.shape[0]
		self.z = np.array( np.zeros((self.n_obs_seq,self.n_states)), dtype=np.float)
		self.zpath = np.array( np.zeros((self.n_obs_seq,self.n_states)), dtype=np.float)
		for i in range (0,self.n_states,1):
			self.zpath[0,i] = -1
			self.z[0,i] = self.startprob_[i] * self.emissionprob_[i,obs_seq[0]]
		for t in range(1,self.n_obs_seq,1):
			for i in range(0,self.n_states,1):
				maxium = self.z[t-1,0] * self.transmat_[0,i] * self.emissionprob_[i,obs_seq[t]]
				node = 0
				temp = 0
				for j in range(1,self.n_states,1):
					temp = self.z[t-1,j] * self.transmat_[j,i] * self.emissionprob_[i,obs_seq[t]]
					if (maxium < temp):
						maxium = temp
						node = j
				self.z[t,i] = maxium
				self.zpath[t,i] = node
		#print "zpath: {0}" . format(self.zpath)
		#print "z: {0}" . format(self.z)
		max_probability = self.z[self.n_obs_seq-1][0]
		max_node = 0
		temp = 0
		vpath = []
		vpro = []
		#print "max_probability=%f" % max_probability
		#find max_probability
		t = self.n_obs_seq-1
		while (t >=0):
			for i in range (0,self.n_states,1):
				temp = self.z[t][i]
				if (max_probability < temp):
					max_probability = temp
					max_node = i
			vpath.append(i)
			vpro.append(max_probability)
			t -= 1
		#print "vpath: {0}" . format(vpath)
		#print "vpro: {0}" . format(vpro)
		vpath_reverse = []
		vpro_reverse = []
		for i in range (0,len(vpath),1):
			vpath_reverse.append(vpath[len(vpath) - 1 - i])
			vpro_reverse.append(vpro[len(vpro) - 1 - i])
		#print "vpath_reverse: {0}" . format(vpath_reverse)
		#print "vpro_reverse: {0}" . format(vpro_reverse)
		return np.log(vpro_reverse), vpath_reverse
	def computeGamma(self, obs_seq):
		#
		print "computeGamma"
		self.n_obs_seq = obs_seq.shape[0]
		#self.forward(obs_seq)
		#self.backward(obs_seq)
		self.gamma = np.array( np.zeros((self.n_obs_seq,self.n_states)), dtype=np.float)
		for t in range(0,self.n_obs_seq,1):
			denominator = 0.0
			for i in range(0,self.n_states,1):
				self.gamma[t,i] = self.alpha[t,i] * self.beta[t,i] 
				denominator += self.gamma[t,i]
			for i in range(0,self.n_states,1):
				self.gamma[t,i] /= denominator
		#print "gamma: {0}" . format(self.gamma)

	def computeXi(self, obs_seq):
		print "computeXi"
		self.n_obs_seq = obs_seq.shape[0]
		self.xi = np.array( np.zeros((self.n_obs_seq-1,self.n_states,self.n_states)), dtype=np.float)
		for t in range(0,self.n_obs_seq-1,1):
			denominator = 0.0
			for i in range(0,self.n_states,1):
				for j in range(0,self.n_states,1):
					self.xi[t,i,j] = self.alpha[t,i] * self.beta[t+1][j] * self.transmat_[i][j] * self.emissionprob_[j][obs_seq[t+1]]
					denominator += self.xi[t,i,j]
			for i in range(0,self.n_states,1):
				for j in range(0,self.n_states,1):
					self.xi[t,i,j] /= denominator
		#print "xi: {0}" . format(self.xi)


	def BaumWelch(self, O):
		print "BaumWelch"
		L = len(O)
		newpi = np.array( np.zeros((self.n_states)), dtype=np.float)
		newA = np.array( np.zeros((self.n_states,self.n_states)), dtype=np.float)
		newB = np.array( np.zeros((self.n_states,self.n_observations)), dtype=np.float)
		denominator_newA = np.array( np.zeros((self.n_states)), dtype=np.float)
		denominator_newB = np.array( np.zeros((self.n_states)), dtype=np.float)
		Count = 0
		prev_logp = 0
		prev_delta = 10e-70
		ratio = 1
		while True:
			Count+=1
			if (Count > 3000):
				print "pi: {0}" . format(self.startprob_)
				print "A: {0}" . format(self.transmat_)
				print "B: {0}" . format(self.emissionprob_)				
				break

def main():
	h = simple_hmm()
	h.forward(h.seen)
	h.backward(h.seen)
	logprob, box = h.viterbi(h.seen)
	print "The ball picked: {0}" . format(map(lambda x: h.observations[x], h.seen.T[0]))
	print "The hidden box: {0}" . format(map(lambda x: h.states[x], box))
	h.computeGamma(h.seen)
	h.computeXi(h.seen)
	X = np.array([[0,1,0,1],[0,0,0,1],[1,0,1,1]])
	h.BaumWelch(X)


if __name__ == '__main__':
	main()
