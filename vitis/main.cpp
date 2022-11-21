#include<vector>
#include<string>
#include<iostream>

#include <sstream>
#include <fstream>
#include <algorithm>

#include<stdlib.h>
#include<malloc.h>

////

#include "platform.h"
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xil_io.h"
#include "xsdps.h"
#include <random>

using namespace std;

#define ROW_MAX 2250
#define LINE_LEN 150000
#define MFCC_P 13
#define TEST_TIMES 1

void ReadCsvFile(const char* filename, vector<vector<string> >* str_features);
void split_str(string str, vector<string>* split_str);
void rebuilt_mfcc_feat(vector<string> flatten_mfcc, vector<vector<float> >* mfcc_float);
void ComputeDTW(vector<vector<float> > cep1, vector<vector<float> > cep2, float* dtw);
void distance(float *ps1, float *ps2, float* sum);
void min(float num1, float num2, float num3, float* min_value);
void iteration_dtw(vector<vector<string> > match_csv_content, vector<vector<float> >  query_mfcc_feat, float* min_dist);

int main(int argc, char *argv[])
{
	static FATFS FS_instance;
    init_platform();
    printf("Mounting SD Card\n\r");
	FRESULT result = f_mount(&FS_instance,"0:", 1);
    if (result != 0) {
	printf("Couldn't mount SD Card. Try again later.\n");
	}
/*
    char cfilename[] = "0.csv";
    vector<vector<string> > lib_features;
    ReadCsvFile(cfilename, &lib_features);
    printf("\nlib_feature shape:%d\n", lib_features.size());
    vector<vector<float> > mfcc_feat1;
    rebuilt_mfcc_feat(lib_features[0], &mfcc_feat1);
    printf("mfcc_feat1 size:%d x %d\n", mfcc_feat1.size(), mfcc_feat1[0].size());
    vector<vector<float> > mfcc_feat2;
    rebuilt_mfcc_feat(lib_features[1], &mfcc_feat2);
    printf("mfcc_feat2 size:%d x %d\n", mfcc_feat2.size(), mfcc_feat2[0].size());
    float dtw_dist;
    ComputeDTW(mfcc_feat1, mfcc_feat2, &dtw_dist);
    printf("DTW Dist is:%f", dtw_dist);
    printf("OUT END!\n");
*/
    vector<vector<string> > match_csv_content;
    vector<vector<float> >  match_mfcc_feat;
    vector<vector<string> > query_csv_content;
    vector<vector<float> >  query_mfcc_feat;

    //Dynamic Time Warping to Recognize
        vector<float> total_mindist;
        float minimum_dist;
        float corr_rate;
        int   minimum_index;
        int query_rand_num;
        int correct_num ,wrong_num;
        correct_num = 0;
        wrong_num = 0;

        for (int t = 0; t < TEST_TIMES; t++)
        {

            query_rand_num = rand() % 10;
            //    query_rand_num = 0;
        //    cout << "Now test : " << query_rand_num << endl;
//            string query_file = "q" + to_string(query_rand_num) + ".csv";
//            printf("qf_len:%d\n", query_file.length());
           char q_num;
           itoa(query_rand_num, &q_num, 10);
           char query_path[7] = {'q', q_num, '.', 'c','s','v','\0'};
//           for(int i = 0; i<query_file.length(); i++){
//        	   query_path[i] = query_file[i];
//           }

           	printf("get query feats\n");
            ReadCsvFile(query_path, &query_csv_content);

            total_mindist.clear();
            for (int j = 0; j < 10; j++)
                {
            	printf("Match Num %d start.\n", j);
            	string match_file = to_string(j) + ".csv";
            	printf("mf_len:%d\n", match_file.length());
            	char match_path[6];
            	for(int i = 0; i<match_file.length(); i++){
            		match_path[i] = match_file[i];
            	}
            	match_path[5] = '\0';
                ReadCsvFile(match_path, &match_csv_content);
                int random_gen_seed = rand() % query_csv_content.size();
                rebuilt_mfcc_feat(query_csv_content[random_gen_seed], &query_mfcc_feat);
                printf("Computing DTW...Please wait.\n");
                iteration_dtw(match_csv_content, query_mfcc_feat, &minimum_dist);
                printf("End Computing.\n");
                total_mindist.push_back(minimum_dist);
                random_gen_seed = 0;
                printf("Match Num %d end.\n", j);
            }

            minimum_index = std::min_element(total_mindist.begin(), total_mindist.end()) - total_mindist.begin();
            printf("Recognition Number: %d", minimum_index);
            if(minimum_index == query_rand_num){
                correct_num += 1;
            }
            else{
                wrong_num += 1;
            }
            query_rand_num = 0;
        }
        corr_rate = float(correct_num) / float((correct_num + wrong_num));
        printf("correct times:%d\n", correct_num);
        printf("wrong times:%d\n", wrong_num);
        printf("Recognition Correct rate: %f.\n", corr_rate);

/*    FIL test_data_inputs_pointer;
    char cfilename[] = "0.csv";
    FRESULT open_result = f_open(&test_data_inputs_pointer, cfilename, FA_OPEN_ALWAYS | FA_READ);
    if(open_result != 0){
    	printf("Open CSV Error!\n");
    }
    else{
    	printf("Open CSV success!\n");
    	char* line_buffer = NULL;
    	printf("Begin read line!\n");
    	while(!f_eof(&test_data_inputs_pointer)){
    		f_gets(line_buffer, LINE_LEN, &test_data_inputs_pointer);
    		printf("Line content:%s\n", line_buffer);
    	}
    f_close(&test_data_inputs_pointer);*/
    cleanup_platform();
    return 0;
}

