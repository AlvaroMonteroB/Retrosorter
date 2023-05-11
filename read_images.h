﻿#include<fstream>

#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;
typedef unsigned char uchar;
typedef unsigned int uint;


//============================================================================================================
//=================================================Structs====================================================
//============================================================================================================
typedef struct{
	//File header
	unsigned char file_header[2];//'BM'
	unsigned char file_size[4];// 	File size in bytes
	unsigned char reserved[4];// 	unused (=0)
	//Info header
	unsigned char file_des[4];//Offset from beginning of file to the beginning of the bitmap data
	unsigned char size_header[4];// 	Size of InfoHeader =40
	unsigned char img_width[4];// 	Horizontal width of bitmap in pixels
	unsigned char img_height[4];// 	Vertical height of bitmap in pixels
	unsigned char img_plane[2];//Number of Planes (=1)
	unsigned char img_depht[2];//Bytes per Pixel
	unsigned char img_compress[4];//Type of Compression
	unsigned char img_size[4];//(compressed) Size of Image
	unsigned char img_hor_res[4];//horizontal resolution: Pixels/meter
	unsigned char img_vert_res[4];//vertical resolution: Pixels/meter
	unsigned char img_color[4];//Number of actually used colors
	unsigned char img_color_imp[4];// 	Number of important colors
}BMP_H;//Bitmap header


typedef struct{

}JPG_H;//Jpeg header


typedef struct{

}PNG_H;//PNG header

typedef struct{
    uchar B;
    uchar G;
    uchar R;
}B_RGB_pixel;


//============================================================================================================
//======================================Prototipos de funciones===============================================
//============================================================================================================
int get_number(uchar*data,int size);
bool isBMP(string filename);
bool isJPG(string filename);
bool isPNG(string filename);
void hysteresis_thresholding(vector<float> magnitud, vector<float> direction, vector<uchar>& edges, float t_high, float t_low, int width, int height);
void sobel_filter(vector<vector<uchar>>img_2d,vector<float> &grad_x,vector<float>&grad_y);
void magnitude_direction(vector<float> grad_x,vector<float>grad_y,vector<float>&direction,vector<float>&magnitud);
vector<unsigned char> gaussian_filter(const vector<unsigned char>& image, int width, int height, int kernel_size, double sigma);
char* header_to_char(BMP_H header);

//
//==========================Clases

//Clase Image
class Image{//Al final solo necesitamos los bordes
	private:
		BMP_H header;
		vector<uchar>pixels;
		int channels;
		int step;
		uint width;
		uint height;
		int size;
		string type;
		vector<uchar>g_img;
		void read_bmp(string path);
		void read_jpg();
		void read_png();
		void get_grey();
		void output_img(int _channel,int _step, int _widht,int _height, int _size, string _type,BMP_H head_Er);
		vector<uchar> modif_header();
	public:
		~Image(){

		}
		int im_read(string path,bool band);
		void assign_vector(vector<uchar>input,uint n_height,uint n_width, int n_channel);
		Image(string name);
		void write_img(string name);
		Image *canny(float threshold1,float threshold2 );
        vector<uchar>pixel_data();
		vector<uchar> grey_vector();
		int Channels(){
			return channels;
		}
        int Step(){
            return step;
        }
        int Width(){
            return width;
        }
        int Height(){
            return height;
		}
		int Size(){
			return size;
		}




};

//Metodos de clase Image

Image::Image(string name){
	pixels.resize(0);
	channels=0;
	step=0;
	width=0;
	height=0;
	size=0;
	type="";
	g_img.resize(0);
}


int Image::im_read(string path,bool band){//true=color   false = b & n
	int optn;
	pixels.resize(0);
	g_img.clear();
	if (isBMP(path))
	{
		read_bmp(path);

	}else if(isJPG(path)){
        optn=2;
    }else if(isPNG(path)){
        optn=3;
    }else{
		return 1;
	}
	if(!pixels.data()){
			return 2;
		}


	get_grey();
	if (!band)
	{   pixels.resize(0);
		pixels=g_img;
		size=size/3;
		step=channels=1;

	}
	return 0;

    
    
}
//exit
void Image::read_bmp(string path){
    ifstream F(path,std::ios::binary);
	if(!F.is_open()){
		return;
	}
    type="bmp";
	F.read(reinterpret_cast<char*>(&header),sizeof(BMP_H));
	width=get_number(header.img_width,4);//Obteniendo el alto y ancho de la imagen
	height=get_number(header.img_height,4);
	channels=get_number(header.img_depht,2)/8;//
	size=get_number(header.img_size,4);
	step=get_number(header.img_depht,2)/8;
	F.seekg(get_number(header.file_des,4),std::ios::beg);//Posicionamos el apuntador al final del offset
	pixels.resize(channels*width*height);
	F.read(reinterpret_cast<char*>(pixels.data()),(width*height*channels));//Leemos todos los pixeles y se almacenan en un vector
	F.close();

}

