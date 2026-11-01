//---------------------------------------------------------------------------
#ifndef unUtilH
#define unUtilH
//---------------------------------------------------------------------------
enum TEdAccess {ac_SELECT,ac_INSERT,ac_UPDATE,ac_DELETE};
typedef Set<TEdAccess,ac_SELECT,ac_DELETE> TEdAccesses;
AnsiString ConvertAccess(const TEdAccesses& access);
TEdAccesses ConvertAccess(const AnsiString& S);
//---------------------------------------------------------------------------
typedef struct VerInfo
{
  AnsiString vver;
  DWORD HVer;
  DWORD LVer;
}TVerInfo;
//---------------------------------------------------------------------------
typedef struct AccTask
{
  long ntask;
  char name[256];
  long nstart;
  TEdAccesses acc;
}TAccTask;
//---------------------------------------------------------------------------
TVerInfo GetFileVerInfo(AnsiString filename)
{
  TVerInfo VerInf;
  VS_FIXEDFILEINFO **lpBuffer;
  DWORD k;
  unsigned int zz;

  DWORD d=GetFileVersionInfoSize(filename.c_str(),&k);
  LPVOID lpData= new char[d];
  lpBuffer= new VS_FIXEDFILEINFO*[1];
  if((GetFileVersionInfo(filename.c_str(),NULL,d,lpData)&&VerQueryValue(lpData,"\\",(void**)lpBuffer,&zz))){
    VerInf.HVer=lpBuffer[0]->dwFileVersionMS;
    VerInf.LVer=lpBuffer[0]->dwFileVersionLS;
    VerInf.vver=IntToStr(HIWORD(VerInf.HVer))+"."+IntToStr(LOWORD(VerInf.HVer))+"."+
                IntToStr(HIWORD(VerInf.LVer))+"."+IntToStr(LOWORD(VerInf.LVer));
  }else{
    VerInf.HVer=0;
    VerInf.LVer=0;
    VerInf.vver="";
  }
  return VerInf;
}
//---------------------------------------------------------------------------
void InitGlobalVariable(void)
{
  DecimalSeparator='.';
  ShortDateFormat="dd.mm.yyyy";
  DateSeparator='.';
  TimeSeparator=':';
}
//---------------------------------------------------------------------------
AnsiString GetTmpPath(void);
//---------------------------------------------------------------------------
#endif
