#include "MT.hpp"

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
/* variable declarations here */
double pf;      /* input scalar */
double th;
double *D;     /* data is store in colunm*/

mwSize dim;  
mwSize N;


/* get input */
D=mxGetPr(prhs[0]);
N=mxGetN(prhs[0]);
dim=mxGetM(prhs[0]);

pf=mxGetScalar(prhs[1]);
th=mxGetScalar(prhs[2]);



// /* code here */
//     
//     
    vector<double * > Data;
    matrix2vector(D, N, dim, Data);
  

    DTT_parameters Parameters;
    
    Parameters.belta=0.01;          //--------------setting the significance for fake false
    Parameters.Fy=2.23;            // 2.23=Fy(0.01), alfa is the statistical confidence
    
    Parameters.pf=pf;            //--------------setting parameters for minimum cluster population
    Parameters.dim=dim;
    Parameters.sim_thrsh=th;
    

     Parameters.compute_parameters();
    Dictionary Dic(Parameters);
    
    
   
    DTT_clustering (Dic, Parameters, Data); 
    
    

// /* get output */
vector <double *> CC;   
for (int i=0; i<Dic.centers.size(); i++){
    CC.push_back(Dic.centers[i].value);
//         for (int j=0; j<dim; j++)
//             cout<< CC[i][j]<<' ';
//         cout<<endl;
}

plhs[0] = mxCreateDoubleMatrix(Dic.centers.size(), dim, mxREAL);
double * outMatrix = mxGetPr(plhs[0]);
vector2matrix(outMatrix, Dic.centers.size() ,dim, CC);
}



void matrix2vector(double * M,  mwSize N, mwSize dim, vector < double * > & D ){
    mwSize i;
    for (i=0; i<N; i++){
        D.push_back(&M[i*dim]);
    }
}

void vector2matrix(double * M, mwSize N, mwSize dim, vector <double *> &D){
    mwSize i,j;
   
    for (i=0; i<dim; i++)
        for (j=0; j<N; j++)
            M[i*N+j]=D[j][i];
}




