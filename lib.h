#include<vector>
#include<fstream>
#include<filesystem>
#include<opencv2/opencv.hpp>
using cv::Mat;
using std::vector;
using std::string;
namespace fs=std::filesystem;

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
            if(pair>240){
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
                if(pixels[i]>240){
                    weight[i]+=1;
                }
            }
            


        }
    }
    return pixels;
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
        image=cv::imread(name);
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
    vector<int> weight;
    string train_data;
    public:
    Percept(string filename){//Adquirir el vector de pesos
    train_data=filename;
    std::ifstream f(train_data);
    int valor;
    while(f>>std::setw(4)>>valor){
        weight.push_back(valor);
        weight=weight;
    }
    f.close();
}
    int neuron(vector<uchar>input){//Algoritmo de la neurona
    int suma=0;
    for (size_t i = 0; i < input.size(); i++)
    {
       suma+=weight[i]*input[i]; 
    }
    return suma;
}
    vector<uchar> input_data(string name){//Leerá una imagen a la vez
        vector<uchar>output;//Salida de la funcion
        Image file=Image(name);
        cv::Mat data_i=file.get_img_data();
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


};
//=============================================================================================================
//==================================TRAINING FUNCTION==========================================================
//=============================================================================================================

void training(string weight_neuron){
    std::string path = "../../train_data"; // Ruta de la carpeta
    vector<string> file_names;
    Data weight("../../weight_data.txt",0);
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) { // Verificar si es un archivo regular
            file_names.push_back(entry.path().string()); //aqui guardamos cada nombre del directorio
            
        }
    }
    vector<Image> images;
    for (auto &pair:file_names)//Ahora formamos un vector con las imagenes
    {
        images.push_back(Image(pair));
    }
    for(Image &pair:images){//Convertir imagenes a vectores
        weight.vector_handler(pair.get_img_data());
    }
    std::cout<<"Trained\n";
    weight.Save_data("../../weight_data.txt");
    return;
}


//==============================================================================================================
//============================================NEURON APPLICATION===================================================
void neural_application(int neu_num,vector<string>in_names){

}
