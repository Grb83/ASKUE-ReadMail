//---------------------------------------------------------------------------

#ifndef ConnStatusH
#define ConnStatusH
//---------------------------------------------------------------------------
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Ora.hpp"
#include <DB.hpp>
//---------------------------------------------------------------------------
class SetConnStatus
{
private:
  TOraSession *OraSession1;
  TOraQuery *oqSetStatus;
public:
  SetConnStatus(AnsiString connString);
  bool SetStatus(AnsiString host, AnsiString port, AnsiString conn_state, int state_code);
};
#endif
