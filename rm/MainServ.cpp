//---------------------------------------------------------------------------
#include <vcl.h>
#include "MainServ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TsMain *sMain;
//---------------------------------------------------------------------------
__fastcall TsMain::TsMain(TComponent* Owner)
	: TService(Owner)
{
}

TServiceController __fastcall TsMain::GetServiceController(void)
{
	return (TServiceController) ServiceController;
}

void __stdcall ServiceController(unsigned CtrlCode)
{
	sMain->Controller(CtrlCode);
}
//---------------------------------------------------------------------------
void __fastcall TsMain::ServiceStart(TService *Sender,
      bool &Started)
{
    if(!ThreadExit)
    {
         Initial->WriteFullMessage("Инициализация функции чтения почты");
         ReadThread = new TReadThread(true);
         ReadThread->OnTerminate=ReadThreadTerminate;
         ReadThread->Resume();
         Started = true;
    }
    else
    {
         Started = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TsMain::ServiceStop(TService *Sender,
      bool &Stopped)
{
    if(ThreadExit)
    {
        Stopped=true;
        if(ReadThread)delete ReadThread;
    }
    else
    {
        ReadThread->Terminate();
        int i = 0;
        while(!ThreadExit&&i++<WaitHint/1000)
          Sleep((unsigned long)999);
        Stopped=true;                                     
        if(ReadThread)delete ReadThread;
    }
}
//---------------------------------------------------------------------------
void __fastcall TsMain::ReadThreadTerminate(System::TObject* Sender)
{
  bool Stopped;
  Initial->WriteMessage("Останов функции чтения почты",0);
  ThreadExit=true;
  try
  {
        SERVICE_STATUS ServiceStatus;
        ZeroMemory( &ServiceStatus, sizeof(ServiceStatus));
        ControlService(OpenService(OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS),
                                   Name.c_str(),
                                   SERVICE_STOP),
                       SERVICE_CONTROL_STOP,
                       &ServiceStatus);
  }
  catch (...){}
}
//---------------------------------------------------------------------------


void __fastcall TsMain::ServiceShutdown(TService *Sender)
{
     ReadThread->Suspend();
     if(ReadThread) delete ReadThread;
}
//---------------------------------------------------------------------------


void __fastcall TsMain::ServiceCreate(TObject *Sender)
{
    Initial = new TInitial();
    ThreadExit = !Initial->Read_ini();
    Name = Initial->ServiceName;
    DisplayName = Initial->DisplayName;
}
//---------------------------------------------------------------------------

void __fastcall TsMain::ServiceDestroy(TObject *Sender)
{
     delete Initial;
}
//---------------------------------------------------------------------------



