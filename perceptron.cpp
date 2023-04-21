#include <vector>
#include<fstream>
#include<iomanip>
typedef unsigned char uchar;
using std::vector;
using std::string;
class Percept{
    private:
    vector<int> weight;
    string train_data;
    public:
    Percept(string filename);
    int neuron(vector<uchar>input);
};

Percept::Percept(string filename){
    train_data=filename;
    std::ifstream f(train_data);
    int valor;
    while(f>>std::setw(4)>>valor){
        weight.push_back(valor);
    }
    f.close();
}

int Percept::neuron(vector<uchar>input){
    int suma=0;
    for (size_t i = 0; i < input.size(); i++)
    {
       suma+=weight[i]*input[i]; 
    }
    return suma;
}