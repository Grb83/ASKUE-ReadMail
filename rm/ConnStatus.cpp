//---------------------------------------------------------------------------
#pragma hdrstop

#include "ConnStatus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Ora"
//---------------------------------------------------------------------------
SetConnStatus::SetConnStatus(AnsiString connString)
{
  OraSession1 = new TOraSession(NULL);
  oqSetStatus = new TOraQuery(NULL);
  oqSetStatus->Session = OraSession1;
  oqSetStatus->SQL->Text = "begin \
  ryda.ADMIN_PG.SET_DIAG_STATE(:AV_SOURCE, :AV_IP_ADDR, :AD_TIME_STAMP, :AD_NEXT_TIME_STAMP,       \
                               :AV_MESSAGE, :AN_STATE, :AV_STATE_NAME, :AN_ERROR, :AV_ERROR_NAME); \
                            end;";
  OraSession1->ConnectString = connString;
  try{
    OraSession1->Connect();
  }
  catch (Exception &e){
    throw Exception("Строка связи задана неверно!");
  }
}
//---------------------------------------------------------------------------
bool SetConnStatus::SetStatus(AnsiString host, AnsiString port, AnsiString conn_state, int state_code)
{
  try{
    oqSetStatus->Close();
    oqSetStatus->ParamByName("AV_SOURCE")->AsString = "Почтовый робот";
    oqSetStatus->ParamByName("AV_IP_ADDR")->AsString = host + ":" + port;
    oqSetStatus->ParamByName("AD_TIME_STAMP")->AsDateTime= Now();
    //oqSetStatus->ParamByName("AD_NEXT_TIME_STAMP")->AsDateTime = NULL;
    oqSetStatus->ParamByName("AV_MESSAGE")->AsString = "";
    oqSetStatus->ParamByName("AN_STATE")->AsInteger = state_code;
    oqSetStatus->ParamByName("AV_STATE_NAME")->AsString = conn_state;
    oqSetStatus->ParamByName("AN_ERROR")->AsInteger = NULL;
    oqSetStatus->ParamByName("AV_ERROR_NAME")->AsString = NULL;
    oqSetStatus->Execute();
    oqSetStatus->Close();
    return true;
  }catch(Exception &e){
    return false;
  }
}
//---------------------------------------------------------------------------
