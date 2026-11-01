//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <map.h>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <oxmldom.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------

typedef map<AnsiString,AnsiString> TDict;
typedef pair <AnsiString,AnsiString> String_Pair;

class TfmMain : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *MainMenu1;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *N3;
  TMenuItem *N4;
  TMenuItem *N5;
  TMenuItem *N6;
  TSaveDialog *SaveDialog1;
  TXMLDocument *XMLDocument1;
  TMenuItem *N7;
  TMenuItem *N8;
  TMenuItem *POP31;
  TMenuItem *N9;
  TMenuItem *POP32;
  TPopupMenu *Popup;
  TMenuItem *N10;
  TOpenDialog *OpenDialog1;
  TActionList *ActionList1;
  TAction *acOpenFile;
  TImageList *ImageList1;
  TPanel *Panel2;
  TTreeView *TreeView1;
  TSplitter *Splitter1;
  TStringGrid *StringGrid1;
  TToolBar *ToolBar1;
  TToolButton *ToolButton1;
  TToolButton *ToolButton3;
  TToolButton *ToolButton4;
  TToolButton *ToolButton5;
  TMenuItem *N12;
  TAction *acNewFile;
  TAction *acSaveFile;
  TAction *acSaveAs;
  TAction *acAddPath;
  TMenuItem *N13;
  TMenuItem *POP35;
  TMenuItem *POP36;
  TAction *acDeletePath;
  TAction *acAddPOP3Connect;
  TAction *acDeletePOP3Connect;
  TToolButton *ToolButton2;
  TToolButton *ToolButton6;
  TToolButton *ToolButton7;
  TToolButton *ToolButton8;
  TMenuItem *N11;
  TMenuItem *N14;
  TMenuItem *N15;
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall TreeView1Change(TObject *Sender, TTreeNode *Node);
  void __fastcall StringGrid1SetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
  void __fastcall TreeView1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall acOpenFileExecute(TObject *Sender);
  void __fastcall acNewFileExecute(TObject *Sender);
  void __fastcall acSaveFileExecute(TObject *Sender);
  void __fastcall acSaveAsExecute(TObject *Sender);
  void __fastcall acAddPathExecute(TObject *Sender);
  void __fastcall acDeletePathExecute(TObject *Sender);
  void __fastcall acAddPOP3ConnectExecute(TObject *Sender);
  void __fastcall acDeletePOP3ConnectExecute(TObject *Sender);
  void __fastcall TreeView1KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall AboutClick(TObject *Sender);
private:	// User declarations
  TDict Dict;
  AnsiString FName;
  void __fastcall LoadFile();
  void __fastcall CreateTemplet();
  void __fastcall ShowTree();
  bool __fastcall Modified;
  IXMLNode* __fastcall AddPOP3ConnectNode(IXMLNode* Parent);
  IXMLNode* __fastcall AddPathNode(IXMLNode* Parent);
  AnsiString __fastcall GetNameFromDict(AnsiString EnglName);
  void __fastcall AddToTree(IXMLNode* ParentNode, IXMLNode* ChildNode);
  void __fastcall RemoveNode(AnsiString ParentName);
  void __fastcall SetEnabledAc(void);
public:		// User declarations
  __fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