void Image::get_grey(){
	if (channels==1)
	{
        g_img=pixels;
        return;
    }
    
    for (int i = 0; i < pixels.size(); i+=3)
    {
        uchar B = pixels[i];
        uchar G = pixels[i+1];
        uchar R = pixels[i+2];
        uchar prom=(B+R+G)/3;
        g_img.push_back(prom);
	}
}

void Image::assign_vector(vector<uchar>input,uint n_height,uint n_width, int n_channel){
	pixels=input;
	height=n_height;
	width=n_width;
	channels=n_channel;
}

void Image::output_img(int _channel,int _step, int _widht,int _height, int _size, string _type,BMP_H head_Er){//Inicializar imagen de salida
     this->channels=_channel;
     this->step=_step;
     this->width=_widht;
     this->height=_height;
     this->size=_size;
     this->type=_type;
     this->header=head_Er;
}

Image *Image::canny(float threshold1,float threshold2){
    vector<uchar>edges;
	Image *output=new Image("name");
	output->output_img(1,1,width,height,size,type,header);
	vector<uchar>filtered=gaussian_filter(g_img,width,height,5,1);
    std::vector<std::vector<uchar>> img_2d(height, std::vector<uchar>(width));
    for (int i = 0; i < height; i++) {//i=y
        for (int j = 0; j < width; j++) {//j=x
            img_2d[i][j] = filtered[i * width + j];
        }
    }
    vector<float>gradx(height*width),grady(height*width);
    sobel_filter(img_2d,gradx,grady);
    vector<float>magnitud,direccion;
    magnitude_direction(gradx,grady,direccion,magnitud);
	hysteresis_thresholding(magnitud,direccion,edges,(.9),(.4),width,height);
	output->assign_vector(edges,height,width,1);
	gradx.clear();
	grady.clear();
	magnitud.clear();
	direccion.clear();
	edges.clear();
	return output;
}

vector<uchar> Image::pixel_data(){
    return pixels;
}

vector<uchar> Image::grey_vector(){
    return g_img;
}

vector<uchar> Image::modif_header() {
    vector<uchar> output;

    // Copiar los datos de los miembros de la estructura header en el vector output
    output.insert(output.end(), header.file_header, header.file_header + 2);
    output.insert(output.end(), header.file_size, header.file_size + 4);
    output.insert(output.end(), header.reserved, header.reserved + 4);
    output.insert(output.end(), header.file_des, header.file_des + 4);
    output.insert(output.end(), header.size_header, header.size_header + 4);
    output.insert(output.end(), header.img_width, header.img_width + 4);
    output.insert(output.end(), header.img_height, header.img_height + 4);
    output.insert(output.end(), header.img_plane, header.img_plane + 2);

    // Copiar el arreglo profundidad
    uchar profundidad[2];
    profundidad[0] = (channels * 8) & 0xff;
    profundidad[1] = ((channels * 8) >> 8) & 0xff;
    output.insert(output.end(), profundidad, profundidad + 2);

    output.insert(output.end(), header.img_compress, header.img_compress + 4);
    output.insert(output.end(), header.img_size, header.img_size + 4);
    output.insert(output.end(), header.img_hor_res, header.img_hor_res + 4);
    output.insert(output.end(), header.img_vert_res, header.img_vert_res + 4);
    output.insert(output.end(), header.img_color, header.img_color + 4);
    output.insert(output.end(), header.img_color_imp, header.img_color_imp + 4);

    return output;
}


void Image::write_img(string name){
    vector<uchar> head =modif_header();
    ofstream f(name);
    if (!f.is_open())
    {
        cout<<"Couldnt open file\n";
    }
    
    f.write(reinterpret_cast<char*>(head.data()),54);
    f.write(reinterpret_cast<char*>(pixels.data()),size);
    f.close();

}
    

//=================================
//==========Funciones para el filtro de canny
vector<unsigned char> gaussian_filter(const vector<unsigned char>& image, int width, int height, int kernel_size, double sigma) {
   cout<<"Filtro gaussiano\n";
    vector<unsigned char> filtered_image(width * height);

    // Crear kernel gaussiano
    vector<double> kernel(kernel_size * kernel_size);
    double sum = 0;
    int r = kernel_size / 2;
    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            double value = exp(-(i * i + j * j) / (2 * sigma * sigma));
            kernel[(i + r) * kernel_size + (j + r)] = value;
            sum += value;
        }
    }
    for (int i = 0; i < kernel_size * kernel_size; i++) {
        kernel[i] /= sum;
    }

    // Aplicar kernel a cada píxel de la imagen
    for (int y = r; y < height - r; y++) {
        for (int x = r; x < width - r; x++) {
            double sum = 0;
            for (int i = -r; i <= r; i++) {
                for (int j = -r; j <= r; j++) {
                    sum += image[(y + i) * width + (x + j)] * kernel[(i + r) * kernel_size + (j + r)];
                }
            }
            filtered_image[y * width + x] = sum;
        }
    }

    return filtered_image;
}

