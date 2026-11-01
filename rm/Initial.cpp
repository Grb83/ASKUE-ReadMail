//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Initial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TInitial *Initial;
//---------------------------------------------------------------------------

__fastcall TInitial::TInitial()
{
}
//---------------------------------------------------------------------------

__fastcall TInitial::~TInitial()
{
     if (!POP3_Connects.empty())
     {
      TPOP3_ConnectsList::const_iterator Iter;
      for (Iter = POP3_Connects.begin( ) ; Iter != POP3_Connects.end( ); Iter++)
      delete (*Iter);
     }
    if (!SelectPaths.empty())
     {
      TSelectPathsList::const_iterator Iter;
      for (Iter = SelectPaths.begin( ) ; Iter != SelectPaths.end( ); Iter++)
      delete (*Iter);
     }

     WriteMessage("Останов",0);
}
//---------------------------------------------------------------------------
AnsiString GetNodeAttribute(IXMLNode* iNode, AnsiString AttribName, AnsiString DefaultValue = "")
{
  AnsiString res;
  if (iNode == NULL) return DefaultValue;
  res = iNode->Attributes[AttribName];
  if (res == "") return DefaultValue;
  return res;
}
//---------------------------------------------------------------------------
bool TInitial::ReadPathsList(IXMLNode *RootNode)
{
   IXMLNode *PathsNode;
   IXMLNodeList *PathsList;
   try
    {
       PathsNode = RootNode->ChildNodes->FindNode("Paths");
       if (PathsNode == NULL)
         throw Exception("В файле инициализации не найден узел <Paths>");
      FDestinDir = GetNodeAttribute(PathsNode,"Default");
      if (FDestinDir.IsEmpty())
         throw Exception("В файле инициализации не найден параметр <Paths> Default");
      FCmdLine = GetNodeAttribute(PathsNode,"ExecApp");
      PathsList = PathsNode->ChildNodes;
      if (PathsList->GetCount() == 0) return true;
      for(int i = 0; i < PathsList->GetCount(); i++)
      {
         IXMLNode* PathNode = PathsList->Get(i);
         if (PathNode->NodeType != ntElement) continue;
         TSelectPath* SelectPath = new TSelectPath();
         SelectPath->AddressFull  = GetNodeAttribute(PathNode,"AddressFull","*");
         SelectPath->AddressSmall = GetNodeAttribute(PathNode,"AddressSmall","*");
         SelectPath->Subject      = GetNodeAttribute(PathNode,"Subject","*");
         SelectPath->Path         = GetNodeAttribute(PathNode,"Path");
         SelectPath->CmdLine         = GetNodeAttribute(PathNode,"ExecApp");
         if (SelectPath->Path.IsEmpty()){
            WriteMessage("В файле инициализации не найден параметр <Path> Path");
            delete SelectPath;
            continue;
         }
         SelectPaths.push_back(SelectPath);
      }
    }
  catch(Exception& E)
  {
        WriteMessage("Ошибка разбора файла инициализацииt: "+E.Message);
        return false;
  }
  return true;

}
//---------------------------------------------------------------------------
bool TInitial::ReadPOP3ServersList(IXMLNode *RootNode)
{
   IXMLNode *POP3ConnectsNode;
   IXMLNodeList *POP3ConnectsList;
   try
    {
       POP3ConnectsNode = RootNode->ChildNodes->FindNode("POP3Connects");
       if (POP3ConnectsNode == NULL)
         throw Exception("В файле инициализации не найден узел <POP3Connects>");
       POP3ConnectsList =  POP3ConnectsNode->ChildNodes;
       if (POP3ConnectsList->GetCount() == 0)
         throw Exception("Указан пустой список почтовых серверов");
       for(int i = 0; i < POP3ConnectsList->GetCount(); i++)
       {
         IXMLNode* POP3ConnectNode = POP3ConnectsList->Get(i);
         if (POP3ConnectNode->NodeType != ntElement) continue;
         TPOP3_Connect* POP3_Connect = new TPOP3_Connect();
         POP3_Connect->Host = POP3ConnectNode->Attributes["Host"];
         POP3_Connect->Port = POP3ConnectNode->Attributes["Port"];
         POP3_Connect->UserID = POP3ConnectNode->Attributes["UserID"];
         POP3_Connect->Password = POP3ConnectNode->Attributes["Password"];
         POP3_Connects.push_back(POP3_Connect);
       }
    }
  catch(Exception& E)
  {
        WriteMessage("ReadPOP3ServersList: "+E.Message);
        return false;
  }
  return true;
}
// --------------------------------------------------------------------------
bool __fastcall TInitial::Read_ini()
{
    _di_IXMLDocument XMLDocument1;
    IXMLNode *MessagesNode;
    IXMLNode *RootNode;
    IXMLNode *ServiceNode;
    IXMLNode *ConnectNode;
    IXMLNode *FileNode;
    IXMLNode *SaveOriginalMessageNode;
    AnsiString str;

    try{
      ConfigFileName = ChangeFileExt( ParamStr(0), ".xml" );
      if (!FileExists(ConfigFileName)){
         ConfigFilePresent = false;
         throw Exception("Файл не найден " + ConfigFileName);
      }
      ConfigFilePresent = true;

       XMLDocument1 = LoadXMLDocument(ConfigFileName);
       if(!XMLDocument1->Active)
         throw Exception("Ошибка чтения файла " + ConfigFileName);

       XMLDocument1->Options = TXMLDocOptions();
       RootNode = XMLDocument1->DocumentElement;
       if (RootNode == NULL)
         throw Exception("Ошибка формата файла " + ConfigFileName);

//     <Messages>
       MessagesNode = RootNode->ChildNodes->FindNode("Messages");
       FLogFileName = GetNodeAttribute(MessagesNode,"LogFileName",ChangeFileExt(Application->ExeName, ".log"));
       FEventFileName = GetNodeAttribute(MessagesNode,"EventFileName",ChangeFileExt(Application->ExeName, ".evt"));
       if(FEventFileName.LowerCase()=="no")
         FEventFileName="";

       WriteFullMessage("**Запуск**");

       str = GetNodeAttribute(MessagesNode,"WriteMessage","no");
       FWriteMessage = str.LowerCase()=="yes" ? true:false;

        str = GetNodeAttribute(MessagesNode, "ReportLevel", "full");
        FReportLevel = str.LowerCase();
        if(!(FReportLevel=="full")&&!(FReportLevel=="compact"))
         WriteMessage("В файле инициализации неверно указан параметр <Messages> ReportLevel {Compact, Full}");

//      <Service>
        ServiceNode = RootNode->ChildNodes->FindNode("Service");
        if(ServiceNode == NULL)
           throw Exception("В файле инициализации не найден узел <Service>");

        FServiceName = GetNodeAttribute(ServiceNode, "ServiceName");
        if(FServiceName.IsEmpty())
           throw Exception("В файле инициализации не найден параметр <Service> ServiceName");

        FDisplayName = GetNodeAttribute(ServiceNode, "DisplayName");
        if(FDisplayName.IsEmpty())
           throw Exception("В файле инициализации не найден параметр <Service> DisplayName");

//      <Connect>
        ConnectNode = RootNode->ChildNodes->FindNode("Connect");
        if (ConnectNode == NULL)
           throw Exception("В файле инициализации не найден узел <Connect>");

        str = GetNodeAttribute(ConnectNode, "ReconnectDelay");
        FReconnectDelay = StrToIntDef(str,-1);
        if(FReconnectDelay < 0)
           throw Exception("В файле инициализации не найден параметр <Connect> ReconnectDelay, или он < 0 ");

        str = GetNodeAttribute(ConnectNode, "ConnectAttemptCount");
        FConnectAttemptCount = StrToIntDef(str,-1);
        if(FConnectAttemptCount < 1)
           throw Exception("В файле инициализации не найден параметр <Connect> ConnectAttemptCount, или он < 1 ");

        FDBName = GetNodeAttribute(ConnectNode, "DBName");
        if(FDBName.IsEmpty())
  //         throw Exception("В файле инициализации не найден параметр <Connect> DBName");
           WriteMessage("В файле инициализации не найден параметр <Connect> DBName");

//      <File>
        FileNode = RootNode->ChildNodes->FindNode("File");
        str = GetNodeAttribute(FileNode, "SourceScanInterval");
        FSourceScanInterval = StrToIntDef(str,-1);
        if(FSourceScanInterval < 1)
           throw Exception("В файле инициализации не найден параметр <File> SourceScanInterval, или он < 1 ");

        str = GetNodeAttribute(FileNode, "SaveBodyToFile","yes");
        FSaveBodyToFile = str.LowerCase();

        str = GetNodeAttribute(FileNode, "ArchStr");
        FArchStr = str.LowerCase();
        if(!FArchStr.Trim().IsEmpty())
        {
                if(!(FArchStr.Pos(".exe")>0))
                {
                   WriteMessage("В файле инициализации неверно задан параметр [File] FArchStr. Не найдена подстрока с указанием архиватора. Архиватор будет запущен из PATH");
                   FArchStr = "WinRar";
                }
                else
                {
                    FArchStr = FArchStr.SubString(1,FArchStr.Pos(".exe")+3);
                    if(!FileExists(FArchStr))
                    {
                      WriteMessage("Не найден архиватор "+FArchStr+". Архиватор будет запущен из PATH");
                      FArchStr = "WinRar";
                    }
                    else
                      FArchStr = "\""+FArchStr+"\"";
                }
        }
        else
        {
          FArchStr = "WinRar";
          WriteMessage("В файле инициализации не задан параметр [File] FArchStr. Архиватор будет запущен из PATH");
        }

       str = GetNodeAttribute(FileNode, "ArchExtensions");
       FArchExtensions = str.LowerCase();

      if(!ReadPOP3ServersList(RootNode))
        throw Exception("Ошибка чтения списка почтовых серверов");
     if(!ReadPathsList(RootNode))
        throw Exception("Ошибка чтения списка путей сохранения сообщений");

     //      <SaveOriginalMessage>
        SaveOriginalMessageNode = RootNode->ChildNodes->FindNode("SaveOriginalMessage");
        if (SaveOriginalMessageNode == NULL){
          FSaveOriginal = false;
        }
        str = GetNodeAttribute(SaveOriginalMessageNode,"SaveOriginal","no");
        FSaveOriginal = str.LowerCase()=="yes" ? true:false;
        FSaveOriginalPath = GetNodeAttribute(SaveOriginalMessageNode,"SaveOriginalPath","C:\\");
        FSaveOriginalPath =(FSaveOriginalPath.LastDelimiter("\\")==FSaveOriginalPath.Length())?FSaveOriginalPath:FSaveOriginalPath+"\\";
    }catch (Exception &E){
      	WriteMessage("Read_ini: "+E.Message);
        return false;
    }
   return true;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TInitial::GetReportLevel()
{
    _di_IXMLDocument XMLDocument1;
    if (!ConfigFilePresent) return FReportLevel;
    try
    {
       XMLDocument1 = LoadXMLDocument(ConfigFileName);
       if(!XMLDocument1->Active) return FReportLevel;
       XMLDocument1->Options = TXMLDocOptions();
       AnsiString res = GetNodeAttribute(XMLDocument1->DocumentElement->ChildNodes->FindNode("Messages"),
                                         "ReportLevel","full");
       FReportLevel = res.LowerCase();
    }
   catch (Exception &E)
    {
      	WriteMessage("Ошибка получения уровня ведения журнала: "+E.Message);
    }

   return FReportLevel;
}
//---------------------------------------------------------------------------
bool TInitial::GetWriteMessage()
{
 _di_IXMLDocument XMLDocument1;
  if (!ConfigFilePresent) return FWriteMessage;
    try
    {
       XMLDocument1 = LoadXMLDocument(ConfigFileName);
       if(!XMLDocument1->Active) return FWriteMessage;
       XMLDocument1->Options = TXMLDocOptions();
       AnsiString res = GetNodeAttribute(XMLDocument1->DocumentElement->ChildNodes->FindNode("Messages"),
                                         "WriteMessage","no");
       FWriteMessage = res.LowerCase()=="yes" ? true:false;
    }
   catch (Exception &E)
    {
      	WriteMessage("TInitial::GetReportLevel: "+E.Message);
    }
    return FWriteMessage;
}
//---------------------------------------------------------------------------
bool __fastcall TInitial::WriteMessage(AnsiString Mess,bool err)
{
  GetWriteMessage();
  try{
    if(err){
      AnsiString s=ExtractFilePath(Application->ExeName)+"rmerror.bat";
      if(FileExists(s)){
        s="\""+s+"\" \""+Mess+"\"";
        WinExec(s.c_str(),SW_HIDE);
      }
    }
    if(FWriteMessage){
      unsigned short year, month, day;
      unsigned short hour, min, sec, msec;
      TDateTime DT = TDateTime::CurrentDateTime();
      DT.DecodeDate(&year, &month, &day);
      DT.DecodeTime(&hour, &min, &sec, &msec);
      f = fopen(FLogFileName.c_str(), "at");
      fprintf(f, " %02hu.%02hu.%02hu  %02hu:%02hu:%02hu --> %s\n" ,day, month, year, hour, min, sec, Mess.c_str());
      fclose(f);
    }
  }catch(Exception &e){
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TInitial::WriteFullMessage(AnsiString Mess)
{
  if(GetReportLevel()=="full")
    return WriteMessage(Mess,0);
  else
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TInitial::WriteEventMessage(AnsiString Mess)
{
  try{
    if(!EventFileName.IsEmpty()){
      unsigned short year, month, day;
      unsigned short hour, min, sec, msec;
      TDateTime DT = TDateTime::CurrentDateTime();
      DT.DecodeDate(&year, &month, &day);
      DT.DecodeTime(&hour, &min, &sec, &msec);
      f = fopen(EventFileName.c_str(), "at");
      fprintf(f, " %02hu.%02hu.%02hu  %02hu:%02hu:%02hu --> %s\n" ,day, month, year, hour, min, sec, Mess.c_str());
      fclose(f);
    }
  }catch(Exception &e){
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------



