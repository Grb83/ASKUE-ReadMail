//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMain.h"
#include "unAbout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#define imFolder  11
#define imLine    10
#define imDetal   26
//---------------------------------------------------------------------------
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormCreate(TObject *Sender)
{
 StringGrid1->ColWidths[0] = 200;
 StringGrid1->ColWidths[1] = 400;
 StringGrid1->Cells[0][0] = "Наименование";
 StringGrid1->Cells[1][0] = "Значение";

 Dict.insert(String_Pair("RMConfig","Конфигурация почтового робота"));
 Dict.insert(String_Pair("Service","Сервис"));
 Dict.insert(String_Pair("ServiceName","Имя сервиса"));
 Dict.insert(String_Pair("DisplayName","Имя для отображения"));
 Dict.insert(String_Pair("Connect","Соединение"));
 Dict.insert(String_Pair("ReconnectDelay","Задержка повторной связи (сек)"));
 Dict.insert(String_Pair("ConnectAttemptCount","Количество попыток связи"));
 Dict.insert(String_Pair("DBName","Имя БД <user/pass@db>"));
 Dict.insert(String_Pair("File","Файл"));
 Dict.insert(String_Pair("SourceScanInterval","Интервал сканирования (сек)"));
 Dict.insert(String_Pair("SaveBodyToFile","Сохранять текст сообщения"));
 Dict.insert(String_Pair("ArchExtensions","Расширения архивных файлов"));
 Dict.insert(String_Pair("ArchStr","Строка запуска архиватора"));
 Dict.insert(String_Pair("Paths","Пути"));
 Dict.insert(String_Pair("Default","Путь по умолчанию"));
 Dict.insert(String_Pair("Path","Путь"));
 Dict.insert(String_Pair("Caption","Описание"));
 Dict.insert(String_Pair("Subject","Тема сообщения"));
 Dict.insert(String_Pair("AddressSmall","Короткий адрес"));
 Dict.insert(String_Pair("AddressFull","Полный адрес"));
 Dict.insert(String_Pair("ExecApp","Запускаемое приложение"));
 Dict.insert(String_Pair("POP3Connects","POP3 соединения"));
 Dict.insert(String_Pair("POP3Connect","POP3 соединение"));
 Dict.insert(String_Pair("Host","Адрес сервер"));
 Dict.insert(String_Pair("Port","Порт"));
 Dict.insert(String_Pair("UserID","Пользователь"));
 Dict.insert(String_Pair("Password","Пароль"));
 Dict.insert(String_Pair("Messages","Журнал сообщений"));
 Dict.insert(String_Pair("WriteMessage","Вести журнал"));
 Dict.insert(String_Pair("LogFileName","Имя файла журнала"));
 Dict.insert(String_Pair("ReportLevel","Уровень журнала"));
 Dict.insert(String_Pair("EventFileName","Журнал событий"));
 Dict.insert(String_Pair("SaveOriginalMessage","Сохранять письма в исходном виде"));
 Dict.insert(String_Pair("SaveOriginal","Сохранять"));
 Dict.insert(String_Pair("SaveOriginalPath","Каталог"));

 SetEnabledAc();
 if (ParamCount() > 0) FName = ParamStr(0);
  else FName = ExtractFilePath(Application->ExeName) + "RM.xml";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormShow(TObject *Sender)
{
  if(FileExists(FName)){
   LoadFile();
   ShowTree();
   Caption = Application->Title + " - " + ExtractFileName(FName);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::SetEnabledAc(void)
{
  bool b=TreeView1->Items->Count;
  acAddPath->Enabled=b;
  acAddPOP3Connect->Enabled=b;
  acSaveFile->Enabled=b;
  acSaveAs->Enabled=b;
  if(!b){
    StringGrid1->Enabled=b;
    acDeletePOP3Connect->Enabled=b;
    acDeletePath->Enabled=b;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::ShowTree()
{
  AnsiString EnglNodeName;
  AnsiString Name;
  TTreeNode* Root;
  TTreeNode* Parent;
  TTreeNode* Child;
  IXMLNodeList* FirstStepNodes;
  IXMLNodeList* SecondStepNodes;

  TreeView1->Items->Clear();
  SetEnabledAc();
  EnglNodeName = XMLDocument1->DocumentElement->GetLocalName();
  Root = TreeView1->Items->AddFirst(NULL, GetNameFromDict(EnglNodeName));
  Root->Data = (void*)XMLDocument1->DocumentElement;
  Root->ImageIndex = imFolder;
  Root->SelectedIndex = imFolder;
  FirstStepNodes = XMLDocument1->DocumentElement->ChildNodes;
  for(int i = 0; i < FirstStepNodes->GetCount(); i++)
  {
    if (FirstStepNodes->Nodes[i]->NodeType != ntElement) continue;
    EnglNodeName = FirstStepNodes->Nodes[i]->GetLocalName();
    Parent = TreeView1->Items->AddChild(Root, GetNameFromDict(EnglNodeName));
    Parent->Data = (void*)FirstStepNodes->Nodes[i];
    SecondStepNodes = FirstStepNodes->Nodes[i]->ChildNodes;
    if (SecondStepNodes == NULL) continue;
    if (SecondStepNodes->Count == 0)
    {
      Parent->ImageIndex = imLine;
      Parent->SelectedIndex = imLine;
      continue;
    }
    Parent->ImageIndex = imFolder;
    Parent->SelectedIndex = imFolder;
    for(int j = 0; j < SecondStepNodes->Count; j++)
    {
     if (SecondStepNodes->Nodes[j]->NodeType != ntElement) continue;
     EnglNodeName = SecondStepNodes->Nodes[j]->GetLocalName();
     AnsiString s=GetNameFromDict(EnglNodeName);
     if(EnglNodeName=="Path"){
       IXMLNodeList* Attrib = SecondStepNodes->Nodes[j]->GetAttributeNodes();
       if(Attrib->Count&&!Attrib->Nodes[0]->GetNodeValue().IsNull())
        s=Attrib->Nodes[0]->GetNodeValue();
     }
     Child = TreeView1->Items->AddChild(Parent, s);
     Child->Data = (void*)SecondStepNodes->Nodes[j];
     Child->ImageIndex = imDetal;
     Child->SelectedIndex = imDetal;
    }
  }
  TreeView1->Selected=Root;
  TreeView1->FullExpand();
  SetEnabledAc();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::LoadFile()
{
  XMLDocument1->Active = false;
  XMLDocument1->FileName = FName;
  XMLDocument1->Active = true;
  if(AnsiString(XMLDocument1->DocumentElement->GetLocalName()) != "RMConfig"){
    throw Exception("Данный файл не является файлом конфигурации почтового робота");
  }
  Modified = false;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::CreateTemplet()
{
  IXMLNode* RootNode;
  IXMLNode* Node;
  IXMLNode* SubNode;

  XMLDocument1->Active = false;
  FName = "rm.xml";
  XMLDocument1->FileName = "";
  XMLDocument1->Active = true;
  XMLDocument1->Encoding="Windows-1251";
  XMLDocument1->DocumentElement = XMLDocument1->CreateElement("RMConfig","");
  RootNode = XMLDocument1->DocumentElement;

  Node = RootNode->AddChild("Service");
  Node->SetAttribute("ServiceName",AnsiString("RM"));
  Node->SetAttribute("DisplayName",AnsiString("Чтение почты"));

  Node = RootNode->AddChild("Connect");
  Node->SetAttribute("ReconnectDelay",10);
  Node->SetAttribute("ConnectAttemptCount",3);
  Node->SetAttribute("DBName",AnsiString("user/pass@db"));

  Node = RootNode->AddChild("File");
  Node->SetAttribute("SourceScanInterval",30);
  Node->SetAttribute("SaveBodyToFile",AnsiString("Yes"));
  Node->SetAttribute("ArchExtensions",AnsiString("*.rar,*.arj,*.zip"));
  Node->SetAttribute("ArchStr",AnsiString(""));

  Node = RootNode->AddChild("Paths");
  Node->SetAttribute("Default",AnsiString("C:\\"));
  Node->SetAttribute("ExecApp",AnsiString(""));
  AddPathNode(Node);

  Node = RootNode->AddChild("POP3Connects");
  AddPOP3ConnectNode(Node);

  Node = RootNode->AddChild("Messages");
  Node->SetAttribute("WriteMessage",AnsiString("yes"));
  Node->SetAttribute("LogFileName",AnsiString(""));
  Node->SetAttribute("ReportLevel",AnsiString("Full"));
  Node->SetAttribute("EventFileName",AnsiString(""));

  Modified = true;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::TreeView1Change(TObject *Sender, TTreeNode *Node)
{
  AnsiString EnglNodeName;
  AnsiString Name;

  if (Node->Data == NULL) return;
  IXMLNode* XMLNode = (IXMLNode*)Node->Data;
  IXMLNodeList* Attrib = XMLNode->GetAttributeNodes();
  acDeletePath->Enabled = (AnsiString(XMLNode->GetLocalName()) == "Path");
  acDeletePOP3Connect->Enabled = (AnsiString(XMLNode->GetLocalName()) == "POP3Connect");

  for(int i = 0; i < Attrib->Count; i++)
  {
     EnglNodeName = Attrib->Nodes[i]->GetLocalName();
     Name = Dict.operator [](EnglNodeName);
     if (Name == "") Name = EnglNodeName;
     StringGrid1->Cells[0][i+1] = Name;
     if (Attrib->Nodes[i]->GetNodeValue().IsNull())
      StringGrid1->Cells[1][i+1] = "";
     else{
        StringGrid1->Cells[1][i+1] =   XMLNode->GetAttribute(EnglNodeName);
        /*if (EnglNodeName=="DBname")
          ShowMessage("задан параметр 'Имя БД'");*/
     }

  }
  if(Attrib->Count == 0){
     StringGrid1->Cells[0][1] = "";
     StringGrid1->Cells[1][1] = "";
     StringGrid1->RowCount = 2;
   }else
    StringGrid1->RowCount = Attrib->Count + 1;
   StringGrid1->Enabled=Attrib->Count;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::StringGrid1SetEditText(TObject *Sender, int ACol,
      int ARow, const AnsiString Value)
{
  if (!TreeView1->Selected||!TreeView1->Selected->Data)
    return;
  IXMLNode* XMLNode = (IXMLNode*)TreeView1->Selected->Data;
  IXMLNodeList* Attrib = XMLNode->GetAttributeNodes();
  if (ARow > Attrib->Count) return;
   Attrib->Nodes[ARow-1]->SetNodeValue(Value);
  Modified = true;
}
//---------------------------------------------------------------------------
IXMLNode* __fastcall TfmMain::AddPathNode(IXMLNode* Parent)
{
  IXMLNode* SubNode;
  SubNode = Parent->AddChild("Path",0);
  SubNode->SetAttribute("Caption",AnsiString("Путь 1"));
  SubNode->SetAttribute("Subject",AnsiString("*"));
  SubNode->SetAttribute("AddressSmall",AnsiString("*"));
  SubNode->SetAttribute("AddressFull",AnsiString("*"));
  SubNode->SetAttribute("Path",AnsiString("C:\\"));
  SubNode->SetAttribute("ExecApp",AnsiString(""));
  return SubNode;
}
//---------------------------------------------------------------------------
IXMLNode* __fastcall TfmMain::AddPOP3ConnectNode(IXMLNode* Parent)
{
  IXMLNode* SubNode;
  SubNode = Parent->AddChild("POP3Connect");
  SubNode->SetAttribute("Host",AnsiString(""));
  SubNode->SetAttribute("Port",110);
  SubNode->SetAttribute("UserID",AnsiString(""));
  SubNode->SetAttribute("Password",AnsiString(""));
  return SubNode;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfmMain::GetNameFromDict(AnsiString EnglName)
{
    AnsiString Name;
    Name = Dict.operator [](EnglName);
    if (Name == "") return EnglName;
    return Name;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::AddToTree(IXMLNode* ParentNode, IXMLNode* ChildNode)
{
  for(int i = 0; i < TreeView1->Items->Count; i++)
  {
    if (TreeView1->Items->Item[i]->Data != ParentNode) continue;
    AnsiString EnglNodeName = ChildNode->GetLocalName();
    TTreeNode* Child = TreeView1->Items->AddChildObjectFirst(TreeView1->Items->Item[i], GetNameFromDict(EnglNodeName),(void*)ChildNode);
    Child->ImageIndex = imDetal;
    Child->SelectedIndex = imDetal;
    TreeView1->Selected=Child;
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::RemoveNode(AnsiString ParentName)
{
  TTreeNode* CurNode = TreeView1->Selected;
  if (!CurNode||!CurNode->Data)
    return;
  IXMLNode* Node;
  Node = XMLDocument1->DocumentElement->ChildNodes->FindNode(ParentName);
  Node->ChildNodes->Remove((IXMLNode*)CurNode->Data);
  TreeView1->Items->Delete(CurNode);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::TreeView1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Button != mbRight) return;
   Popup->Popup(X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  if (!Modified) return;
  int res = MessageDlg("Файл был изменен. Сохранить изменения?",
            mtConfirmation, TMsgDlgButtons() << mbYes	<< mbNo << mbCancel	, 0);
  switch(res)
  {
    case mrCancel: CanClose = false; break;
    case mrYes: CanClose = false; acSaveFileExecute(this); break;
    case mrNo: CanClose = true; break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acOpenFileExecute(TObject *Sender)
{
  if (!OpenDialog1->Execute()) return;
  FName = OpenDialog1->FileName;
  LoadFile();
  ShowTree();
  Caption = Application->Title + " - " + ExtractFileName(FName);
  Modified = false;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acNewFileExecute(TObject *Sender)
{
  CreateTemplet();
  ShowTree();
  Caption = Application->Title + " - " + ExtractFileName(FName);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acSaveFileExecute(TObject *Sender)
{
  if (FName == "Untitled.xml")
    acSaveFileExecute(this);
  else{
    XMLDocument1->SaveToFile(FName);
    Modified = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acSaveAsExecute(TObject *Sender)
{
  if(!SaveDialog1->Execute()) return;
  XMLDocument1->SaveToFile(SaveDialog1->FileName);
  FName = SaveDialog1->FileName;
  Caption = Application->Title + " - " + ExtractFileName(FName);
  Modified = false;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acAddPathExecute(TObject *Sender)
{
  if(!XMLDocument1->Active) return;
  IXMLNode* Node;
  Node = XMLDocument1->DocumentElement->ChildNodes->FindNode("Paths");
  if (!Node) return;
  IXMLNode* ChildNode = AddPathNode(Node);
  AddToTree(Node, ChildNode);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acDeletePathExecute(TObject *Sender)
{
  RemoveNode("Paths");
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acAddPOP3ConnectExecute(TObject *Sender)
{
  if(!XMLDocument1->Active) return;
  IXMLNode* Node;
  Node = XMLDocument1->DocumentElement->ChildNodes->FindNode("POP3Connects");
  if (!Node) return;
  IXMLNode* ChildNode = AddPOP3ConnectNode(Node);
  AddToTree(Node, ChildNode);
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::acDeletePOP3ConnectExecute(TObject *Sender)
{
  RemoveNode("POP3Connects");
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::TreeView1KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Shift.Contains(ssCtrl))
    return;
  switch(Key){
   case 46: //delete
    if (acDeletePath->Enabled)
     {
       acDeletePathExecute(this);
       return;
     }
    if (acDeletePOP3Connect->Enabled)
     {
       acDeletePOP3ConnectExecute(this);
       return;
     }
    break;
   case 45: //insert
    if (!TreeView1->Selected||!TreeView1->Selected->Data)
      return;
    IXMLNode* XMLNode = (IXMLNode*)TreeView1->Selected->Data;
    AnsiString NodeName = XMLNode->GetLocalName();
    if ((NodeName == "Paths") || (NodeName == "Path"))acAddPathExecute(this);
    if ((NodeName == "POP3Connects") || (NodeName == "POP3Connect")) acAddPOP3ConnectExecute(this);
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::AboutClick(TObject *Sender)
{
  fmAbout = new TfmAbout(this);
  fmAbout->ShowModal();
  delete fmAbout;
}
//---------------------------------------------------------------------------

