//---------------------------------------------------------------------------

#include <vcl.h>
#include"lib.h"
#include<iostream>
#include<Graphics.hpp>
#pragma hdrstop

#include "Unit1.h"
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLectura *Lectura;
//---------------------------------------------------------------------------
__fastcall TLectura::TLectura(TComponent* Owner)
	: TForm(Owner)
{





}
//---------------------------------------------------------------------------
void __fastcall TLectura::Leer_imagenClick(TObject *Sender)
{

	ShowMessage("Escritura de pesos a "+Weight_path->Text+"\n"+"Archivos de entrenamiento "+Path_String->Text);
	Image img("Name");
	System::AnsiString ansistr=Path_String->Text;//System::AnsiString.c_str()
	string path= ansistr.c_str();
}

//---------------------------------------------------------------------------
