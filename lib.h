#include<vector>
#include<fstream>
#include<thread>
#include<filesystem>
#include<opencv2/opencv.hpp>
#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<omp.h>
#include<iostream>

using cv::Mat;
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
    
    vector<uchar> vector_handler(cv::Mat img){//Este handler es por si quiero entrenar varias neuronas a la vez con no muchos datos
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

    vector<int>threaded_vector(cv::Mat img){//Este es para entrenar a la neurona con muchos datos
        vector<int>peso;
        peso.resize(img.total());
        vector<uchar> pixels;
        cv::Size dim=img.size();
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

        for (unsigned int i = 0; i < pixels.size(); i++)
            {
                if(pixels[i]<PIXEL_UMBRAL){
                    peso[i]+=1;
                }
            }
        return peso;//Este resultado se debe tomar de cada thread para usarlo
    }
    
    void final_vector(vector<vector<int>>pesos,string adress_){//Vamos a sumar todos los vectores a la vez
        int siz=pesos[0].size();
        weight.resize(siz);
        #pragma omp parallel for reduction(+:weight)
        for (auto &vect_:pesos)
        {
            for(int i;i<siz;i++){
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




class Image{
    private:
        cv::Mat image;
        int channels;
        vector<int> value;
        void get_channels(){//Metodo privado para obtener los canales
            channels=image.channels();  
            }
    public:
        Image(string name){//Constructor que lee las imagenes y adquiere los canales
        image=cv::imread(name,cv::IMREAD_GRAYSCALE);
        if(!image.data){
            printf("Couldn't read data \n");
            return;
        }
        get_channels();
    
}
        
        cv::Mat get_img_data(){//Retorna la matriz de imagen
    return image;
}
        
};




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
        #pragma omt parallel for
        for (auto &pair:Weight_matrix)
        {
            for (int i = 0; i < pair.weight.size(); i++)
            {
                sum+=(255-input[i])*pair.weight[i];
            }
            result.push_back({sum,pair.file});
        }
        
    
    }
    //Leemos la imagen para multiplicarla por el vector de pesos
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

void heavy_training(string weight_file,string root_index){//Este entrenamiento es para solo una neurona con muchos datos
    Data h_data(weight_file,0);
    vector<Image>images=Read_images(root_index);
    vector<vector<int>>result;
    #pragma omp parallel for
    for(auto &pair:images){
        result.push_back(h_data.threaded_vector(pair.get_img_data()));
    }
    h_data.final_vector(result,weight_file);
}


void training(string weight_neuron){//Para entrenar una sola neurona con poca información

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

void thread_training(vector<Image>images,string train_file){//Para entrenar varias neuronas a la vez
    Data weight("si");
    int i=0;
    for(Image &pair:images){//Convertir imagenes a vectores
        weight.vector_handler(pair.get_img_data());
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
