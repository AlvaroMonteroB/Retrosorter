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
  int get_bitmap(Image input,bool mode,TBitmap *output);
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::processClick(TObject *Sender)
{
ShowMessage("Escritura de pesos a "+weight_data->Text+"\n"+"Archivos de entrenamiento "+txt_path->Text);
	Image img("Name");
	System::AnsiString ansistr=txt_path->Text;//System::AnsiString.c_str()
	string path= ansistr.c_str();//Archivo donde almacenamos las direcciones del train data
	ansistr=weight_data->Text;
	string weight_path=ansistr.c_str();
	filenames=train_path(path);

	if(!filenames.data()){
	ShowMessage("No se pudieron leer los nombres");
	}else{ShowMessage("Txt leido");}

}
//---------------------------------------------------------------------------
 //D:\Repositorios\Inv_proj1\train_path.txt
 //D:\Repositorios\Inv_proj1\weights\weight_data_Camera.txt

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
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

int get_bitmap(Image input,bool mode,TBitmap *output){//true =color; false =b & n
 int height=input.Height();
 int width=input.Width();
 vector<uchar>pixels=input.pixel_data();
 if(!mode){
  output->SetSize(width,height);
  output->PixelFormat=pf8bit;
	for(int y=0; y<height;y++){
		uchar* row=(uchar*)output->ScanLine[y];
		for(int x=0;x<width;x++){//width 3024, height 4032
			//
			row[x]=(pixels[y*width+x]);
		}

	}

 }
  return 0;
}



vector<uchar> stretch(Image input,int _height, int _width){//regresa una imagen redimensionada a un tamaño especifico
if(!(input.Height()/input.Width()==_height/_width)){
	vector<uchar>empty(0);
	return empty;
} //Pasar a Tbitmap
bool mode;
input.Channels()==1?mode=false:mode=true;
TBitmap* img=new TBitmap();
get_bitmap(input,mode,img);
TBitmap*out=new TBitmap();
out->Canvas->StretchDraw({0,0,_width,_height},img);
vector<uchar>output;
for(int i=0;i<_height;i++){
	uchar* row = (uchar*)out->ScanLine[i];
	output.insert(output.end(), row, row + _width);
	}


 return output;
}
