//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Trayicon.h"
#include <ImgList.hpp>
#include "ReadThread.h"
#include <Buttons.hpp>
#include "trayicon.h"
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
        TImageList *ImageList1;
        TTrayIcon *TrayIcon1;
        TLabel *Label1;
        TBitBtn *BitBtn1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
        bool ThreadExit;
        TReadThread *ReadThread;
        void __fastcall SendingThreadTerminate(System::TObject* Sender);
public:		// User declarations
        __fastcall TfmMain(TComponent* Owner);
        virtual __fastcall ~TfmMain();
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
