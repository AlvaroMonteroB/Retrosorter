#include<Windows.h>
#include"read_images.h"
#include<thread>
#include<filesystem>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<map>
#include<omp.h>


using std::vector;
using std::string;
namespace fs=std::filesystem;
#define PIXEL_UMBRAL 100
typedef struct{
        string umbral;
        string weights;
}Operation_data;




typedef struct{
    int sum;
    string file;
}Sum_name;

typedef struct{
    string file;
    vector<int>weight;
}File_weight;


typedef struct{
    string file;
    int sum;
}File_umbral;
//
//==========================
//

//=============================================================================================================
//======================================================CLASES=================================================
//=============================================================================================================
class Data{//Clase para construir el vector de pesos
    private:
    vector<int>weight;
    string name;//La direccion en la cual se almacena el archivo de pesos
    int size;
    public:
    Data(string file_name,int sz=0){
        name=file_name;
        size=sz;
    }
    
    vector<uchar> vector_handler(){//Este handler es por si quiero entrenar varias neuronas a la vez con no muchos datos
    }

//Este resultado se debe tomar de cada thread para usarlo
    
    
    void final_vector(vector<vector<int>>pesos,string adress_){//Vamos a sumar todos los vectores a la vez
        int siz=pesos[0].size();
        weight.resize(siz);
        for (auto &vect_:pesos)
        {            
            for(int i=0;i<siz;i++){
                weight[i]+=vect_[i];
            }
        }
        Save_data(adress_);
    }

    void save_new_vector(vector<int>wg,string file){//Este es para reforzar el entrenamiento
        weight=wg;
        Save_data(file);
    }

    void Save_data(string stream){//Guardamos el vector de pesos en un archivo
    std::ofstream f(stream);
    for(auto &pair:weight){
        f<<pair<<' ';
    }
    f.close();
}
};






vector<Image>Read_images(string root_path);


class Percept{
    private:
    vector<File_weight>Weight_matrix;
    vector<int> weight;//El vector de pesos adquirido
    string threshold;
    int umbral;
    public:
    vector<int> get_weight(int i){//Adquirir el vector de pesos
        return Weight_matrix[i].weight;
    }
    
    Percept(vector<string> file_names){//Adquirir el vector de pesos
        File_weight aux;
       for (auto &pair:file_names)
       {
        umbral=0;
        threshold="hello";
        std::ifstream f(pair);
        int valor;
        while(f>>std::setw(4)>>valor){
            weight.push_back(valor);
        }//Aqui se llena el primer vector de pesos
        f.close();
        aux={pair,weight};
        Weight_matrix.push_back(aux);
        weight.clear();
       }
        
    }
    
    vector<Sum_name> neuron(vector<uchar>input){//Algoritmo de la neurona
        vector<Sum_name>result;
        int sum=0;
        Sum_name aux={0,""};
        for (auto &pair:Weight_matrix)
        {
            for (int i = 0; i < pair.weight.size(); i++)
            {
                sum+=(input[i])*pair.weight[i];
            }
            result.push_back({sum,pair.file});
        }
        
    
    }
    //Leemos la imagen para multiplicarla por el vector de pesos

    

    void read_threshold(string direction){//Leer el umbral
        threshold=direction;
        std::ifstream F(threshold);
        std::getline(F,direction);
        umbral=std::stoi(direction);

    }

    int get_threshhold(){//Tener el umbral
        return umbral;
    }
};
//=============================================================================================================
//==================================TRAINING FUNCTION==========================================================
//=============================================================================================================
//Archivo de pesos y la carpeta de los datos de entrenamiento



void training(string weight_neuron){//Para entrenar una sola neurona con poca información

    std::string path = "../Camera_data/train_data"; // Ruta de la carpeta
    vector<string> file_names;
    Data weight(weight_neuron,0);
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) { // Verificar si es un archivo regular
            file_names.push_back(entry.path().string()); //aqui guardamos cada nombre del directorio
            
        }
    }
    vector<Image> images;
    int i=0;
    for (auto &pair:file_names)//Ahora formamos un vector con las imagenes
    {
        images.push_back(Image(pair));
    }
    for(Image &pair:images){//Convertir imagenes a vectores
    
    }
    std::cout<<"Trained\n";
    weight.Save_data("../weights/weight_data_Camera.txt");
    return;
}

void thread_training(vector<Image>images,string train_file){//Para entrenar varias neuronas a la vez
    Data weight("si");
    int i=0;
    for(Image &pair:images){//Convertir imagenes a vectores
       
    }
    std::cout<<"Trained\n";
    weight.Save_data(train_file);
    return;
}

vector<Image>Read_images(string root_path){//Leer el directorio raiz para sacar las imagenes en el
    vector<string>filenames;
    for (const auto& entry : fs::directory_iterator(root_path)) {
        if (entry.is_regular_file()) { // Verificar si es un archivo regular
            filenames.push_back(entry.path().string()); //aqui guardamos cada nombre del directorio
            
        }
    }
    vector<Image>output;
    for(auto &pair:filenames){
        output.push_back(pair);
    }
    return output;
}


//==============================================================================================================
//============================================NEURON APPLICATION===================================================
/*vector<File_umbral> neural_application(vector<File_umbral>file_names){
    Percept Neuron1(weight_file);
    Neuron1.read_threshold(threshold_file);
    for (auto &pair:file_names)
    {
        vector<uchar>img=Neuron1.input_data(pair.file);
        pair.sum=Neuron1.neuron(img);
        if(pair.sum>Neuron1.get_threshhold()){
            std::cout<<pair.file<<" Es una camara con puntuacion "<<pair.sum<<'\n' ;
        }else{
            std::cout<<pair.file<<" No es una camara con puntuacion "<<pair.sum<<'\n';
        }


    }
    
        
    return file_names;
    
}   
*/
//Conseguir los archivos para el testeo
vector <string> testing_path(string path){
    vector<string> file_names;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) { // Verificar si es un archivo regular
            file_names.push_back(entry.path().string()); //aqui guardamos cada nombre del directorio
        }
    }
    return file_names;
}
