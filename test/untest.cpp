//---------------------------------------------------------------------------

#include <vcl.h>
#include <StrUtils.hpp>
#include <IdBaseComponent.hpp>
#include <IdCoder.hpp>
#include <IdCoderQuotedPrintable.hpp>
#include <IdCoderUUE.hpp>
#include <IdCoderMIME.hpp>
#include <IdCoderXXE.hpp>
#pragma hdrstop
#include <IdCoderQuotedPrintable.hpp>
#include <IdCoderUUE.hpp>
#include <IdCoderMIME.hpp>
#include <IdCoderXXE.hpp>
#pragma hdrstop

// Добавляем явные указания линкеру подтянуть реализацию декодеров:
#pragma link "IdCoder.obj"
#pragma link "IdCoder3to4.obj"
#pragma link "IdCoderMIME.obj"

#include "untest.h"
#include "kasUtil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  AnsiString s;
  TIdDecoderQuotedPrintable *IdQuotedPrintableEn;
//  TIdDecoder4to3 *Id;
  TIdDecoder *Id;
  //TIdDecodeTable ADecodeArray;
  AnsiString GBase64CodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  //TIdDecodeTable GBase64DecodeTable;

  Id= new  TIdDecoderMIME(NULL);
//  Id= new  TIdDecoderQuotedPrintable(NULL);
  s="8PLv9O/r7+wgMjktMDktMDYuZG9j";
  s="=CD=C1=CB=C5=D4_=FA=F6=F2=EB.tif";
  s="7c/OydTP0iDQz8vB2sHOycog1dPU0s/K09TXINXexdTBIOHz6/X8Xw==";
  s="7c/OydTP0iDQz8vB2sHOycog1dPU0s/K09TXINXexdTBIOHz6/X8Xw==IPDF3sHU2C5wZGY=";
  //"=?koi8-u?B?7c/OydTP0iDQz8vB2sHOycog1dPU0s/K09TXINXexdTBIOHz6/X8Xw==?= =?koi8-u?B?IPDF3sHU2C5wZGY=?="
  //"=?koi8-r?B?8MzBziDExcvBwtLYICAyMDA2Lnhscw==?="
  //base64

  //quoted-printable

  s=Id->DecodeToString(s);
  s=Koi8rToWin1251(s);
  //s=OEMToWin1251(s);
  //=?koi8-r?B?8MzBziDExcvBwtLYICAyMDA2Lnhscw==?=
  //:00CC5E40 "\x0Fя\fМ\x1Cв\fL\\ј\x1C--‚\x02\x03#\x03\x03bз†З0яя"
  delete Id;
}
//---------------------------------------------------------------------------
