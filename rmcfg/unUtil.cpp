//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "unUtil.h"
//---------------------------------------------------------------------
AnsiString ConvertAccess(const TEdAccesses& access)
{
  char ch[]={'0','0','0','0'};
  if(access.Contains(ac_SELECT))
    ch[0]='1';
  if(access.Contains(ac_INSERT))
    ch[1]='1';
  if(access.Contains(ac_UPDATE))
    ch[2]='1';
  if(access.Contains(ac_DELETE))
    ch[3]='1';
  return AnsiString(ch);
}
//---------------------------------------------------------------------
TEdAccesses ConvertAccess(const AnsiString& S)
{
  TEdAccesses access;
  char ch[]={'0','0','0','0'};
  strcpy(ch,S.c_str());
  if(ch[0]=='1')
    access<<ac_SELECT;
  if(ch[1]=='1')
    access<<ac_INSERT;
  if(ch[2]=='1')
    access<<ac_UPDATE;
  if(ch[3]=='1')
    access<<ac_DELETE;
  return access;
}
//---------------------------------------------------------------------
AnsiString GetTmpPath(void)
{
  LPTSTR lpMsgBuf;
  char tmpdir[256];
  if(!GetTempPath(sizeof(tmpdir),tmpdir)){
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,
      GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL);
    throw Exception(AnsiString(lpMsgBuf));
  }
  AnsiString s=tmpdir;
  if(!DirectoryExists(s)&&!ForceDirectories(s))
    throw Exception("Ошибка получения временного каталога");
  s = (s.LastDelimiter("\\")==s.Length()?s:s+"\\");
  return s;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
