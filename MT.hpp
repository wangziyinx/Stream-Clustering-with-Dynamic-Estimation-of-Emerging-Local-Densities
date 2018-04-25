#include "mex.h"

#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <cstdio>
#include <ctime>

#include "DTT_clustering.h"

void matrix2vector(double * M,  mwSize N, mwSize dim, vector < double * > & D );
void vector2matrix(double * M, mwSize N, mwSize dim, vector <double *> &D);