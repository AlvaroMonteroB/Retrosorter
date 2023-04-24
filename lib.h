#include<vector>
#include<fstream>
#include<filesystem>
#include<opencv2/opencv.hpp>
#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<iostream>

using cv::Mat;
using std::vector;
using std::string;
namespace fs=std::filesystem;
#define PIXEL_UMBRAL 100


typedef struct{
    string file;
    int sum;
}File_umbral;

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
    vector<uchar> vector_handler(cv::Mat img){
        unsigned int step=unsigned int(img.step);
        vector<uchar> pixels;
        cv::Size dim=img.size();
        if (weight.empty()){
            size=(dim.width*dim.height);
            if (img.isContinuous()){
                pixels.assign(img.data,img.data+img.total()*img.channels());
            }else{
                for (int i = 0; i < dim.height; i++)
                    {
                        uchar*row=img.ptr<uchar>(i);
                        pixels.insert(pixels.end(),row,row+img.cols*img.channels());
                    }
            }
        //clasificacion pedorra

        
        for (auto &pair:pixels)
        {
            if(pair<PIXEL_UMBRAL){
                weight.push_back(1);
            }else{
                weight.push_back(0);
            }
        }
        
        
    }else{//Construyendo el vector de pesos
        if (dim.width*dim.height==size){//La imagen es del mismo tamaño, después puedo quitar esto
            if(img.isContinuous()){
                pixels.assign(img.data,img.data+img.total()*img.channels());
            }else{
            for (int i = 0; i < dim.height; i++)
                {
                    uchar*row=img.ptr<uchar>(i);
                    pixels.insert(pixels.end(),row,row+img.cols*img.channels());
                }
            }
            for (unsigned int i = 0; i < pixels.size(); i++)
            {
                if(pixels[i]<PIXEL_UMBRAL){
                    weight[i]+=1;
                }
            }
            


        }
    }
    return pixels;
}

    void save_new_vector(vector<int>wg,string file){
        weight=wg;
        Save_data(file);
    }

    void Save_data(string stream){
    std::ofstream f(stream);
    for(auto &pair:weight){
        f<<pair<<' ';
    }
    f.close();
}
};




class Image{
    private:
        cv::Mat image;
        int channels;
        vector<int> value;
        void get_channels(){
    channels=image.channels();  
}
    public:
        Image(string name){
        image=cv::imread(name,cv::IMREAD_GRAYSCALE);
        if(!image.data){
            printf("Couldn't read data \n");
            return;
        }
        get_channels();
    
}
        cv::Mat get_img_data(){
    return image;
}
        
};




class Percept{
    private:
    vector<int> weight;//El vector de pesos adquirido
    string train_data;//Direccion y nombre del archivo de pesos
    string threshold;
    int umbral;
    public:
    vector<int> get_weight(){
        return weight;
    }
    Percept(string filename){//Adquirir el vector de pesos
        train_data=filename;
        umbral=0;
        threshold="hello";
        std::ifstream f(train_data);
        int valor;
        while(f>>std::setw(4)>>valor){
            weight.push_back(valor);
        }
        f.close();
    }
    
    int neuron(vector<uchar>input){//Algoritmo de la neurona
        int suma=0;
        for (int i = 0; i < input.size(); i++)
        {
        suma+=weight[i]*input[i]; 
        }
        return suma;
}
    
    vector<uchar> input_data(string name){//Leerá una imagen a la vez
        vector<uchar>output;//Salida de la funcion
        Image file=Image(name);
        cv::Mat data_i=file.get_img_data();
        output.reserve(data_i.total());
        if(data_i.isContinuous()){
            output.assign(data_i.data,data_i.data+data_i.total()*data_i.channels());
        }else{
            for (int i = 0; i < data_i.rows; i++)
            {
                uchar *row=data_i.ptr<uchar>(i);
                output.insert(output.end(),row,row+data_i.cols*data_i.channels());
            }
            
        }
        
        return output;
    }
    
    void reinforcement(cv::Mat img){
        unsigned int step=unsigned int(img.step);
        vector<uchar> pixels;
        cv::Size dim=img.size();
        if (dim.width*dim.height==weight.size()){//La imagen es del mismo tamaño, después puedo quitar esto
            if(img.isContinuous()){
                pixels.assign(img.data,img.data+img.total()*img.channels());
            }else{
            for (int i = 0; i < dim.height; i++)
                {
                    uchar*row=img.ptr<uchar>(i);
                    pixels.insert(pixels.end(),row,row+img.cols*img.channels());
                }
            }
            for (unsigned int i = 0; i < pixels.size(); i++)
            {
                if(pixels[i]<PIXEL_UMBRAL){
                    weight[i]+=1;
                }
            }
            


        }
        Data output("No_name");
        output.save_new_vector(weight,train_data);
        
        
    }

    void read_threshold(string direction){
        threshold=direction;
        std::ifstream F(threshold);
        std::getline(F,direction);
        umbral=std::stoi(direction);
        std::cout<<umbral<<"\n";

    }

    int get_threshhold(){
        return umbral;
    }
};
//=============================================================================================================
//==================================TRAINING FUNCTION==========================================================
//=============================================================================================================

void training(string weight_neuron){

    std::string path = "../train_data"; // Ruta de la carpeta
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
        weight.vector_handler(pair.get_img_data());
    }
    std::cout<<"Trained\n";
    weight.Save_data("../weight_data.txt");
    return;
}


//==============================================================================================================
//============================================NEURON APPLICATION===================================================
vector<File_umbral> neural_application(vector<File_umbral>file_names,string weight_file){
    Percept Neuron1(weight_file);
    Neuron1.read_threshold("../umbral.txt");
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

vector <string> testing_path(string path){
    path="../test_data";
    vector<string> file_names;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) { // Verificar si es un archivo regular
            file_names.push_back(entry.path().string()); //aqui guardamos cada nombre del directorio
        }
    }
    return file_names;
}
