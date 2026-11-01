#include <SysUtils.hpp>
#include <SvcMgr.hpp>
#pragma hdrstop
#define Application Svcmgr::Application
USEFORM("readmail\MainService.cpp", ReadMail); /* TService: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try{
    Application->Initialize();
		Application->CreateForm(__classid(TReadMail), &ReadMail);
    Application->Run();
	}catch (Exception &exception){
		Sysutils::ShowException(&exception, System::ExceptAddr());
	}
	return 0;
}
/*
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try{
    Application->Initialize();
		Application->CreateForm(__classid(TReadMail), &ReadMail);
    Application->Run();
	}catch (Exception &exception){
		Sysutils::ShowException(&exception, System::ExceptAddr());
	}
	return 0;
}
*/
