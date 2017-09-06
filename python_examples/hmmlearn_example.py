#!/usr/bin/python
import numpy as np
from hmmlearn import hmm

def main():
    states = ["box 1", "box 2", "box3"]
    n_states = len(states)
    observations = ["red", "white"]
    n_observations = len(observations)
  
    model = hmm.MultinomialHMM(n_components=n_states)
    #hmm.GaussianHMM(n_components=3, covariance_type="full")
    model.startprob_ = np.array([0.2, 0.4, 0.4])
    model.transmat_ = np.array([
                                            [0.5, 0.2, 0.3],
                                            [0.3, 0.5, 0.2],
                                            [0.2, 0.3, 0.5]
                                            ])

    model.emissionprob_ = np.array([
                                            [0.5, 0.5],
                                            [0.4, 0.6],
                                            [0.7, 0.3]
                                            ])
    seen = np.array([[0,1,0]]).T
    logprob, box = model.decode(seen, algorithm="viterbi")
    
    print "The ball picked: {0}" . format(map(lambda x: observations[x], seen.T[0]))
    print "The hidden box: {0}" . format(map(lambda x: states[x], box))
    predict_ans = model.predict(seen)
    print "predict_ans: {0}" . format(map(lambda x: states[x], predict_ans))
    #Find obs_seq Prob: ln0.13022 = -2.03854530992
    print model.score(seen)

    print "Start training"
    model2 = hmm.MultinomialHMM(n_components=n_states, n_iter=20, tol=0.01)
    X2 = np.array([[0,1,0,1],[0,0,0,1],[1,0,1,1]])
    model2.fit(X2)
    print np.exp(model2.score(X2[0]))
    """
    model2.fit(X2)
    #
    print model2.startprob_
    print model2.transmat_
    print model2.emissionprob_
    print model2.score(X2)
    #
    model2.fit(X2)
    print model2.startprob_
    print model2.transmat_
    print model2.emissionprob_
    print model2.score(X2)
    #
    model2.fit(X2)
    print model2.startprob_
    print model2.transmat_
    print model2.emissionprob_
    print model2.score(X2)
    """
if __name__ == '__main__':
    main()
