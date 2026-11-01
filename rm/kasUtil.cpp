//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "kasUtil.h"
                                            

//---------------------------------------------------------------------------
#define DEC(c)  (((c) - ' ') & 077)

#pragma package(smart_init)

//---------------------------------------------------------------------------
// Таблица перекодировки Win-1251 в KOI8R
  char WinKoi8r_Table[] = {128,129,130,131,132,133,134,135,136,137,60,139,140,141,142,
                           143,144,145,146,147,148,169,150,151,152,153,154,62,176,157,
                           183,159,160,246,247,74,164,231,166,167,179,169,180,60,172,
                           173,174,183,156,177,73,105,199,181,182,158,163,191,164,62,
                           106,189,190,167,225,226,247,231,228,229,246,250,233,234,235,
                           236,237,238,239,240,242,243,244,245,230,232,227,254,251,253,
                           154,249,248,252,224,241,193,194,215,199,196,197,214,218,201,
                           202,203,204,205,206,207,208,210,211,212,213,198,200,195,222,
                           219,221,223,217,216,220,192,209};

// Таблица перекодировки KOI8R в Win-1251
  char Koi8rWin_Table[] = {128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
                           143,144,145,146,147,148,149,150,151,152,153,218,155,176,157,
                           183,159,160,161,162,184,186,165,166,191,168,169,170,171,172,
                           173,174,175,156,177,178,168,170,181,182,175,184,185,186,187,
                           188,189,190,185,254,224,225,246,228,229,244,227,245,232,233,
                           234,235,236,237,238,239,255,240,241,242,243,230,226,252,251,
                           231,248,253,249,247,250,222,192,193,214,196,197,212,195,213,
                           200,201,202,203,204,205,206,207,223,208,209,210,211,198,194,
                           220,219,199,216,221,217,215,218};

bool __fastcall CompareVer(AnsiString aUpdatePath)
{
  VS_FIXEDFILEINFO *lpBufferThis = NULL, *lpBuffer = NULL;
  LPVOID lpDataThis = NULL, lpData = NULL;
  DWORD k, d, dThis;
  unsigned int zz;
  bool res = true;
    try
    {
          if((d = GetFileVersionInfoSize(AnsiString(aUpdatePath+ExtractFileName(Application->ExeName)).c_str(),&k))==0) return true;
          lpData= new char[d];
          dThis = GetFileVersionInfoSize(Application->ExeName.c_str(),&k);
          lpDataThis= new char[dThis];
          if(GetFileVersionInfo(AnsiString(aUpdatePath+ExtractFileName(Application->ExeName)).c_str(),NULL,d,lpData)&&VerQueryValue(lpData,"\\",(void**)&lpBuffer,&zz)
             &&GetFileVersionInfo(Application->ExeName.c_str(),NULL,dThis,lpDataThis)&&VerQueryValue(lpDataThis,"\\",(void**)&lpBufferThis,&zz))
                if(((lpBuffer->dwFileVersionMS>lpBufferThis->dwFileVersionMS)||(lpBuffer->dwFileVersionLS>lpBufferThis->dwFileVersionLS)))
                  res = false;
     }
     __finally
     {
//         if(lpBufferThis) delete lpBufferThis;      В Win98 вызывает ошибку
//         if(lpBuffer) delete lpBuffer;
         if(lpDataThis) delete[] lpDataThis;
         if(lpData) delete[] lpData;
     }
  return res;
}

//---------------------------------------------------------------------------
AnsiString __fastcall GetVerText()
{
  AnsiString s;
  VS_FIXEDFILEINFO *lpBufferThis = NULL;
  LPVOID lpDataThis = NULL;
  DWORD k, dThis;
  unsigned int zz;

     dThis = GetFileVersionInfoSize(Application->ExeName.c_str(),&k);
     lpDataThis= new char[dThis];
     GetFileVersionInfo(Application->ExeName.c_str(),NULL,dThis,lpDataThis);
     VerQueryValue(lpDataThis,"\\",(void**)&lpBufferThis,&zz);

     s = "Версия "+IntToStr(HIWORD(lpBufferThis-> dwFileVersionMS))+"."+IntToStr(LOWORD(lpBufferThis->dwFileVersionMS))+"."+
              IntToStr(HIWORD(lpBufferThis->dwFileVersionLS))+"."+IntToStr(LOWORD(lpBufferThis->dwFileVersionLS));
     delete[] lpDataThis;
     return s;
}