void split_str(string str, vector<string>* split_str)
{
    string word = "";
//    vector<string> temp;
    for (auto x : str)
    {
        if (x == ' ')
        {
            //temp.push_back(word);
        	split_str->push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    //return temp;
//    split_str = temp;
    return;
}

void ReadCsvFile(const char* filename, vector<vector<string> >* str_features){
    string temp;
    vector<string> v_temp;
//    vector<vector<string> > vv_temp;
    FIL test_data_inputs_pointer;
//        char cfilename[] = "0.csv";
    FRESULT open_result = f_open(&test_data_inputs_pointer, filename, FA_OPEN_ALWAYS | FA_READ);
    if(open_result != 0){
    	printf("Open CSV Error!\n");
    }
    else{
    printf("Open CSV file (\" %s \") success!\n", filename);
    char* line_buffer = NULL;
    printf("Begin read line!\n");
    while(!f_eof(&test_data_inputs_pointer)){
        f_gets(line_buffer, LINE_LEN, &test_data_inputs_pointer);
        temp = line_buffer;
        temp = temp + " ";
//        printf("read_line_len:%d.", temp.length());
        if(temp.length()!=0){
//        	printf("Begin split!");
        	split_str(temp, &v_temp);
        	if(v_temp.size()!=0){
//        		printf("%d,", v_temp.size());
        		if(v_temp.size() % 13 == 0 ){
        			str_features->push_back(v_temp);
        			v_temp.clear();
        		}
        		else{
        		v_temp.clear();
        		}
//        		printf("push back error!");
        	}
        	else{
        		printf("split error!");
        		continue;
        	}
        }
        else{
        	printf("This line is empty.\n");
        	continue;
        }
//        printf("Line content:%s\n", line_buffer);
    }
   f_close(&test_data_inputs_pointer);
   }
   printf("End read line!\n");

    return;
}

void rebuilt_mfcc_feat(vector<string> flatten_mfcc, vector<vector<float> >* mfcc_float) {  //turn a flatten string to a 13*frame_num vector
//    vector<vector<float> > temp;
    vector<float> col_temp;
    for (int j = 0; j < (flatten_mfcc.size() / 13); j++)
    {
        for (int i = 0; i < 13; i++)
        {
            col_temp.push_back(stof(flatten_mfcc[j * 13 + i]));
        }
        mfcc_float->push_back(col_temp);
        col_temp.clear();
    }
    return;
}

void ComputeDTW(vector<vector<float> > cep1, vector<vector<float> > cep2, float* dtw) {
    float post_insert = 0, post_delete = 0, post_match = 0;
    float dtw_temp = 0;

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
            float cost;
            distance(s, t, &cost);
//            post_insert = distance(s_insert, t_insert);
//            post_match = distance(s_match, t_match);
//            post_delete = distance(s_delete, t_delete);
            distance(s_insert, t_insert, &post_insert);
            distance(s_match, t_match, &post_match);
            distance(s_delete, t_delete, &post_delete);
            float min_temp;
            min(post_insert, post_delete, post_match, &min_temp);
            dtw_temp += (cost + min_temp);
//            dtw_temp += (cost + min(post_insert, post_delete, post_match));
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
    *dtw = dtw_temp / (cep1.size() + cep2.size());
    return;
}

void distance(float *ps1, float *ps2, float* sum) {
    int i=0;
    float sum_temp=0;
    for(i=0;i<MFCC_P;i++){
    	sum_temp += abs(ps1[i] - ps2[i]);
    }
    *sum = sum_temp;
    return;
}

void min(float num1, float num2, float num3, float* min_value) {
//    float min;
    float temp;
    temp = (num1 < num2) ? num1 : num2;
    *min_value = (temp < num3) ? temp : num3;
    return;
}

void iteration_dtw(vector<vector<string> > match_csv_content, vector<vector<float> >  query_mfcc_feat, float* min_dist){
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
            float dtw_distance = 0;
//            float normal_dtw_distance = 0;
            rebuilt_mfcc_feat(match_csv_content[i], &match_mfcc_feat);
            //match_mfcc_feat = rebuilt_mfcc_feat(match_csv_content[i]);
//            printf("Computing...");
            ComputeDTW(query_mfcc_feat,match_mfcc_feat, &dtw_distance);
            //distance = ComputeDTW(query_mfcc_feat,match_mfcc_feat);
            normal_dist = dtw_distance / (query_mfcc_feat.size() + match_mfcc_feat.size());
            eud_distance.push_back(normal_dist);
        }
    }
    auto min_pos   = std::min_element(eud_distance.begin(), eud_distance.end());
    *min_dist = *min_pos;
    return;
}
