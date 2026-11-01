//---------------------------------------------------------------------------
#ifndef MainServiceH
#define MainServiceH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <SvcMgr.hpp>
#include <vcl.h>
//---------------------------------------------------------------------------
class TMailServicePrivate;
//---------------------------------------------------------------------------
class TReadMail : public TService
{
__published:    // IDE-managed Components
  void __fastcall ServiceCreate(TObject *Sender);
  void __fastcall ServiceDestroy(TObject *Sender);
  void __fastcall ServiceAfterInstall(TService *Sender);
  void __fastcall ServiceBeforeUninstall(TService *Sender);
  void __fastcall ServiceContinue(TService *Sender, bool &Continued);
  void __fastcall ServicePause(TService *Sender, bool &Paused);
  void __fastcall ServiceShutdown(TService *Sender);
  void __fastcall ServiceStart(TService *Sender, bool &Started);
  void __fastcall ServiceStop(TService *Sender, bool &Stopped);
private:        // User declarations
  TMailServicePrivate *d;
public:         // User declarations
	__fastcall TReadMail(TComponent* Owner);
	TServiceController __fastcall GetServiceController(void);

	friend void __stdcall ServiceController(unsigned CtrlCode);
};
//---------------------------------------------------------------------------
extern PACKAGE TReadMail *ReadMail;
//---------------------------------------------------------------------------
#endif
