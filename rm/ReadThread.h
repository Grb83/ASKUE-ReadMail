//---------------------------------------------------------------------------

#ifndef ReadThreadH
#define ReadThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "MailThread.h"
#include <XMLDoc.hpp>
//---------------------------------------------------------------------------
class TReadThread : public TMailThread
{

protected:
        bool __fastcall GetMail();
        int __fastcall CopyFiles();
        void __fastcall DeleteTmpFiles();
        bool __fastcall UnpackFile(AnsiString aFileName);
        bool __fastcall ExecApp(AnsiString CmdLine,AnsiString aFileName);
        void __fastcall ReadInitParam();

        void __fastcall Get_ReportLevel();
        void __fastcall Write_Message();
        void __fastcall Write_EMessage();
        bool TestMailParams(IXMLNode* Node, AnsiString AddressSmall, AnsiString AddressFull,
                            AnsiString Subject, bool useMask);
        AnsiString __fastcall GetDestDir(AnsiString AddressSmall, AnsiString AddressFull,
                                   AnsiString Subject, AnsiString& CmdLine);
        AnsiString __fastcall GetFName(AnsiString s);
        AnsiString __fastcall ConvertStr(AnsiString s);
       // Читает путь сохранения сообщения из файла конфигурации путей
public:
        void __fastcall Execute();
        bool FNoCycle;
        __fastcall TReadThread(bool CreateSuspended);
        virtual __fastcall ~TReadThread();
};
//---------------------------------------------------------------------------
#endif
