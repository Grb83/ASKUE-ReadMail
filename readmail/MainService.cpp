//---------------------------------------------------------------------------
#include "MainService.h"
#include "ThreadUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TReadMail *ReadMail;
//---------------------------------------------------------------------------
/*! Внутренний класс
*/
class TMailServicePrivate
{
public:
  /*  Производится создание потока, для проверки и запуска почтовой слушки ;-)
      Где:
      \param fsuspend - признак спящего потока
      \param priority - приоритет работы потока
  */
  TMailServicePrivate(bool fsuspend, TThreadPriority priority)
  { thread  = new MailServiceThread(fsuspend);
    thread->Priority  = priority;
  }
  /* Производится удаление потока
  */
  ~TMailServicePrivate();

  //! Собственно сам поток
  MailServiceThread *thread;
};
//---------------------------------------------------------------------------
TMailServicePrivate::~TMailServicePrivate()
{ try
  { if (thread)
    { thread->KillProcess();
      thread->Terminate();
      delete  thread;
    }
  }
  catch (...)
  {
  }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/*! Конструктор класса службы
*/
__fastcall TReadMail::TReadMail(TComponent* Owner)
	: TService(Owner)
{
}

TServiceController __fastcall TReadMail::GetServiceController(void)
{
	return (TServiceController) ServiceController;
}

void __stdcall ServiceController(unsigned CtrlCode)
{
	ReadMail->Controller(CtrlCode);
}
//---------------------------------------------------------------------------

/*! Обработка события при создании объекта службы.

    Производится создание внутреннего объекта службы.
*/
void __fastcall TReadMail::ServiceCreate(TObject *Sender)
{
  d = new TMailServicePrivate(true, tpLower);
}
//---------------------------------------------------------------------------

/*! Обработка события перед удалением объекта службы.

    Производится удаление объекта потока
*/
void __fastcall TReadMail::ServiceDestroy(TObject *Sender)
{
  delete  d;
  d = NULL;  
}
//---------------------------------------------------------------------------

/*! Запуск службы после инсталляции/регистрации службы
*/
void __fastcall TReadMail::ServiceAfterInstall(TService *Sender)
{
/*  try
  { StartService(OpenService(OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS),
                              Name.c_str(),
                              SERVICE_START),
                  0,
                  NULL);
  }
  catch (...)
  {
  }
  */
}
//---------------------------------------------------------------------------

/*! Остановка службы перед деинсталяцией/разрегистрацией службы
*/
void __fastcall TReadMail::ServiceBeforeUninstall(TService *Sender)
{
  try
  { SERVICE_STATUS  ServiceStatus;
    ZeroMemory(&ServiceStatus, sizeof(ServiceStatus));
    ControlService(OpenService(OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS),
                                Name.c_str(),
                                SERVICE_STOP),
                    SERVICE_CONTROL_STOP,
                    &ServiceStatus);
  }
  catch (...)
  {
  }
}
//---------------------------------------------------------------------------

/*! Продолжение работы потока при продолжении работы службы
*/
void __fastcall TReadMail::ServiceContinue(TService *Sender,
      bool &Continued)
{
  d->thread->Resume();
  Continued = true;
}
//---------------------------------------------------------------------------

/*! Приостановка работы потока при приостановке работы службы
*/
void __fastcall TReadMail::ServicePause(TService *Sender, bool &Paused)
{
  d->thread->Suspend();
  Paused  = true;
}
//---------------------------------------------------------------------------

/*! Остановка потока при полной остановке службы
*/
void __fastcall TReadMail::ServiceShutdown(TService *Sender)
{
  try
  { if (d->thread)
      d->thread->KillProcess();
  }
  catch (...)
  {
  }
}
//---------------------------------------------------------------------------

/*! Запуск потока при запуске службы
*/
void __fastcall TReadMail::ServiceStart(TService *Sender, bool &Started)
{
  d->thread->Resume();
  Started = true;
}
//---------------------------------------------------------------------------

/*! Остановка потока при остановке службы
*/
void __fastcall TReadMail::ServiceStop(TService *Sender, bool &Stopped)
{
  d->thread->Terminate();
  Stopped = true;
}
//---------------------------------------------------------------------------

