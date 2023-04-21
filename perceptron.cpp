#include <vector>
#include<fstream>
#include<iomanip>
typedef unsigned char uchar;
using std::vector;
class Percept{
    private:
    vector<int> weight;
    public:
    Percept();
    int neuron(vector<uchar>input);
};

Percept::Percept(){
    std::ifstream f("weigth_data.txt");
    int valor;
    while(f>>valor){
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