//---------------------------------------------------------------------------
#ifndef kasUtilH
#define kasUtilH
//---------------------------------------------------------------------------
  // Таблица перекодировки Win-1251 в KOI8R
  extern char WinKoi8r_Table[];

  // Таблица перекодировки KOI8R в Win-1251
  extern char Koi8rWin_Table[];

  // Переводит строку Win-1251 в KOI8R
  AnsiString Win1251ToKoi8r(AnsiString aValue);

  // Переводит строку KOI8R в Win-1251
  AnsiString Koi8rToWin1251(AnsiString aValue);

  // Переводит строку Win-1251 в DOS кодировку
  AnsiString Win1251ToOEM(AnsiString aValue);

  // Переводит строку DOS в Win-1251 кодировку
  AnsiString OEMToWin1251(AnsiString aValue);

  // Переводит строку UUEncode в Win-1251
  AnsiString UUEncodeToWin1251(AnsiString aValue);

  // Сравнивает версии exe текущего и того что находится на aUpdatePath
  bool __fastcall CompareVer(AnsiString aUpdatePath);

  // Возвращает версию exe "Версия х.х.х.х"
  AnsiString __fastcall GetVerText();

  // Разбивает текст на строки
  // aTrim - если true то обрезает пробелы в конце и начале каждой подстроки
  // aSkip - если false и между разделителями стоит путая строка т.е. два разделителя подряд то она добавляется иначе нет
  void __fastcall TextSeparate(AnsiString aSourceString, AnsiString aSeparator, TStrings* aStrings, bool aTrim, bool aSkip);

  // Преобразует строку в дату по формату
  TDateTime Str_To_DateTime(AnsiString aStrDateTime, AnsiString aFormat);

  // Считает количество вхождений строки S2 в S1
  long Str_EntryCount_toStr(AnsiString S1, AnsiString S2);

  // Ищет в строке aSourse подстроку aOldSubSrt и заменяет ее на aNewSubSrt
  AnsiString ReplaceSubStr(AnsiString aSourse, AnsiString aOldSubSrt, AnsiString aNewSubSrt);
  // Ищет в строке разделенной разделителями подстроки
  AnsiString GetStr(AnsiString LongStr,int Poz,AnsiString Razdel="*");
#endif

