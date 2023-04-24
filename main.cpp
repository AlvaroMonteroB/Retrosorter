#include"lib.h"

using std::cout;

void main(){
    vector<File_umbral>file_names;
    vector<string>names=testing_path("../test_data");    
    for (auto &pair:names)
    {
        File_umbral test={pair,0};
        file_names.push_back(test);
    }
    
    
    
    file_names=neural_application(file_names,"../weight_data.txt");

    
//Percept neuron1("../weight_data.txt");


    
    
    
}