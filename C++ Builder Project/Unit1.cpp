//---------------------------------------------------------------------------

#include <vcl.h>
#include <Vcl.Themes.hpp>
#include <Vcl.Styles.hpp>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
// exported standard C++ interface function that calls into VCL

__declspec(dllexport) void ShowForm(CB_t cb) {
	buttonClicked = cb;
	TStyleManager::SetStyle("Windows10");
	TForm1 *Form2 = new TForm1(NULL);
	Form2->ShowModal();
}
void __fastcall TForm1::Button1Click(TObject *Sender)
{
myLabel->Caption = IntToStr(StrToInt(myLabel->Caption) + 1);
buttonClicked();
}
//---------------------------------------------------------------------------

