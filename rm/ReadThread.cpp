//---------------------------------------------------------------------------

#include <vcl.h>
#include <StrUtils.hpp>
#include <IdCoderQuotedPrintable.hpp>
#include <IdCoderUUE.hpp>
#include <IdCoderMIME.hpp>
#include <IdCoderXXE.hpp>
#include <stdio.h>
#include <dlk\dlkstring.h>
#pragma hdrstop

#include "ReadThread.h"
#include "Initial.h"
#include "kasUtil.h"
#include "Main.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TReadThread::TReadThread(bool CreateSuspended)
        : TMailThread(CreateSuspended)
{
  Synchronize(ReadInitParam);
  FNoCycle = false;
}
//---------------------------------------------------------------------------
__fastcall TReadThread::~TReadThread()
{
}
//---------------------------------------------------------------------------
void __fastcall TReadThread::Execute()
{
  int ErrCount = 0;
  TPOP3_ConnectsList::const_iterator Iter;
  CoInitialize(NULL);
  try{
  	while(!Terminated){
      int sl=0;
      for(Iter = FPOP3_Connects->begin( ) ; Iter != FPOP3_Connects->end( ); Iter++){
        if(!Terminated&&POP3Connect(*Iter)){
          ErrCount = 0;
          while(!GetMail()&&ErrCount<2){ 
            ErrCount++;
            sl = 0;
            while(sl<10&&!Terminated)
              ::Sleep((unsigned long)1000), sl++;
          }
          POP3Disconnect();
        }
      }
      DeleteTmpFiles();
      sl = 0;
      while(sl<2&&!Terminated){
        ::Sleep((unsigned long)1000);
        sl++;
      }
      WriteFullMessage("****************************************************");
      if(ErrCount>0&&ErrCount<=3&&!Terminated){
        sl = 0;
        while(sl<10&&!Terminated){
          ::Sleep((unsigned long)1000);
          sl++;
        }
      }else{
        if(FNoCycle)
          Terminate();
        ErrCount = 0;
        while(sl<FSourceScanInterval&&!Terminated)
          ::Sleep((unsigned long)1000), sl++;
        }
      }
  }catch(Exception &E){
    WriteMessage("Ошибка запуска нити чтения почты: "+E.Message);
  }
  CoUninitialize();
}
//---------------------------------------------------------------------------
//сравнение строк, аналогично работе ф-ции like БД
//ищет строку 2 в сторке 1
bool Like(AnsiString aMailStr, AnsiString aMaskStr)
{
  if(aMaskStr.Trim()=="*")
    return true;

  std::string mailstr;
  std::string maskstr;
  size_t      pos;
  int         tp  = 0;

  mailstr.assign(aMailStr.c_str());
  maskstr.assign(aMaskStr.c_str());

  stripWhiteSpace(mailstr);
  stripWhiteSpace(maskstr);

  if (0 == mailstr.size() || 0 == maskstr.size())
    return  false;

  stdLower(mailstr);
  stdLower(maskstr);

  if (0 == (pos = maskstr.find('*')))
  { tp  += 2;
    maskstr.erase(pos, 1);
  }
  if (std::string::npos != (pos = maskstr.find('*')))
  { tp  += 1;
    maskstr.erase(pos, maskstr.size());
  }
  if (tp && 0 == maskstr.size())
    return  true;

  switch (tp)
  { case 3: 
      return  std::string::npos != (pos = mailstr.find(maskstr));
      break;
    case 2: 
      if (std::string::npos != (pos = mailstr.find(maskstr)))
      { return  (mailstr.size() - pos) == maskstr.size();
      }
      break;
    case 1: 
      return  0 == (pos = mailstr.find(maskstr));
      break;
    default:  
      return  mailstr == maskstr;
      break;
  }
  return  false;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TReadThread::GetDestDir(AnsiString AddressSmall, AnsiString AddressFull,
                                   AnsiString Subject, AnsiString& CmdLine)
{
  TSelectPathsList::const_iterator Iter;
  AnsiString s;
  try{
    CmdLine=Initial->CmdLine;
    s=Initial->DestinDir;
    for(Iter = Initial->SelectPaths.begin( ) ; Iter != Initial->SelectPaths.end( ); Iter++){
      if(Like(AddressSmall.UpperCase(),(*Iter)->AddressSmall.UpperCase()) &&
          Like(AddressFull.UpperCase(),(*Iter)->AddressFull.UpperCase()) &&
          Like(Subject.UpperCase(),(*Iter)->Subject.UpperCase())){
        CmdLine = (*Iter)->CmdLine;
        s = (*Iter)->Path;
        break;
      }
    }
  }catch (Exception &E){
    WriteMessage("Ошибка выбора целевого каталога: "+E.Message);
  }
  return (s.LastDelimiter("\\")==s.Length())?s:s+"\\";
}
//---------------------------------------------------------------------------
bool __fastcall TReadThread::GetMail()
{
  AnsiString FileDTName, DestDir, CmdLine;
  int FileCount = 0, CurrMsgAttCount;
  long MsgCount;
  TIdDecoder *Id;
  TStrings *MsgBody = new TStringList();
  TStrings *tmpMsgBody = new TStringList();
  TStrings *MsgAtt = new TStringList();
  TStrings *errstrl=new TStringList();
  TIdMessage *FIdMsg = new TIdMessage(NULL);
  AnsiString AddressSmall, AddressFull, Subject;
  AnsiString tmpFname=ExtractFilePath(Application->ExeName)+"Att.tmp";
  try{
    WriteFullMessage("+++ Чтение почты с почтового сервера");
    MsgCount = POP3_ReadMail->CheckMessages();
    for(int i=1; !Terminated && POP3_ReadMail->Connected() && i<=MsgCount; i++){
      try{
        ::Sleep(20);
        FIdMsg->Clear();
        MsgBody->Clear();
        WriteFullMessage("Чтение письма №"+IntToStr(i));
        errstrl->Clear();
        POP3_ReadMail->RetrieveRaw(i,errstrl);
        errstrl->SaveToFile(tmpFname);
        if(Initial->SaveOriginal){
         FileDTName = "m"+TDateTime::CurrentDateTime().FormatString("yymmddhhnnsszzz")+".msg";
         DestDir=Initial->SaveOriginalPath;
         WriteFullMessage("Сохранение оригинала письма в файл: "+DestDir+FileDTName);
         if(!DirectoryExists(DestDir)&&!ForceDirectories(DestDir))
            throw Exception("Невозможно создать каталог "+DestDir);
         errstrl->SaveToFile(DestDir+FileDTName);
        }
        FIdMsg->LoadFromFile(tmpFname,false);
        AddressFull = FIdMsg->From->Address;
        WriteFullMessage("Прочитано сообщения от "+AddressFull);
        AddressSmall = AddressFull.SubString(1,AddressFull.Pos("@")-1);
        Subject =  ConvertStr(FIdMsg->Subject).Trim();
        DestDir = GetDestDir(AddressSmall,AddressFull,Subject,CmdLine);
        CurrMsgAttCount = 0;
        WriteEventMessage("Письмо от: "+AddressFull+". Тема: "+Subject);
        if(FIdMsg->MessageParts->Count){
          for(int j=0;j<FIdMsg->MessageParts->Count; j++){
            if(dynamic_cast<TIdAttachment*>(FIdMsg->MessageParts->Items[j])){
              if(!DirectoryExists(DestDir)&&!ForceDirectories(DestDir))
                throw Exception("Невозможно создать каталог "+DestDir);
              TIdAttachment *IdA = ((TIdAttachment*)FIdMsg->MessageParts->Items[j]);
              if(IdA->ContentTransfer.LowerCase()=="uuencode"||IdA->ContentTransfer.LowerCase()=="x-uue"){
                WriteFullMessage("Сохранение пристежки во временный файл "+tmpFname);
                DeleteFile(tmpFname);
                IdA->SaveToFile(tmpFname);
                WriteFullMessage("Раскодирование временного файла "+tmpFname);
                MsgAtt->LoadFromFile(tmpFname);
                MsgAtt->Text = UUEncodeToWin1251(MsgAtt->Text);
                FileDTName = GetFName(IdA->FileName);
                WriteFullMessage("1_Сохранение данных файл: "+DestDir+FileDTName);
                MsgAtt->SaveToFile(DestDir+FileDTName);
                if(CmdLine != "")
                  ExecApp(CmdLine,DestDir+FileDTName);
              }else{
                FileDTName = GetFName(IdA->FileName);
                WriteFullMessage("2_Сохранение данных файл: "+DestDir+FileDTName);
                if(FileExists(DestDir+FileDTName)){
                  for(int z=0;z<10;z++){
                    if(!FileExists(DestDir+"a"+IntToStr(z)+"_"+FileDTName)){
                      FileDTName="a"+IntToStr(z)+"_"+FileDTName;
                      WriteFullMessage("2_Выбор нового имени файла: "+DestDir+FileDTName);
                      break;
                    }
                  }
                }
                if(DeleteFile(DestDir+FileDTName))
                  WriteFullMessage("2_Удален предыдущий файл: "+DestDir+FileDTName);

                IdA->SaveToFile(DestDir+FileDTName);
                if(FArchExtensions.LowerCase().Pos(ExtractFileExt(FileDTName).LowerCase())>0&&UnpackFile(DestDir+FileDTName)){
                  WriteFullMessage("Удаление архивного файла "+DestDir+FileDTName);
                  if(!DeleteFile(DestDir+FileDTName))
                    WriteMessage("Ошибка при удалении архивного файла "+DestDir+FileDTName,0);
                }
              }
              FileCount++;
              CurrMsgAttCount++;
            }
            if(dynamic_cast<TIdText*>(FIdMsg->MessageParts->Items[j])){
              TIdText* IdT = (TIdText*)FIdMsg->MessageParts->Items[j];
              AnsiString MsgCT = IdT->ContentType.LowerCase();
              if((MsgCT.Pos("text/plain")>0||MsgCT.IsEmpty())&&!IdT->Body->Text.IsEmpty()){
                MsgBody->Text = IdT->Body->Text;
                if(MsgCT.Pos("koi8-r")>0)
                  MsgBody->Text=Koi8rToWin1251(MsgBody->Text);
              }
            }
          }
        }else{
          MsgBody->Text=FIdMsg->Body->Text;
          if(FIdMsg->ContentTransferEncoding.LowerCase()=="quoted-printable"|| 
             FIdMsg->ContentTransferEncoding.LowerCase()=="uuencode"||
             FIdMsg->ContentTransferEncoding.LowerCase()=="x-uue"){
            if(FIdMsg->ContentTransferEncoding.LowerCase()=="quoted-printable"){
              Id=new TIdDecoderQuotedPrintable(NULL);
              MsgBody->Text=Id->DecodeToString(MsgBody->Text);
              delete Id;
              MsgBody->Text=OEMToWin1251(MsgBody->Text);
            }
            if(FIdMsg->ContentTransferEncoding.LowerCase()=="uuencode"||
               FIdMsg->ContentTransferEncoding.LowerCase()=="x-uue"){
              int z = 0;
              bool BeginFind = false;
              for(;z<MsgBody->Count; z++){
                if(MsgBody->Strings[z].Pos("begin")){
                  BeginFind = true;
                  break;
                }
              }
              if(BeginFind){
                tmpMsgBody->Clear();
                z++;
                for(;z<MsgBody->Count; z++){
                  if(MsgBody->Strings[z].Pos("end")){
                    MsgBody->Text = UUEncodeToWin1251(tmpMsgBody->Text);
                    break;
                  }
                  tmpMsgBody->Add(MsgBody->Strings[i]);
                }
              }
            }
          }else{
            if(FIdMsg->CharSet.LowerCase()=="koi8-r")
              MsgBody->Text=Koi8rToWin1251(MsgBody->Text);
          }
        }
        if(FSaveBodyToFile=="yes"&&CurrMsgAttCount==0){
          if(!MsgBody->Text.IsEmpty()){
            if(!DirectoryExists(DestDir)&&!ForceDirectories(DestDir))
              throw Exception("Невозможно создать каталог "+DestDir);
            FileDTName = GetFName("");
            WriteFullMessage("3_Сохранение данных файл: "+DestDir+FileDTName);
            MsgBody->SaveToFile(DestDir+FileDTName);
            if(CmdLine != "")
              ExecApp(CmdLine,DestDir+FileDTName);
          }else{
            WriteFullMessage("Принятое сообщение не содержит данных");
          }
        }
        WriteFullMessage("Удаление письма №"+IntToStr(i));
        POP3_ReadMail->Delete(i);
      }catch(Exception& E){
        WriteMessage("!!! GetMail_1: "+E.Message);
        try{
          DestDir = ExtractFilePath(Application->ExeName)+"ErrMailBox\\";
          if(!DirectoryExists(DestDir)&&!ForceDirectories(DestDir))
            throw Exception("Невозможно создать каталог "+DestDir);
          FileDTName = "!!_Err_"+GetFName("");
          WriteMessage("Сохранение неразобраного письма в файл: "+DestDir+FileDTName,0);
          errstrl->SaveToFile(DestDir+FileDTName);
          WriteFullMessage("Удаление письма №"+IntToStr(i));
          POP3_ReadMail->Delete(i);
        }catch(Exception& E){
          WriteMessage("!!! GetMail_2: "+E.Message,0);
        }
      }
      errstrl->Clear();
    }
    WriteMessage("+++ Завершено чтение почты с почтового сервера. Прочитано: "+IntToStr(MsgCount)+" писем.",0);
  }catch(Exception& E){
    delete MsgBody;
    delete tmpMsgBody;
    delete MsgAtt;
    delete FIdMsg;
    WriteMessage("!!! GetMail_3: "+E.Message);
    return false;
  }
  delete MsgBody;
  delete tmpMsgBody;
  delete MsgAtt;
  delete errstrl;
  delete FIdMsg;
  return true;
}
// --------------------------------------------------------------------------
bool __fastcall TReadThread::ExecApp(AnsiString CmdLine,AnsiString aFileName)
{
  try{
    WriteFullMessage("Запуск приложения "+CmdLine);
    CmdLine=CmdLine+" \""+aFileName+"\"";
    if(WinExec(CmdLine.c_str(),SW_HIDE)>31)
      return true;
    WriteMessage("Ошибка при запуске приложения "+CmdLine);
  }catch(Exception& E){
    WriteMessage("Ошибка запуска приложения: "+E.Message);
  }
  return false;
}
// --------------------------------------------------------------------------
void __fastcall TReadThread::DeleteTmpFiles()
{
  AnsiString TmpDir;
  char c[1024];
  try{
    GetTempPath(sizeof(c),c);
    TmpDir = c;
    WriteFullMessage("*** Удаление временных файлов ***");
    if(GetFilesName(TmpDir, "ind*.*", ".tmp")){
      for(int i=0; i<tmpFilesName->Count&&!Terminated; i++){
        WriteFullMessage("Удаление файла "+TmpDir+tmpFilesName->Strings[i]);
        if(!DeleteFile(TmpDir+tmpFilesName->Strings[i]))
          WriteMessage("Ошибка при удалении файла "+TmpDir+tmpFilesName->Strings[i],0);
      }
    }
    WriteFullMessage("Удаление временных файлов завершено");
  }catch(Exception& E){
    WriteMessage("Ошибка удаления временных файлов: "+E.Message,0);
  }
}
//---------------------------------------------------------------------------
bool __fastcall TReadThread::UnpackFile(AnsiString aFileName)
{
  bool res = true;
  try{
    AnsiString ArchStr = FArchStr+" e -o+ -ibck -ierr \""+aFileName+"\" \""+ExtractFileDir(aFileName)+"\\\"";
    STARTUPINFO Startlnfo = { sizeof(TStartupInfo) };
    PROCESS_INFORMATION ProcInfo;
    Startlnfo.cb = sizeof(Startlnfo);
    Startlnfo.dwFlags = STARTF_USESHOWWINDOW;
    Startlnfo.wShowWindow = SW_HIDE;
    if (! CreateProcess(NULL, ArchStr.c_str(),
                        NULL, NULL, false,
                        CREATE_NEW_CONSOLE | HIGH_PRIORITY_CLASS,
                        NULL, NULL, &Startlnfo, &ProcInfo))
    {
      WriteFullMessage("Ошибка запуска архиватора: "+IntToStr(GetLastError()));
      res = false;
    }
    else
    {
      WriteFullMessage("Распаковка файла "+aFileName);
      if (WaitForSingleObject(ProcInfo.hProcess, 30000) == WAIT_TIMEOUT){
        WriteMessage("За 30 сек. распаковка архива не завершена.");
        res = false;
        TerminateProcess(ProcInfo.hProcess, 1);
      }
      CloseHandle(ProcInfo.hProcess);
    }
  }catch(Exception& E){
      WriteMessage("Ошибка распаковки файла: "+E.Message);
      res = false;
  }
  return res;
}
//---------------------------------------------------------------------------
void __fastcall TReadThread::Write_Message()
{
  Initial->WriteMessage(Mess,MessErr);
}
//---------------------------------------------------------------------------
void __fastcall TReadThread::Write_EMessage()
{
  Initial->WriteEventMessage(EMess);
}
//---------------------------------------------------------------------------
void __fastcall TReadThread::Get_ReportLevel()
{
  FReportLevel = Initial->ReportLevel;
}
// --------------------------------------------------------------------------
void __fastcall TReadThread::ReadInitParam()
{
  try{
    FReconnectDelay = Initial->ReconnectDelay;
    FConnectAttemptCount = Initial->ConnectAttemptCount;
    FDBName = Initial->DBName;
    FSourceScanInterval = Initial->SourceScanInterval;
    FDestinDir = Initial->DestinDir;
    FSaveBodyToFile = Initial->SaveBodyToFile;
    FArchStr = Initial->ArchStr;
    FArchExtensions = Initial->ArchExtensions;
    FPOP3_Connects = &Initial->POP3_Connects;
  }catch(Exception& E){
        WriteMessage("Ошибка чтения параметров: "+E.Message);
  }
}
// --------------------------------------------------------------------------
AnsiString __fastcall TReadThread::GetFName(AnsiString s)
{
  AnsiString s1="\\/:?\"<>|";
  try{
   if(s.IsEmpty())
     return "f"+TDateTime::CurrentDateTime().FormatString("yymmddhhnnsszzz")+".txt";
   s=ConvertStr(s);
   for(int i=0;i<s1.Length();i++){
    if(s.Pos(s1.SubString(i+1,1))>0)
      return "f"+TDateTime::CurrentDateTime().FormatString("yymmddhhnnsszzz")+".txt";
   }
   return s;
  }catch(Exception &E){
    WriteMessage("Ошибка получения имени файла: "+E.Message);
  }
  return "f"+TDateTime::CurrentDateTime().FormatString("yymmddhhnnsszzz")+".txt";
}
// --------------------------------------------------------------------------
AnsiString __fastcall TReadThread::ConvertStr(AnsiString s)
{
  TStrings *ts=new TStringList();
  TIdDecoder *Id=NULL;
  AnsiString scode;
  AnsiString ss=s;
  long ltype=0;
  try{
    WriteFullMessage("Раскодируем: "+s);
    TextSeparate(s,"?",ts,0,0);
    if(ts->Count>4&&ts->Strings[0]=="="){
      if(ts->Strings[2].Trim()=="B"){
        Id=new TIdDecoderMIME(NULL);
        ltype=1;
      }
      if(ts->Strings[2].Trim()=="Q"){
        Id=new TIdDecoderQuotedPrintable(NULL);
        ltype=2;
      }
      if(ltype==1){
        s=ReplaceSubStr(s," ","");
      }
      if(ltype){
        s=ReplaceSubStr(s,ts->Strings[0]+"?"+ts->Strings[1]+"?"+ts->Strings[2]+"?","");
        s=ReplaceSubStr(s,"?=","");
        s=Id->DecodeToString(s);
      }
      scode=ts->Strings[1].Trim().LowerCase();
      if(scode.Pos("koi8-u")>0||scode.Pos("koi8-r")>0)
        s=Koi8rToWin1251(s);
    }

    WriteFullMessage("Результат раскодирования: "+s);
  }catch(Exception &E){
    WriteMessage("Ошибка декодирования: "+E.Message+" Строка:"+ss);
  }
  if(Id)
    delete Id;
  delete ts;
  return s;
}
// --------------------------------------------------------------------------

