#include <vector>
#include<fstream>
#include<iomanip>
#include <filesystem>
#include<opencv2/opencv.hpp>
using std::string;
using std::vector;
using cv::Mat;
namespace fs=std::filesystem;

class Data{
    private:
    vector<int>weight;
    int size;
    public:
    Data();
    void vector_handler(cv::Mat img);
    void Save_data(string stream);
};



class Image{
    private:
        unsigned char * file_header;
        unsigned char *img_header;
        cv::Mat image;
        int channels;
        vector<int> value;
        void get_channels();
    public:
        Image(string name);
        cv::Mat get_img_data();
        
};


//Funciones para la imagen
Image::Image(string name){
    image=cv::imread(name);
    if(!image.data){
        printf("Couldn't read data \n");
        return;
    }
    get_channels();
    
}
void Image::get_channels(){
    channels=image.channels();
    
}

Mat Image::get_img_data(){
    return image;
}



//Funciones para el vector
//Actualmente solo sirve con imagenes b&n
void Data::vector_handler(cv::Mat img){
    int step=img.step;
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
}



void Data::Save_data(string stream){
    std::ofstream f(stream);
    for(auto &pair:weight){
        f<<pair<<' ';
    }
    f.close();
}






void training(){
    std::string path = "../../train_data"; // Ruta de la carpeta
    vector<string> file_names;
    Data weight;
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