
#include"read_images.h"
#include<thread>
#include<filesystem>
#include<stdio.h>
#include<iomanip>
#include<stdlib.h>
#include<string>
#include<map>



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
 vector<string>train_path(string file);
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
        vector<uchar>output;
        return output;
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
        return result;
    
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
vector<string>train_path(string file){
	vector<string>output;
	ifstream f(file);
	if(!f.is_open()){
		exit(0);
	}
	string line;
	while(getline(f,line)){
		output.push_back(line);
	}
	f.close();
	return output;
}
bool training(vector<string>names,string weight_file){
	Image img("name");
	vector<vector<uchar>>train_images;
	for(auto &file:names){
		img.im_read(file,false);
		Image edge=img.canny(100,100);
		train_images.push_back(edge.pixel_data());
	}
	vector<int>weight(train_images[0].size());
	for(auto& pair:train_images){
		for(int i=0;i<weight.size();i++){
			if(pair[i]>PIXEL_UMBRAL){
				weight[i]+=1;
			}
		}
	ofstream f(weight_file);
	for(auto &pair:weight){
        f<<pair<<' ';
	}
	f.close();

	}
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