void sobel_filter(vector<vector<uchar>>img_2d,vector<float> &grad_x,vector<float>&grad_y){
    int gx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
    int gy[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
    for (int i = 0; i < img_2d.size(); i++)//i=y
    {
        for (int j = 0; j < img_2d[0].size(); j++)//j=x
        {
            float sumx=0,sumy=0;
            for (int x = 0; x <=1; x++)
            {
                for (int y = -1; y <=1 ; y++)
                {   if(i==img_2d.size()-1&&x>0){
                        sumx+=gx[x+1][y+1]*img_2d[i][j+y];
                        sumy+=gy[x+1][y+1]*img_2d[i][j+y];

                        }else if(j==img_2d[0].size()-1&&y>0){
                            sumx+=gx[x+1][y+1]*img_2d[i+x][j];
                            sumy+=gy[x+1][y+1]*img_2d[i+x][j];
                        }else if(j==img_2d[0].size()-1&&i==img_2d.size()-1&&x>0&&y>0){
                            sumx+=gx[x+1][y+1]*img_2d[i][j];
                            sumy+=gy[x+1][y+1]*img_2d[i][j];
                        }else{
                            sumx+=gx[x+1][y+1]*img_2d[i+x][j+y];
                            sumy+=gy[x+1][y+1]*img_2d[i+x][j+y];
                        }
                    
                }
                
            }
            grad_x[i*img_2d[0].size()+j]=(sumx);
            grad_y[i*img_2d[0].size()+j]=(sumy);

        }
        
    }
    
}

void magnitude_direction(vector<float> grad_x,vector<float>grad_y,vector<float>&direction,vector<float>&magnitud){
    for (int i = 0; i < grad_x.size(); i++)
    {
        magnitud.push_back(sqrt(pow(grad_x[i],2)+pow(grad_y[i],2)));
        direction.push_back(atan2(grad_y[i],grad_x[i]));
    }
    
}

void hysteresis_thresholding(vector<float> magnitud, vector<float> direction, vector<uchar>& edges, float t_high, float t_low, int width, int height) {
    cout<<"Hsytieresis\n    ";
    cout<<"Magnitud: "<<to_string(magnitud.size())<<" direccion: "<<to_string(direction.size());
    edges.resize(width * height);
    // Convertimos los umbrales a valores de magnitud
    float high_threshold = t_high * (*std::max_element(magnitud.begin(), magnitud.end()));
    float low_threshold = t_low * high_threshold;
    // Recorremos la imagen y aplicamos los umbrales
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i * width + j;
            if (magnitud[index] >= high_threshold) {
                edges[index] = 255;
            }
            else if (magnitud[index] < low_threshold) {
                edges[index] = 0;
            }
            else {
                // Realizamos el seguimiento de bordes
                float dir = direction[index];
                float mag = magnitud[index];
                int x1 = j + round(cos(dir));
                int y1 = i + round(sin(dir));
                int x2 = j - round(cos(dir));
                int y2 = i - round(sin(dir));
                if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height && magnitud[y1 * width + x1] >= high_threshold) {
                    edges[index] = 255;
                }
                else if (x2 >= 0 && x2 < width && y2 >= 0 && y2 < height && magnitud[y2 * width + x2] >= high_threshold) {
                    edges[index] = 255;
                }
            }
        }
    }
    cout<<"Vector de imagen= "<<edges.size();
}
//==============================================================================================================
//===============================================Tipo de imagen=================================================
bool isBMP(string filename) {
	return (filename.size() > 4 && filename.substr(filename.size() - 4) == ".bmp");
}

bool isJPG(string filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        return false;
    }
    unsigned char buffer[2];
	file.read((char*)buffer, 2);
	file.close();
    return (buffer[0] == 0xFF && buffer[1] == 0xD8);
}

bool isPNG(string filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        return false;
    }
    unsigned char buffer[8];
    file.read((char*)buffer, 8);
    return (buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4E && buffer[3] == 0x47
		&& buffer[4] == 0x0D && buffer[5] == 0x0A && buffer[6] == 0x1A && buffer[7] == 0x0A);
}

int get_number(uchar*data,int size){
	int n=0;
	for (int i = 0; i < size; i++)
	{
		n|=static_cast<int>(data[i])<<(i*8);
	}
	return n;

}

