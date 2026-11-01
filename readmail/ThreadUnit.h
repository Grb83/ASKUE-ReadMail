//---------------------------------------------------------------------------

#ifndef ThreadUnitH
#define ThreadUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class MailServiceThreadPrivate;
//---------------------------------------------------------------------------
class MailServiceThread : public TThread
{
private:
  MailServiceThreadPrivate  *d;
protected:
  void __fastcall Execute();
public:
  __fastcall MailServiceThread(bool CreateSuspended);
  __fastcall ~MailServiceThread();

  void __fastcall KillProcess();
};
//---------------------------------------------------------------------------
#endif
