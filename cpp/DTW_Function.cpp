#include "DTW_Function.h"

DTW_Function::DTW_Function(int mfcc_num){
    MFCC_P=mfcc_num;
    MFCC_Pf=float(mfcc_num);
}

DTW_Function::~DTW_Function(){
    return;
}

float DTW_Function::ComputeDTW(vector<vector<float> > cep1, vector<vector<float> > cep2) {
    float post_insert = 0, post_delete = 0, post_match = 0;
    float dtw = 0;

    float* s = new float[13];
    float* t = new float[13];

    float* s_match = new float[13];
    float* t_match = new float[13];

    float* s_insert = new float[13];
    float* t_insert = new float[13];

    float* s_delete = new float[13];
    float* t_delete = new float[13];

    for (int i = 1; i < cep1.size(); i++) {
        for (int j = 1; j < cep2.size(); j++) {
            for (int k = 0; k < MFCC_P; k++) {
                s[k] = cep1[i][k];
                t[k] = cep2[j][k];

                s_match[k] = cep1[(i - 1)][k];
                t_match[k] = cep2[(j - 1)][k];

                s_insert[k] = cep1[(i - 1)][k];
                t_insert[k] = cep2[j][k];

                s_delete[k] = cep1[i][k];
                t_delete[k] = cep2[(j - 1)][k];

            }
            float cost = distance(s, t);
            post_insert = distance(s_insert, t_insert);
            post_match = distance(s_match, t_match);
            post_delete = distance(s_delete, t_delete);
            dtw += (cost + min(post_insert, post_delete, post_match));
        }
    }
    delete[] s;
    delete[] t;
    delete[] s_match;
    delete[] t_match;
    delete[] s_delete;
    delete[] t_delete;
    delete[] s_insert;
    delete[] t_insert;
    //standardization
    dtw = dtw / (cep1.size() + cep2.size());
    return dtw;
}

float DTW_Function::Distance(float *ps1, float *ps2) {
    int i=0;
    float sum=0;
    for(i=0;i<MFCC_P;i++){
        sum += abs(ps1[i] - ps2[i]);
    }
    return sum;
}
float DTW_Function::min(float num1, float num2, float num3) {
    float min;
    float temp;
    temp = (num1 < num2) ? num1 : num2;
    min = (temp < num3) ? temp : num3;
    return min;
}
//MY FUNCTION

vector<string> DTW_Function::split_str(string str)
{
    string word = "";
    vector<string> temp;
    for (auto x : str)
    {
        if (x == ' ')
        {
            temp.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    return temp;
}
/*
vector<vector<float> > DTW_Function::read_mfcc_from_csv(string filename, int line_num) {  //combine read & rebuilt
    ifstream  data(filename);
    string line, csvItem;
    vector<vector<float> > temp;
    int line_num_iter = 0;
    while (getline(data, line))
    {
        line_num_iter++;
        if (line_num_iter == line_num) {
            csvItem = line;
        }
    }
    if (line_num_iter < line_num) {
        cout << "NOT ENOUGH LINE!" << endl;
    }
    else {
        temp = rebuilt_mfcc_feat(split_str(csvItem));
    }
    return temp;
}*/

vector<vector<string> >   DTW_Function::read_mfcc_from_csv(string filename){
    ifstream  data(filename);
    string line;
    string new_line;
    vector< string > readcsv;
    vector< string > line_csv_temp;
    vector<vector<string> > temp;
    temp.clear();
    //vector<vector<vector<float> > > mfcc_feat;
    while(getline(data,line))
    {
/*        stringstream lineStream(line);
        string cell;
        vector<string> parsedRow;
        while(getline(lineStream,cell,','))
        {
            parsedRow.push_back(cell);
        }
        parsedRow[3].push_back(' ');
        readcsv.push_back(parsedRow[3]);
*/        
        new_line = line + " ";
        readcsv.push_back(new_line);
        cout << new_line.length() << endl;
    }
//    readcsv.erase(readcsv.begin());


    for (int i = 0; i < readcsv.size(); i++)    //total samples number
    {
/*         cout << "before split" << endl;
        cout << readcsv[i] << endl;
 */        line_csv_temp = split_str(readcsv[i]);      //MFCC feature flatten
        /* cout << "After split:" << endl;
        for (int t = 0; t < line_csv_temp.size(); t++)
        {
            cout << line_csv_temp[i] << endl;
        } */
        
        temp.push_back(line_csv_temp);
    }
    return temp;
}


vector<vector<float> > DTW_Function::rebuilt_mfcc_feat(vector<string> flatten_mfcc) {  //turn a flatten string to a 13*frame_num vector
    vector<vector<float> > temp;
    vector<float> col_temp;
    for (int j = 0; j < (flatten_mfcc.size() / 13); j++)
    {
        for (int i = 0; i < 13; i++)
        {
            col_temp.push_back(stof(flatten_mfcc[j * 13 + i]));
        }
        temp.push_back(col_temp);
        col_temp.clear();
    }
    return temp;
}

/*int DTW_Function::iteration_dtw(string filename, int lib_len, vector<vector<float> >  query_mfcc_feat, float* min_dist) {
    vector<string> lib_str;
    vector<vector<float> >  lib_mfcc_feat;
    vector<float> dist;
    dist.reserve(lib_len);
    for (int i = 1; i < lib_len + 1; i++) {
        lib_mfcc_feat = read_mfcc_from_csv(filename, i);
        dist.push_back(ComputeDTW(lib_mfcc_feat, query_mfcc_feat));
    }

    auto    min_pos = std::min_element(dist.begin(), dist.end());
    *min_dist = *min_pos;
    return 0;
}*/

int DTW_Function::iteration_dtw(vector<vector<string> > match_csv_content, vector<vector<float> >  query_mfcc_feat, float* min_dist){
    float normal_dist;
    vector<float> eud_distance;
    vector<vector<float> >  match_mfcc_feat;
    for (int i = 0; i < match_csv_content.size(); i++)
    {
        if((match_csv_content[i].size() % 13) != 0){
            cout << "WRONG READ!" << "MFCC Length is: "<< match_csv_content[i].size() << endl;
            /* for (int j = 0; j < match_csv_content[i].size(); j++)
            {
                cout << match_csv_content[i][j] << endl;
            } */
            break;
        }
        else{
            float distance = 0;
            float normal_distance = 0;
            match_mfcc_feat = rebuilt_mfcc_feat(match_csv_content[i]);
            distance = ComputeDTW(query_mfcc_feat,match_mfcc_feat);
            normal_dist = distance / (query_mfcc_feat.size() + match_mfcc_feat.size());
            eud_distance.push_back(normal_dist);
        }
    }
    auto min_pos   = std::min_element(eud_distance.begin(), eud_distance.end());
    *min_dist = *min_pos;

    return 0;
}