//---------------------------------------------------------------------------

void __fastcall TextSeparate(AnsiString aSourceString, AnsiString aSeparator, TStrings* aStrings, bool aTrim, bool aSkip)
{
      int i;
      AnsiString In = aSourceString, tmp;
   aStrings->Clear();
   if(aSourceString.Length()>0)
   {
       while(In.Length()>0)
       {
            tmp = "";
            i = 0;
            while(In.Pos(aSeparator)==1)
            {
                 In = In.SubString(aSeparator.Length()+1,In.Length()-aSeparator.Length());
                 if(!aSkip)
                    aStrings->Add("");
                 i++;
            }
            tmp += (In.Pos(aSeparator)>0) ? In.SubString(1,In.Pos(aSeparator)-1) : In;
            if(!tmp.IsEmpty())
                if(aTrim)
                   aStrings->Add(Trim(tmp));
                else
                   aStrings->Add(tmp);
            In = (In.Pos(aSeparator)>0) ? In.SubString(In.Pos(aSeparator)+aSeparator.Length(),In.Length()-In.Pos(aSeparator)+1) : AnsiString("");
       }

   }
}
//---------------------------------------------------------------------------

TDateTime Str_To_DateTime(AnsiString aStrDateTime, AnsiString aFormat)
{
    if(aStrDateTime.Length()!=aFormat.Length())
       throw EConvertError("Неверный формат даты.");
    char OldDateSeparator = DateSeparator, OldTimeSeparator = TimeSeparator;
    AnsiString OldShortDateFormat = ShortDateFormat,
               OldShortTimeFormat = ShortTimeFormat,
               StrYear(""), StrMonth(""), StrDay(""),
               StrHour(""), StrMin(""), StrSec(""),
               StrFormat = aFormat.LowerCase(),
               NewStrDateTime;
    TDateTime ret;
    DateSeparator = '.';
    ShortDateFormat = "dd.mm.yyyy";
    TimeSeparator = ':';
    ShortTimeFormat = "hh:nn:ss";
    try
    {
          try
          {
                for(int i=1; i<aStrDateTime.Length()+1; i++)
                {
                    if(aStrDateTime[i]!=aFormat[i])
                    {
                         switch(StrFormat[i])
                         {
                             case 'd':
                                      StrDay += aStrDateTime[i];
                                      break;
                             case 'm':
                                      StrMonth += aStrDateTime[i];
                                      break;
                             case 'y':
                                      StrYear += aStrDateTime[i];
                                      break;
                             case 'h':
                                      StrHour += aStrDateTime[i];
                                      break;
                             case 'n':
                                      StrMin += aStrDateTime[i];
                                      break;
                             case 's':
                                      StrSec += aStrDateTime[i];
                                      break;
                         }
                    }
                }
                if(StrDay.IsEmpty()) StrDay = "01";
                if(StrMonth.IsEmpty()) StrMonth = "01";
                if(StrYear.IsEmpty()) StrYear = "1900";
                if(StrHour.IsEmpty()) StrHour = "00";
                if(StrMin.IsEmpty()) StrMin = "00";
                if(StrSec.IsEmpty()) StrSec = "00";
                NewStrDateTime = StrDay+"."+StrMonth+"."+StrYear+" "+StrHour+":"+StrMin+":"+StrSec;
                TDateTime dt(NewStrDateTime);
                ret = dt;
          }
          catch(EConvertError &e)
          {
                throw EConvertError(NewStrDateTime+" Неправильный формат даты.");
          }
    }
    __finally
    {
          DateSeparator = OldDateSeparator;
          ShortDateFormat = OldShortDateFormat;
          TimeSeparator = OldTimeSeparator;
          ShortTimeFormat = OldShortTimeFormat;
    }
    return ret;
}
//---------------------------------------------------------------------------
long Str_EntryCount_toStr(AnsiString S1, AnsiString S2)
{
      AnsiString S = S1;
      long res = 0;
              while(!S.IsEmpty())
              {
                    if(S.Pos(S2)>0)
                    {
                       S = S.SubString(S.Pos(S2)+S2.Length(),S.Length()-(S.Pos(S2)+S2.Length())+1);
                       res++;
                    }
                    else
                       S = "";
              }

   return res;
}
//---------------------------------------------------------------------------
AnsiString Win1251ToKoi8r(AnsiString aValue)
{
       int Code;
       AnsiString res("");
    for(int i = 1; i<=aValue.Length(); i++)
    {
        Code = (BYTE)aValue[i];
        if(Code>127)
           res = res+AnsiString(WinKoi8r_Table[Code-128]);
        else
           res = res+AnsiString(aValue[i]);
    }
    return res;
}
//---------------------------------------------------------------------------

