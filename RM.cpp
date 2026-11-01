//---------------------------------------------------------------------------
#include <vcl.h>
#include <SysUtils.hpp>
#include <SvcMgr.hpp>
#pragma hdrstop
#include "ReadThread.h"
#include "Initial.h"
USEFORM("rm\Main.cpp", fmMain);
USEFORM("rm\MainServ.cpp", sMain); /* TService: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  if(ParamCount()>0&&(LowerCase(ParamStr(1)) == "/win32")){
    try{
      Forms::Application->Initialize();
      Forms::Application->Title = "Чтение почты";
      Forms::Application->CreateForm(__classid(TfmMain), &fmMain);
      Forms::Application->Run();
    }catch (Exception &exception){
      Forms::Application->ShowException(&exception);
    }
  }else{
    if (ParamCount()>0&&(LowerCase(ParamStr(1)) == "/oc")){
      try{
        Forms::Application->Initialize();
        Forms::Application->Run();
        Initial = new TInitial();
        if(Initial->Read_ini()){
          Initial->WriteFullMessage("Инициализация функции чтения почты");
          TReadThread *ReadThread=new TReadThread(true);
          ReadThread->FNoCycle = true;
          ReadThread->Execute();
        }
      }catch (Exception &exception){
        Forms::Application->ShowException(&exception);
      }
    }else{
      try{
        Svcmgr::Application->Initialize();
        Svcmgr::Application->CreateForm(__classid(TsMain), &sMain);
        Svcmgr::Application->Run();
      }catch (Exception &E){
        Sysutils::ShowException(&E, System::ExceptAddr());
      }
    }
  }
  return 0;
}
//---------------------------------------------------------------------------
/*
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  if(ParamCount()>0&&(LowerCase(ParamStr(1)) == "/win32")){
    try{
      Forms::Application->Initialize();
      Forms::Application->Title = "Чтение почты";
      Forms::Application->CreateForm(__classid(TfmMain), &fmMain);
      Forms::Application->Run();
    }catch (Exception &exception){
      Forms::Application->ShowException(&exception);
    }
  }else{
    if (ParamCount()>0&&(LowerCase(ParamStr(1)) == "/oc")){
      try{
        Forms::Application->Initialize();
        Forms::Application->Run();
        Initial = new TInitial();
        if(Initial->Read_ini()){
          Initial->WriteFullMessage("Инициализация функции чтения почты");
          TReadThread *ReadThread=new TReadThread(true);
          ReadThread->FNoCycle = true;
          ReadThread->Execute();
        }
      }catch (Exception &exception){
        Forms::Application->ShowException(&exception);
      }
    }else{
      try{
        Svcmgr::Application->Initialize();
        Svcmgr::Application->CreateForm(__classid(TsMain), &sMain);
        Svcmgr::Application->Run();
      }catch (Exception &exception){
        Sysutils::ShowException(&exception, System::ExceptAddr());
      }
    }
  }
  return 0;
}
*/
