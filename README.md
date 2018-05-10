# Stream Clustering with Dynamic Estimation of Emerging Local Densities (ELD)

This is the implementation of paper:
Ziyin Wang, Gavriil Tsechpenakis. " Stream Clustering with Dynamic Estimation of Emerging Local Densities", ICPR 2018.

mex file has been pre-built.
try simple_test.m first.

if error arises, use matlab mex to building

mex MT.cpp DTT_clustering.cpp

then try simple_test.m


To run the algorithm after buiding:

C=MT(D', pf, theta);

D: input data. It must be double. D is a n by d matarix, the input is d by n matrix.

pf: minimun density ratio to form a sufficient kernel.

theta: kernel size (radius). Note that this value is negative. If your kernel size is 10, then theta = -10.
