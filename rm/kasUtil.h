//---------------------------------------------------------------------------
#ifndef kasUtilH
#define kasUtilH
//---------------------------------------------------------------------------
  // “аблица перекодировки Win-1251 в KOI8R
  extern char WinKoi8r_Table[];

  // “аблица перекодировки KOI8R в Win-1251
  extern char Koi8rWin_Table[];

  // ѕереводит строку Win-1251 в KOI8R
  AnsiString Win1251ToKoi8r(AnsiString aValue);

  // ѕереводит строку KOI8R в Win-1251
  AnsiString Koi8rToWin1251(AnsiString aValue);

  // ѕереводит строку Win-1251 в DOS кодировку
  AnsiString Win1251ToOEM(AnsiString aValue);

  // ѕереводит строку DOS в Win-1251 кодировку
  AnsiString OEMToWin1251(AnsiString aValue);

  // ѕереводит строку UUEncode в Win-1251
  AnsiString UUEncodeToWin1251(AnsiString aValue);

  // —равнивает версии exe текущего и того что находитс€ на aUpdatePath
  bool __fastcall CompareVer(AnsiString aUpdatePath);

  // ¬озвращает версию exe "¬ерси€ х.х.х.х"
  AnsiString __fastcall GetVerText();

  // –азбивает текст на строки
  // aTrim - если true то обрезает пробелы в конце и начале каждой подстроки
  // aSkip - если false и между разделител€ми стоит пута€ строка т.е. два разделител€ подр€д то она добавл€етс€ иначе нет
  void __fastcall TextSeparate(AnsiString aSourceString, AnsiString aSeparator, TStrings* aStrings, bool aTrim, bool aSkip);

  // ѕреобразует строку в дату по формату
  TDateTime Str_To_DateTime(AnsiString aStrDateTime, AnsiString aFormat);

  // —читает количество вхождений строки S2 в S1
  long Str_EntryCount_toStr(AnsiString S1, AnsiString S2);

  // »щет в строке aSourse подстроку aOldSubSrt и замен€ет ее на aNewSubSrt
  AnsiString ReplaceSubStr(AnsiString aSourse, AnsiString aOldSubSrt, AnsiString aNewSubSrt);
  // »щет в строке разделенной разделител€ми подстроки
  AnsiString GetStr(AnsiString LongStr,int Poz,AnsiString Razdel="*");
#endif

