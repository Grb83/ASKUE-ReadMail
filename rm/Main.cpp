//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Initial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Trayicon"
#pragma resource "*.dfm"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

__fastcall TfmMain::~TfmMain()
{
     delete Initial;
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FormCreate(TObject *Sender)
{
    Initial = new TInitial();
    if(Initial->Read_ini())
    {
        Initial->WriteFullMessage("Инициализация функции чтения почты");
	ThreadExit=false;
	ReadThread=new TReadThread(true);
	ReadThread->OnTerminate=SendingThreadTerminate;
        ReadThread->Resume();
    }
    else
      Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::SendingThreadTerminate(System::TObject* Sender)
{
  Initial->WriteMessage("Останов функции чтения почты",0);
	ThreadExit=true;
  ReadThread->~TReadThread();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
        Label1->Visible = true;
	if(ThreadExit)
		CanClose=true;
	else
	{
		ReadThread->Terminate();
		CanClose=false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::BitBtn1Click(TObject *Sender)
{
     Close();
}
//---------------------------------------------------------------------------

