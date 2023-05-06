//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TLectura : public TForm
{
__published:	// IDE-managed Components
	TLabel *Title1;
	TLabel *Instructions;
	TButton *Leer_imagen;
	TEdit *Path_String;
	TLabel *Directory_message;
	TLabel *Instruction2;
	TEdit *Weight_path;
	void __fastcall Leer_imagenClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TLectura(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLectura *Lectura;
//---------------------------------------------------------------------------
#endif