AnsiString Koi8rToWin1251(AnsiString aValue)
{
       int Code;
       AnsiString res("");
    for(int i = 1; i<=aValue.Length(); i++)
    {
        Code = (BYTE)aValue[i];
        if(Code>127)
           res = res+AnsiString(Koi8rWin_Table[Code-128]);
        else
           res = res+AnsiString(aValue[i]);
    }
    return res;
}
//---------------------------------------------------------------------------

AnsiString UUEncodeToWin1251(AnsiString aValue)
{
           AnsiString res("");
           TStrings *InputText = new TStringList;
           int n;
           char ch, *p;
        InputText->Text = aValue;
        for(int i=0; i<InputText->Count; i++)
        {
              p = InputText->Strings[i].c_str();
              n = DEC (*p);
              for (++p; n > 0; p += 4, n -= 3)
              {
                  if (n >= 3)
                  {
                        ch = DEC (p[0]) << 2 | DEC (p[1]) >> 4;
                        res += ch;
                        ch = DEC (p[1]) << 4 | DEC (p[2]) >> 2;
                        res += ch;
                        ch = DEC (p[2]) << 6 | DEC (p[3]);
                        res += ch;
                  }
                  else
                  {
                        if (n >= 1)
                        {
                            ch = DEC (p[0]) << 2 | DEC (p[1]) >> 4;
                            res += ch;
                        }
                        if (n >= 2)
                        {
                            ch = DEC (p[1]) << 4 | DEC (p[2]) >> 2;
                            res += ch;
                        }
                  }
              }
        }
        if(InputText) delete InputText;
    return res;
}
//---------------------------------------------------------------------------
AnsiString Win1251ToOEM(AnsiString aValue)
{
       AnsiString res("");
       char *dest = new char[aValue.Length()+1];
       ZeroMemory(dest,aValue.Length()+1);
    if(!CharToOemBuff(aValue.c_str(),dest,aValue.Length()))
       throw Exception("Ошибка при конвертации в OEM");
    res = dest;
    return res;
}
//---------------------------------------------------------------------------
AnsiString OEMToWin1251(AnsiString aValue)
{
       AnsiString res("");
       char *dest = new char[aValue.Length()+1];
       ZeroMemory(dest,aValue.Length()+1);
    if(!OemToCharBuff(aValue.c_str(),dest,aValue.Length()))
       throw Exception("Ошибка при конвертации в Win1251");
    res = dest;
    return res;
}
//---------------------------------------------------------------------------
AnsiString ReplaceSubStr(AnsiString aSourse, AnsiString aOldSubSrt, AnsiString aNewSubSrt)
{
       AnsiString res("");
       TStrings* SepStrings = new TStringList();
       long OldEntry, NewEntry;
    OldEntry = Str_EntryCount_toStr(aSourse, aOldSubSrt);
    NewEntry = Str_EntryCount_toStr(aSourse, aNewSubSrt);
    TextSeparate(aSourse, aOldSubSrt, SepStrings, false, false);
    SepStrings->Text;
    for(int i=0; i<SepStrings->Count; i++)
       res = res+SepStrings->Strings[i]+(i<SepStrings->Count-1 ? aNewSubSrt:AnsiString(""));
    if(SepStrings) delete SepStrings;
    if(OldEntry>Str_EntryCount_toStr(res, aNewSubSrt)-NewEntry) // Учтем последнее вхождение которое не учли при сепарации
        res = res+aNewSubSrt;
    return res;
}
//---------------------------------------------------------------------------
AnsiString GetStr(AnsiString LongStr,int Poz,AnsiString Razdel)
{
  Poz--;
  int i = 0;
  if (LongStr.Pos(Razdel)==0){
    if(Poz>0)
      return "";
    return LongStr;
  }
  while(LongStr.Pos(Razdel)){
    if(i==Poz){
	    return LongStr.SubString(1,LongStr.Pos(Razdel)-1);
	  }else
      LongStr = LongStr.SubString(LongStr.Pos(Razdel)+Razdel.Length(),LongStr.Length()-LongStr.Pos(Razdel));
    i++;
  }
  if(Poz>i)
    return "";
  return LongStr;
}
//---------------------------------------------------------------------------


