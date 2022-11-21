#ifndef DTW_FUNCTION_H
#define DTW_FUNCTION_H
#include<vector>
#include<string>
#include<iostream>

#include <sstream>
#include <fstream>
#include <algorithm>

#include<stdlib.h>
#include<malloc.h>
using namespace std;

class DTW_Function{
private:
    vector<string> split_str(string str);
    float Distance(float * ps1,float * ps2);
    float min(float num1, float num2, float num3);
private:
    int MFCC_P;
    int MFCC_Pf;

public:
    vector<vector<string> > read_mfcc_from_csv(string filename);
    vector<vector<float> > rebuilt_mfcc_feat(vector<string> flatten_mfcc);
    int iteration_dtw(vector<vector<string> > match_csv_content, vector<vector<float> >  query_mfcc_feat, float* min_dist);
    float ComputeDTW(vector<vector<float> > cep1,vector<vector<float> > cep2);
    DTW_Function(int mfcc_num);
    ~DTW_Function();
};

#endif 
