
#include <iostream>
#include <fstream>

#include "DTT_clustering.h"
using namespace std;



void DTT_clustering (Dictionary &Dic, DTT_parameters &Parameters, vector <double *> &D ){
    Temperate_Table Table(Parameters);
    unsigned seed = (unsigned) chrono::system_clock::now().time_since_epoch().count();
    shuffle (D.begin(), D.end(), default_random_engine(seed));
    for (int i=0; i<D.size(); i++) {
        process_one_point(Dic, Table, D[i]);
    }
}

void process_one_point(Dictionary &Dic, Temperate_Table &Table, double *p){
    Table.decrease();
    int k=Dic.match(p);
    if (k>=0) {
        Dic.update(k, p);
    }
    else{
        k=Table.match(p);
        Table.update(k, p, Dic);
    }
}



double similarity_measure(double *x, double *y, string opt, int dim){
    if (opt.compare("euclidean")==0) {
        double s=0;
        for (int i=0; i<dim; i++) {
            s+=(x[i]-y[i])*(x[i]-y[i]);
        }
        s=sqrt(s);
        return -s;
    }
    else if (opt.compare("cosine")==0) {
        double s=cosine_similarity(x, y, dim);
        return s;
    }
    else {
        cout<<"no such option"<<endl;
        return -1;
    }
}

double cosine_similarity(double *a, double *b, int dim){
    
    double sum1=0;
    double sum_a=0;
    double sum_b=0;
    
    for (int i=0; i<dim; i++) {
        sum1+=a[i]*b[i];
        sum_a+=a[i]*a[i];
        sum_b+=b[i]*b[i];
    }
    return sum1/(sqrt((double)sum_a)*sqrt(sum_b));
}






//-------------------------------------------Dictionary methods----------------------------------------------------
Dictionary:: Dictionary (DTT_parameters & Parameters){
    dim=Parameters.dim;
    sim_thrsh=Parameters.sim_thrsh;
}


int Dictionary::  match(double * p){
    
    
    if (centers.size()==0) {
        return -999999999;
    }
    
    double max_sim_score=-999999999999999999;
    int max_sim_indx=-999999999;
    
    // find the best match in the Dictionary
    for (int k=0; k<centers.size(); k++) {
        double similarity_score= similarity_measure(p, centers[k].value, "euclidean", dim);
        if (similarity_score>max_sim_score) {
            max_sim_score=similarity_score;
            max_sim_indx=k;
        }
    }
    if (max_sim_score>sim_thrsh) {
        return max_sim_indx;
    }
    else if (max_sim_indx<0)
        return max_sim_indx;
    else
        return -max_sim_indx-1;
    
}
void Dictionary:: update(int k, double * p){
    for (int l=0; l<dim; l++)
        centers[k].value[l]=(centers[k].value[l]*centers[k].num_member+p[l])/(centers[k].num_member+1);
    centers[k].num_member++;
}

void Dictionary:: Dic_write(string filename){
    ofstream c;
    //    centers.open("/Users/WangZiyin/Documents/DTT_centers.txt");
    c.open(filename);
    for (int i = 0 ; i<centers.size(); i++) {
        for (int j =0; j<dim; j++) {
            c<<centers[i].value[j]<<' ';
        }
        c<<endl;
    }
    c.close();
}


//-------------------------------------Temperate Table methods----------------------------------------------------
Temperate_Table:: Temperate_Table(DTT_parameters Parameters){
    dim=Parameters.dim;
    sim_thrsh=Parameters.sim_thrsh;
    int_val=Parameters.init_val;
    upper_thresh=Parameters.upper_thrsh;
}


int Temperate_Table:: match(double * p){
    int max_sim_score=-999999999;
    int max_sim_indx=-999999999;
    
    for (int k=0; k<centers.size(); k++) {
        double similarity_score = similarity_measure(p, centers[k].value, "euclidean", dim);
        if (similarity_score>max_sim_score) {
            max_sim_score=similarity_score;
            max_sim_indx=k;
        }
    }
    if (max_sim_score>sim_thrsh)
        return max_sim_indx;
    else return -1;
}

void Temperate_Table:: update(int k, double * p, Dictionary &Dic){
    if (k>=0) {
        for (int l=0; l<dim; l++) {
            centers[k].value[l]=(centers[k].value[l]*centers[k].num_member+p[l])/(centers[k].num_member+1);
        }
        centers[k].num_member++;
        centers[k].active_value+=int_val;// inrease active value by the initial value N_0

        if (centers[k].active_value>upper_thresh) {// if the active value increased higher than threshold
            
            Dic_member new_dic;
            new_dic.num_member=centers[k].num_member;
            new_dic.value= centers[k].value; // assign
            Dic.centers.push_back(new_dic);
            centers.erase(centers.begin()+k);
        }
        
    }
    else{
        Table_member a;
        a.num_member=1;
        a.value=new double[dim];
        a.value=p;
        a.active_value=int_val;
        centers.push_back(a);
    }
}

void Temperate_Table:: decrease(){
    if (centers.size()>0) {
        
        for (int i=0; i<centers.size(); i++) {
            centers.at(i).active_value-=1;
            
            if (centers.at(i).active_value<0) {
                centers.erase(centers.begin()+i);
                i--;
            }
        }
    }
}


void Dictionary::  Assign(vector<int> &A, vector<double *> &D){
    for (int i=0; i<D.size(); i++) {
        int k=match(D[i]);
        if (k<0) {
            k=abs(k)-1;
        }
        A.push_back(k);
    }
}

















