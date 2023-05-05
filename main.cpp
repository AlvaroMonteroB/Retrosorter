#include"lib.h"
#include"window.h"
using std::cout;
//g++ -fopenmp main.cpp -o a
void training();

int main(){
    Image imagen("h");
    imagen.im_read("Camera_data/train_data_camara1.bmp",false);///true = color   false = b & n 
    Image edg=imagen.canny(100,200);
    show_image_window(edg.pixel_data(),edg.Width(),edg.Height(),edg.Channels());
return 0;   
}



void training(){
    
}