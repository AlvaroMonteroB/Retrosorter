//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
#include"Unit2.h"
//---------------------------------------------------------------------------

vector<string>filenames;
vector<vector<uchar>>images_data;
  int get_bitmap(Image input,bool mode,TBitmap *output);
  vector<uchar> stretch(Image input,int _height, int _width);
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::processClick(TObject *Sender)
{
ShowMessage("Escritura de pesos a "+weight_data->Text+"\n"+"Archivos de entrenamiento "+txt_path->Text);
	Image img ("Name");
	Image aux("name");
	Image aux2("name2");
	System::AnsiString ansistr=txt_path->Text;//System::AnsiString.c_str()
	string path= ansistr.c_str();//Archivo donde almacenamos las direcciones del train data
	ansistr=weight_data->Text;
	string weight_path=ansistr.c_str();
	filenames=train_path(path);

	if(!filenames.data()){
	ShowMessage("No se pudieron leer los nombres");
	}else{
	string stored=to_string(filenames.size())+" vectors stored";
	ShowMessage(stored.data());}
	/*for(auto &pair:filenames){
		img.im_read(pair,false);
		aux.assign_vector(stretch(img,504,378),504,378,1);
		aux2=aux.canny(100,200);
		images_data.push_back(aux2.pixel_data());
	}   */

}
//---------------------------------------------------------------------------
 //D:\Repositorios\Inv_proj1\train_path.txt
 //D:\Repositorios\Inv_proj1\weights\weight_data_Camera.txt
 //C:\Users\diavl\OneDrive\Escritorio\Repositorios\Inv_proj1\train_path.txt

//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (filenames.empty()) {
		return;
	}
	Image show("Name");
	int read= show.im_read(filenames[0],false);
	string mess;
	string b=filenames[0];
	ShowMessage(b.data());
	if(read>0){
	mess=error(read);
	ShowMessage(mess.data());
	exit(0);
	return;
	} else{

	   ShowMessage("Imagen leida");
	}
	TForm2 *newForm=new TForm2(this);
	TBitmap *bmp=new TBitmap();

	get_bitmap(show,false,bmp);
   newForm->Image1->Picture->Assign(bmp);
   newForm->Image1->SetBounds(0,0,bmp->Width,bmp->Height);
   newForm->ShowModal();
	vector<uchar>x=stretch(show,504,378);
	if (x.size()==0) {
		ShowMessage("No se pudo escribir el vector");
		exit(2);
	}else if(x.size()==1){
		ShowMessage("La relacion no coincide");
	}
	show.assign_vector(x,504,378,1);
	get_bitmap(show,false,bmp);
   newForm->Image1->Picture->Assign(bmp);
   newForm->Image1->SetBounds(0,0,bmp->Width,bmp->Height);
   newForm->ShowModal();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

int get_bitmap(Image input,bool mode,TBitmap *output){//true =color; false =b & n
 int height=input.Height();
 int width=input.Width();
 int channels=input.Channels();
 vector<uchar>pixels=input.pixel_data();
 output->PixelFormat=pf24bit;
 if(!mode &&channels!=1){
	   pixels=input.grey_vector();
	   output->SetSize(width,height);
 }else if(!mode&&channels==1){
		output->SetSize(width,height);

	for(int y=0; y<height;y++){
		uchar* row=(uchar*)output->ScanLine[y];
		for(int x=0;x<width;x++){//width 3024, height 4032
			//
			row[x*3]=(pixels[y*width+x]);
			row[x*3+1]=(pixels[y*width+x]);
			row[x*3+2]=(pixels[y*width+x]);
		}

	}
 }
 else if(mode&&channels==1){
	   for(int y=0; y<height;y++){
		uchar* row=(uchar*)output->ScanLine[y];
		for(int x=0;x<width;x++){//width 3024, height 4032
			//
			row[x*3]=(pixels[y*width+x]);
			row[x*3+1]=(pixels[y*width+x]);
			row[x*3+2]=(pixels[y*width+x]);
		}

	}
 }else if(mode&&channels>1){
	  for(int y=0; y<height;y++){
		uchar* row=(uchar*)output->ScanLine[y];
		for(int x=0;x<width;x++){//width 3024, height 4032
			int index=(y*width+x)*3 ;
			row[x*3]=(pixels[index]);
			row[x*3+1]=(pixels[index+1]);
			row[x*3+2]=(pixels[index+2]);
		}

	}

 }
  return 0;
}



vector<uchar> stretch(Image input,int _height, int _width){//regresa una imagen redimensionada a un tamaño especifico
if(!(input.Height()/input.Width()==_height/_width)){
	vector<uchar>empty(1);
	return empty;
} //Pasar a Tbitmap
bool mode;
input.Channels()==1?mode=false:mode=true;
TBitmap* img=new TBitmap();
get_bitmap(input,mode,img);
TBitmap*out=new TBitmap();
string aux=" Imagen original Width "+to_string(img->Width)+" Height "+to_string(img->Height);
ShowMessage(aux.data());
out->SetSize(_height,_width);
out->Canvas->StretchDraw({0,0,_width,_height},img);
delete img;

TForm2 *newForm=new TForm2(NULL);
	newForm->Image1->Picture->Assign(out);
   newForm->Image1->SetBounds(0,0,out->Width,out->Height);
   newForm->ShowModal();

vector<uchar>output(out->Width*out->Height);
aux="channels "+to_string(input.Channels());
ShowMessage(aux.data());
TRGBTriple *ptr;
if(input.Channels()==1){
	for (int y = 0; y < out->Height; y++) {
		for (int x = 0; x < out->Width; x ++) {
			TColor color = out->Canvas->Pixels[x][y];
			output[y*out->Width+x]=GetRValue(color);          // b&n
		}
	}
}else{
	for (int y = 0; y < _height; y++) {
		uchar* row = (uchar*)out->ScanLine[y];
		for (int x = 0; x < _width * 3; x += 3) {
			output.push_back(row[x]);
			output.push_back(row[x+1]);
			output.push_back(row[x+2]);
		}
	}
}

 delete out;
 return output;
}
