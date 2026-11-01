//---------------------------------------------------------------------------
#ifndef MainServH
#define MainServH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <SvcMgr.hpp>
#include <vcl.h>
#include "ReadThread.h"
#include "Initial.h"
//---------------------------------------------------------------------------
class TsMain : public TService
{
__published:    // IDE-managed Components
        void __fastcall ServiceStart(TService *Sender, bool &Started);
        void __fastcall ServiceStop(TService *Sender, bool &Stopped);
        void __fastcall ServiceShutdown(TService *Sender);
        void __fastcall ServiceCreate(TObject *Sender);
        void __fastcall ServiceDestroy(TObject *Sender);
private:        // User declarations
        void __fastcall ReadThreadTerminate(System::TObject* Sender);
        TReadThread* ReadThread;
        bool ThreadExit;
public:         // User declarations
	__fastcall TsMain(TComponent* Owner);
	TServiceController __fastcall GetServiceController(void);

	friend void __stdcall ServiceController(unsigned CtrlCode);
};
//---------------------------------------------------------------------------
extern PACKAGE TsMain *sMain;
//---------------------------------------------------------------------------
#endif
