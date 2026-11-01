//---------------------------------------------------------------------------

#ifndef MailThreadH
#define MailThreadH
//---------------------------------------------------------------------------

#include <FileCtrl.hpp>
#include <Classes.hpp>
#include <IdPOP3.hpp>
#include <Initial.h>
#include "ConnStatus.h"
//---------------------------------------------------------------------------
class TMailThread : public TThread
{
protected:
        AnsiString Mess;
        bool MessErr;
        AnsiString EMess;
//     [POP3 Connect]
       TPOP3_ConnectsList* FPOP3_Connects;
//      [Connect]
        long FReconnectDelay;
        long FConnectAttemptCount;
        AnsiString FDBName;
//      [File]
        long FSourceScanInterval;
        AnsiString FDestinDir;
        AnsiString FCreateDir;
        AnsiString FSaveBodyToFile;
        AnsiString FArchStr;    // Путь к архиватору и параметры запуска
        AnsiString FArchExtensions; // Расширения файлов которые тужно распаковывать
//      [Messages]
        AnsiString FReportLevel;
        TStringList *tmpFilesName;
        bool __fastcall POP3Connect(const TPOP3_Connect* POP3_Connect);
        bool __fastcall POP3Disconnect();
        bool __fastcall GetFilesName(AnsiString aDir, AnsiString Mask, AnsiString aFileExtention);

        virtual void __fastcall Get_ReportLevel();
        AnsiString __fastcall GetReportLevel();
        virtual void __fastcall Write_Message();
        virtual void __fastcall Write_EMessage();
        void __fastcall WriteMessage(AnsiString S,bool err=true);
        void __fastcall WriteEventMessage(AnsiString S);
        void __fastcall WriteFullMessage(AnsiString s);

        TIdPOP3 *POP3_ReadMail;
        virtual void __fastcall Execute();

//      --------------------------------------------------

public:
        __fastcall TMailThread(bool CreateSuspended);
        virtual __fastcall ~TMailThread();
};
//---------------------------------------------------------------------------
#endif
