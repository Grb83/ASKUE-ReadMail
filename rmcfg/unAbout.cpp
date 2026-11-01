//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unAbout.h"
#include "unUtil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmAbout *fmAbout;
//---------------------------------------------------------------------------
__fastcall TfmAbout::TfmAbout(TComponent* Owner)
  : TForm(Owner)
{
  Caption=Caption+" "+Application->Title;
}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::Label1Click(TObject *Sender)
{
    ShellExecute(Handle,"open","mailto: office@intep.in.ua",0,0,SW_SHOW);
}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::Image1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::FormCreate(TObject *Sender)
{
  lbVersion->Caption="Версия "+GetFileVerInfo(Application->ExeName).vver;
}
//---------------------------------------------------------------------------

void __fastcall TfmAbout::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==27)
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::Label2Click(TObject *Sender)
{
  ShellExecute(Handle,"open","http://www.intep.in.ua",0,0,SW_SHOW);
}
//---------------------------------------------------------------------------

