//---------------------------------------------------------------------------

#include <vcl.h>
#include  <IniFiles.hpp>
#pragma hdrstop

#include "ThreadUnit.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

/*! Внутренний класс
*/
class MailServiceThreadPrivate
{
public:
  /*! Стартовая инициализация внутренних параметров потока
  */
  MailServiceThreadPrivate();
  /*! Остановка процесса если он запущен
  */
  ~MailServiceThreadPrivate()
  { if (isProcess())
      killProcess();
  };
  /*  Запуск процесса ести такового нет
  */
  void createProcess();
  /*  Остановка процесса ести таковой существует
  */
  void killProcess();
  /*  Очистка указателей процесса
  */
  void clearProcess();
  /*  Проверка существования процесса
  */
  bool isProcess()
  { return  si.cb;
  }

  bool  iswait;           //! Признак ожидания завершения работы процесса
  long  timewait;         //! Врем ожидания зависшего приложения (в мсек)
  long  timerun;          //! Интервал запуска приложения (в мсек)
  long  timedelay;        //! Интервал сна процесса - для проверки внешних команд (в мсек)
  long  timecount;        //! Счетчик времени для опредлеления необходимости того или иного действия

	STARTUPINFO         si; //! Информация для загрузки
	PROCESS_INFORMATION pi; //! Информация о процессе
  AnsiString cline;       //! Командная строка
};
//---------------------------------------------------------------------------
MailServiceThreadPrivate::MailServiceThreadPrivate()
{ ZeroMemory(&si, sizeof(si));
  cline = "";
  timewait  = 0;    //время ожидания зависшего приложения - 5 минут
  timerun   = 0;    //интервал запуска приложения - 5 минут
  timedelay = 0;    //интервал сна потока
  try
  { TMemIniFile *ini  = new TMemIniFile(ChangeFileExt(ParamStr(0), ".Ini"));
    try
    { timerun   = 60000 * ini->ReadInteger("PARAM", "timerun", 5);
      timewait  = 60000 * ini->ReadInteger("PARAM", "timewait", 5);
      timedelay = ini->ReadInteger("PARAM", "timedelay", 200);
      cline     = ini->ReadString("PARAM", "cline", ExtractFilePath(ParamStr(0)) + "ReadMail.exe /c").Trim();
    }
    __finally
    { delete  ini;
      ini = NULL;
    }
  }
  catch (...)
  { cline     = ExtractFilePath(ParamStr(0)) + "ReadMail.exe /c";
    timewait  = 5 * 60000;
    timerun   = 5 * 60000;
    timedelay = 200;
  }
  timecount = 0;            //счетчик милисекунд
  iswait    = false;        //признак нахождения в режиме ожидания
}
//---------------------------------------------------------------------------
void MailServiceThreadPrivate::createProcess()
{ if (isProcess())
  { return;
  }
  try
  { ZeroMemory(&si, sizeof(si));
    //ALL_TIME          = SAA_RUN;
    si.cb             = sizeof(si);
    si.dwFlags        = STARTF_USESHOWWINDOW | STARTF_USEPOSITION | STARTF_USECOUNTCHARS;
    si.dwX            = 0;
    si.dwY            = 0;
    si.dwXCountChars  = 32;
    si.dwYCountChars  = 4;
    si.wShowWindow    = SW_HIDE;
    si.lpTitle        = "Обновление версии с главного сервера";
    // Start the child process.
    if (!CreateProcess( NULL, // No module name (use command line).
                        cline.c_str(), // Command line.
                        NULL,             // Process handle not inheritable.
                        NULL,             // Thread handle not inheritable.
                        FALSE,            // Set handle inheritance to FALSE.
                        0,                // No creation flags.
                        NULL,             // Use parent's environment block.
                        NULL,             // Use parent's starting directory.
                        &si,              // Pointer to STARTUPINFO structure.
                        &pi )             // Pointer to PROCESS_INFORMATION structure.
        )
    { ZeroMemory(&si, sizeof(si));
    }
  }
  catch (...)
  {
  }
}
//---------------------------------------------------------------------------
void MailServiceThreadPrivate::killProcess()
{ try
  { if(isProcess())
    { TerminateProcess(pi.hProcess, 0);
      clearProcess();
    }
  }
  catch (...)
  { clearProcess();
  }
}
//---------------------------------------------------------------------------
void MailServiceThreadPrivate::clearProcess()
{ try
  { CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    ZeroMemory(&si, sizeof(si));
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

/*! Конструктор потока

    Создание внутреннего объекта потока
*/
__fastcall MailServiceThread::MailServiceThread(bool CreateSuspended)
  : TThread(CreateSuspended)
{
  Priority  = tpLower;
  d = new MailServiceThreadPrivate();
  FreeOnTerminate = false;
}
//---------------------------------------------------------------------------

/*! Деструктор потока

    Удаление внутреннего объекта потока
*/
__fastcall MailServiceThread::~MailServiceThread()
{
  delete  d;
  d = NULL;
}
//---------------------------------------------------------------------------

/*! Процедура остановки(убития) процесса
*/
void __fastcall MailServiceThread::KillProcess()
{
  d->killProcess();
}

//---------------------------------------------------------------------------

/*! Основная рабочая процедура потока 
*/
void __fastcall MailServiceThread::Execute()
{
  //---- Place thread code here ----
  d->timecount  = d->timerun + d->timedelay;
  while (!Terminated)
  { //===== Проверка таймер на необходимость запуска процесса
    //===== Если идет ожидание остановки, то проверяется с интервалом ожидани
    //===== Иначе с интервалом запуска 
    if (d->timecount > (d->iswait ? d->timewait : d->timerun))
    { //===== если надо запускать
      d->timecount  = 0;
      try
      { if (d->isProcess())
        { //===== Если процесс запущен - получаем его статус
          if (WaitForSingleObject(d->pi.hProcess, 0) == WAIT_OBJECT_0)
          { //===== Отработал - очищаем его инфу и сбрасываем флаг ожидания
            d->clearProcess();
            d->iswait = false;
          }
          else
          { //===== Не отработал - проверяем на ожидание зависжего
            if (d->iswait)
            { //===== Если мы его уже ждем - Убиваем и сбрасываем флаг ожидания
              d->killProcess();
              d->iswait = false;
            }
            else
            { //===== Иначе - ставим флаг ожидания и ждем
              d->iswait = true;
            }
          }
        }
        if (!Terminated && !d->isProcess())
          d->createProcess();
      }
      catch (...)
      { //timecounter = 0;
        //ALL_TIME    = SAA_RUN;
        ZeroMemory(&(d->si), sizeof(d->si));
      }
    }
    if (!Terminated)
    { d->timecount  += d->timedelay; 
      Sleep(d->timedelay);
    }
  }

  //===== Проверка запущенного процесса
  //===== Если он запущен, то производится его удаление
  try
  { if (d->isProcess())
    { if (WaitForSingleObject(d->pi.hProcess, 0) == WAIT_OBJECT_0)
        d->clearProcess();
      else
        d->killProcess();
    }
  }
  catch (...)
  {
  }
}
//---------------------------------------------------------------------------
