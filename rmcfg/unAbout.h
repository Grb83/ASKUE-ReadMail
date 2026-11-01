//---------------------------------------------------------------------------

#ifndef unAboutH
#define unAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfmAbout : public TForm
{
__published:	// IDE-managed Components
  TImage *Image1;
  TLabel *Label1;
  TLabel *lbVersion;
  TLabel *Label2;
  TLabel *Label4;
  void __fastcall Label1Click(TObject *Sender);
  void __fastcall Image1Click(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall Label2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmAbout *fmAbout;
//---------------------------------------------------------------------------
#endif
