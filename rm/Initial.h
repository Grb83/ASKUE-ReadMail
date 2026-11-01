//---------------------------------------------------------------------------

#ifndef InitialH
#define InitialH
//---------------------------------------------------------------------------
#include <stdio.h>
#include <inifiles.hpp>
#include <FileCtrl.hpp>
#include "kasUtil.h"
#include <XMLDoc.hpp>
#include <vector.h>
//---------------------------------------------------------------------------
struct TPOP3_Connect
{
//     [POP3 Connect]
        AnsiString Host;
        int Port;
        AnsiString UserID;
        AnsiString Password;
        AnsiString Subject;
};

typedef vector<TPOP3_Connect*> TPOP3_ConnectsList;

struct TSelectPath
{
   AnsiString AddressSmall;
   AnsiString AddressFull;
   AnsiString Subject;
   AnsiString Path;
   AnsiString CmdLine;
};

typedef vector<TSelectPath*> TSelectPathsList;

class TInitial
{
protected:
        FILE *f;
        bool ConfigFilePresent;
        AnsiString ConfigFileName;
        bool ReadPOP3ServersList(IXMLNode *RootNode);
        bool ReadPathsList(IXMLNode *RootNode);
        bool GetWriteMessage();
//     [Service]
        AnsiString FServiceName;
        AnsiString FDisplayName;
//      [Connect]
        long FReconnectDelay;
        long FConnectAttemptCount;
        AnsiString FDBName;
//      [File]
        long FSourceScanInterval;
        AnsiString FSaveBodyToFile;
        AnsiString FArchStr;    // Путь к архиватору и параметры затуска
        AnsiString FArchExtensions; // Расширения файлов которые тужно распаковывать
//      [Paths]
        AnsiString FDestinDir;
        AnsiString FCmdLine;
//      [Messages]
        AnsiString FLogFileName;
        AnsiString FEventFileName;
        bool FWriteMessage;
        AnsiString FReportLevel;
        AnsiString __fastcall GetReportLevel();
//      [SaveOriginalMessage]
        bool FSaveOriginal;
        AnsiString FSaveOriginalPath;
public:
        virtual bool __fastcall Read_ini();// Читает параметры из xml файла
//     [POP3 Connectі]
        TPOP3_ConnectsList POP3_Connects;
//     [Service]
        __property AnsiString ServiceName = { read= FServiceName};
        __property AnsiString DisplayName = { read= FDisplayName};
//     [Connect]
        __property long ReconnectDelay  = { read=FReconnectDelay };
        __property long ConnectAttemptCount  = { read=FConnectAttemptCount };
        __property AnsiString DBName  = { read=FDBName };
//     [File]
        __property long SourceScanInterval  = { read=FSourceScanInterval };
        __property AnsiString SaveBodyToFile  = { read=FSaveBodyToFile };
        __property AnsiString ArchStr = { read=FArchStr };
        __property AnsiString ArchExtensions = { read=FArchExtensions };
//      [Path]
        __property AnsiString DestinDir = { read=FDestinDir };
        __property AnsiString CmdLine = { read=FCmdLine };
        TSelectPathsList SelectPaths;
//     [Messages]
        __property AnsiString LogFileName  = { read=FLogFileName };
        __property AnsiString EventFileName  = { read=FEventFileName };
        __property AnsiString ReportLevel = { read=GetReportLevel };
//      [SaveOriginalMessage]
        __property bool SaveOriginal  = { read=FSaveOriginal };
        __property AnsiString SaveOriginalPath  = { read=FSaveOriginalPath };

        bool __fastcall WriteMessage(AnsiString Mess,bool err=true);
        bool __fastcall WriteEventMessage(AnsiString Mess);
        bool __fastcall WriteFullMessage(AnsiString Mess);
        __fastcall TInitial();
        virtual __fastcall ~TInitial();
};
//---------------------------------------------------------------------------
extern PACKAGE TInitial *Initial;
//---------------------------------------------------------------------------

#endif
