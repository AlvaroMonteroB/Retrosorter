//---------------------------------------------------------------------------
#include"lib.h"
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
	}else{ShowMessage("Txt leido");} /*
	bool band=training(filenames,weight_path);
	if(band){
		ShowMessage("Archivo de pesos escrito");
	}else{
		ShowMessage("Error en el entrenamiento");
	}*/
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
	int height=show.Height();
	int width=show.Width();
	bmp->SetSize(width,height);
	bmp->PixelFormat=pf8bit;
	vector<uchar>vect=show.pixel_data();
	for(int y=0; y<height;y++){
		uchar* row=(uchar*)bmp->ScanLine[y];
		for(int x=0;x<width;x++){//width 3024, height 4032
			//
			row[x]=(vect[y*width+x]);
		}

	}
   newForm->Image1->Picture->Assign(bmp);
   newForm->Image1->SetBounds(0,0,width,height);
   newForm->ShowModal();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

