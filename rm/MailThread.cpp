//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MailThread.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TMailThread::TMailThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
      tmpFilesName = new TStringList();
//      FIdEMailAddressList = new TIdEMailAddressList(NULL);
//      FIdEMailAddressList->Add();
}
//---------------------------------------------------------------------------

__fastcall TMailThread::~TMailThread()
{
       if(tmpFilesName) delete tmpFilesName;
//       if(FIdEMailAddressList) FIdEMailAddressList;
}
//---------------------------------------------------------------------------

void __fastcall TMailThread::Execute()
{
}
//---------------------------------------------------------------------------

bool __fastcall TMailThread::POP3Connect(const TPOP3_Connect* POP3_Connect)
{
             bool ConnectedComplete = false;
             int ConnectedCount = 0;
             AnsiString errConnect;
      POP3_ReadMail = new TIdPOP3(NULL);
//        --------------------------------------------------
      while(!ConnectedComplete&&ConnectedCount<FConnectAttemptCount&&!Terminated){
            try{
                    POP3_ReadMail->Host = POP3_Connect->Host;
                    POP3_ReadMail->Port = POP3_Connect->Port;
                    POP3_ReadMail->Username = POP3_Connect->UserID;
                    POP3_ReadMail->Password = POP3_Connect->Password;
                    WriteFullMessage("Соединение с POP3 сервером");
                    POP3_ReadMail->Connect();
                    ConnectedComplete = POP3_ReadMail->Connected();
            }catch(Exception &E){
                 WriteMessage("TMailThread::POP3Connect: "+E.Message);
                 errConnect = E.Message;
                 ConnectedComplete = false;
            }
            int sl=0;
            while(sl<FReconnectDelay&&!ConnectedComplete&&!Terminated&&FConnectAttemptCount>1)
              Sleep((unsigned long)1000), sl++;
            ConnectedCount++;
      }
   if(!ConnectedComplete&&POP3_ReadMail){
        delete POP3_ReadMail;
        POP3_ReadMail = NULL;
   }
   try{
     if(!FDBName.IsEmpty()){
       SetConnStatus* conn_status = new SetConnStatus(FDBName);
       bool set_res;
       if (ConnectedComplete)
          set_res = conn_status->SetStatus(POP3_Connect->Host, POP3_Connect->Port, "Штатная работа.", 1);
       else
          set_res = conn_status->SetStatus(POP3_Connect->Host, POP3_Connect->Port, errConnect, 3);
       if (conn_status)
         delete conn_status;
       if (!set_res)
         WriteFullMessage("Не удалось записать состояние соединения с POP3");
     }
   }catch(Exception &E){
    WriteMessage("TMailThread::POP3ConnectStatus: "+E.Message);
   }
   return ConnectedComplete;
}
// --------------------------------------------------------------------------
bool __fastcall TMailThread::POP3Disconnect()
{
  try{
    if(POP3_ReadMail->Connected()){
      WriteFullMessage("Отсоединение от POP3 сервера_1");
      POP3_ReadMail->WriteLn("QUIT"); //Нужно для очень быстрых машин
      ::Sleep(1000);
    }
    if(POP3_ReadMail->Connected()){
      WriteFullMessage("Отсоединение от POP3 сервера_2");
      POP3_ReadMail->Disconnect();
    }
    if(POP3_ReadMail)
      delete POP3_ReadMail;
    POP3_ReadMail = NULL;
  }catch(Exception& E){
    WriteMessage("TMailThread::POP3Disconnect: "+E.Message,0);
  }
  return true;
}
// --------------------------------------------------------------------------
void __fastcall TMailThread::Get_ReportLevel()
{
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMailThread::GetReportLevel()
{
        Synchronize(Get_ReportLevel);
        return FReportLevel;
}
//---------------------------------------------------------------------------
void __fastcall TMailThread::Write_Message()
{
}
// --------------------------------------------------------------------------
void __fastcall TMailThread::Write_EMessage()
{
}
// --------------------------------------------------------------------------
void __fastcall TMailThread::WriteMessage(AnsiString S,bool err)
{
        Mess = S;
        MessErr=err;
        Synchronize(Write_Message);
}
// --------------------------------------------------------------------------
void __fastcall TMailThread::WriteEventMessage(AnsiString S)
{
        EMess = S;
        Synchronize(Write_EMessage);
}
// --------------------------------------------------------------------------
void __fastcall TMailThread::WriteFullMessage(AnsiString s)
{
  if(GetReportLevel()=="full")
    WriteMessage(s,0);
}
// --------------------------------------------------------------------------
bool __fastcall TMailThread::GetFilesName(AnsiString aDir, AnsiString Mask, AnsiString aFileExtention)
{
  TSearchRec F;
  try{
     tmpFilesName->Clear();
     WriteFullMessage("Чтение файлов с директории "+aDir);
     if(!DirectoryExists(aDir)) WriteMessage("Не найдена директория "+aDir);
     else
     {
           if(FindFirst(aDir+Mask, faAnyFile, F)==0)
           {
                if(aFileExtention.Pos(ExtractFileExt(F.Name.LowerCase()))>0)
                  tmpFilesName->Add(F.Name);
                while(FindNext(F)==0)
                if(aFileExtention.Pos(ExtractFileExt(F.Name.LowerCase()))>0)
                   tmpFilesName->Add(F.Name);
           }
           FindClose(F);
           tmpFilesName->Sorted = true;
           if(tmpFilesName->Count<1)
           {
              WriteFullMessage("Нет файлов");
           }
           else
              WriteFullMessage("Найдено файлов: "+IntToStr(tmpFilesName->Count));
     }
     return (tmpFilesName->Count>0);
  }
  catch(Exception& E)
  {
        WriteMessage("Ошибка чтения файлов: "+E.Message);
        return false;
  }
}


// --------------------------------------------------------------------------
//  POP3  -------------------------------------------------------------------
// --------------------------------------------------------------------------

