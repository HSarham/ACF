#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <set>
#include <filesystem>

using namespace std;

struct ComparisonResult{
    string colored_string_1, colored_string_2;
    int max_ind_1, max_ind_2, max_length_1, max_length_2;
    set<int> file_1_inds, file_2_inds;
    string max_common_string_1(){
        return colored_string_1.substr(max_ind_1, max_length_2);
    }
    string max_common_string_2(){
        return colored_string_2.substr(max_ind_2, max_length_2);
    }
    string file_name_1, file_name_2;
    int max_length(){
        return std::max(max_length_1, max_length_2);
    }
    bool operator < (ComparisonResult &input){
        return this->max_length()<input.max_length();
    }
};

void process_LCS(string file, set<int> file_inds, int &max_ind, int& max_length, string &colored_text){
    bool in = false;
    int last_length = 0;
    int start_ind = 0;
    max_ind = 0;
    max_length = 0;
    colored_text = "";
    for(int i=0;i<file.size();i++){
        if(file_inds.count(i)>0){
            last_length ++;
            if(!in){
                colored_text += "\033[31m";
                in = true;
                start_ind = i;
            }
        }
        else{
            if(in){
                colored_text += "\033[0m";
                in = false;
                if (max_length<last_length){
                    max_length = last_length;
                    max_ind = start_ind;
                }

                last_length=0;
            }
        }
        colored_text += file[i];
    }
    if(in){
        colored_text += "\033[0m";
        in = false;
    }
}

string read_file(ifstream &in_file){
    in_file.seekg(0,std::ios_base::end);
    int end_pos = in_file.tellg();
    in_file.seekg(0,std::ios_base::beg);
    string output;
    output.resize(end_pos);
    in_file.read(output.data(),end_pos);
    return output;
}

void LCS(string file_1, string file_2, set<int> &file_1_inds, set<int> &file_2_inds){
    vector<vector<int>> table(file_1.size(),vector<int>(file_2.size(),0));

    for(int i=0;i<file_1.size();i++){
        for(int j=0;j<file_2.size();j++){
            if(file_1[i]==file_2[j]){

                if(i>0 && j>0){
                    table[i][j] = table[i-1][j-1] + 1;
                }
                else{
                    table[i][j] = 1;
                }
            }
            else{
                int choice_1 = 0, choice_2 = 0;
                if(i>0){
                    choice_1 = table[i-1][j];
                }
                if(j>0){
                    choice_2 = table[i][j-1];
                }

                table[i][j] = std::max(choice_1,choice_2);
            }
        }
    }

    int i = file_1.size()-1;
    int j = file_2.size()-1;

    while(i>=0 && j>=0){
        if(file_1[i]==file_2[j]){
            file_1_inds.insert(i);
            file_2_inds.insert(j);
            i--;
            j--;
        }
        else{
            int choice_1 = 0, choice_2 = 0;
            if(i>0){
                choice_1 = table[i-1][j];
            }
            if(j>0){
                choice_2 = table[i][j-1];
            }
            if (choice_1 > choice_2){
                i--;
            }
            else{
                j--;
            }
        }
    }

    return;
}

ComparisonResult compare_files(string path_1, string path2){
    ifstream in_file_1(path_1), in_file_2(path2);
    string file_1,file_2;

    file_1 = read_file(in_file_1);
    file_2 = read_file(in_file_2);

    set<int> file_1_inds, file_2_inds;

    LCS(file_1, file_2, file_1_inds, file_2_inds);


    ComparisonResult comparison_result;

    process_LCS(file_1,file_1_inds, comparison_result.max_ind_1, comparison_result.max_length_1, comparison_result.colored_string_1);
    process_LCS(file_2,file_2_inds, comparison_result.max_ind_2, comparison_result.max_length_2, comparison_result.colored_string_2);

    return comparison_result;
}

int main(int argc, char* argv[]){

    if (argc<2){
        cout<<"Args: (<input_folder> | <input_file_1> <input_file_2>)"<<endl;
        return 0;
    }

    bool folder_mode = true;

    if (argc>2){
        folder_mode = false;
    }

    if(folder_mode){
        //get the file paths
        filesystem::path folder_path = argv[1];
        vector<filesystem::path> file_paths;
        for (auto &dir_ent: filesystem::directory_iterator(folder_path)){
            if(dir_ent.is_regular_file()){
                file_paths.push_back(dir_ent.path());
            }
        }
        //compare all file pairs
        const size_t num_files = file_paths.size();
        vector<ComparisonResult> comparison_reuslts;
        for (int i=0; i<num_files; i++){
            for (int j=i+1; j<num_files; j++){
                ComparisonResult comparison_result = compare_files(file_paths[i],file_paths[j]);
                comparison_result.file_name_1 = file_paths[i].filename();
                comparison_result.file_name_2 = file_paths[j].filename();
                comparison_reuslts.push_back(comparison_result);
            }
        }
        std::sort(comparison_reuslts.begin(),comparison_reuslts.end());
        for(ComparisonResult &cr:comparison_reuslts){
            cout<<cr.max_length()<<": "<<cr.file_name_1<<" "<<cr.file_name_2<<endl;
        }
    }
    if (!folder_mode){
        auto comparison_result = compare_files(argv[1],argv[2]);
        cout<<comparison_result.max_common_string_1()<<endl;
        cout<<"------------------"<<endl;
        cout<<comparison_result.max_common_string_2()<<endl;
    }

    return 0;
}
