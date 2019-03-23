// MyWinHttpClient.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>
#include <fstream>
#include <locale>

#include <sstream>
#include "src\HttpClient.h"
#include "src\HttpClientHelper.h"

#define Exit(message)								\
	std::wcout << message << std::endl;				\
	std::wcout << _T("Press any key to exit...");	\
	_getch();										\
	return{}

int main()
{
	/*const std::wstring w_str(L"Test Тест");
	std::string s1 = Wstr2Str(w_str);
	std::string s2 = WstringToString(w_str, CP_ACP);

	const std::string c_str("Test Тест");
	std::wstring w1 = Str2Wstr(c_str);
	std::wstring w2 = StringToWstring(c_str, CP_ACP);

	BSTR bstr = ::SysAllocString(L"Test Тест");
	w1 = BSTRToWstring(bstr);
	s1 = BSTRToString(bstr, CP_ACP);

	auto bStr1 = WstringToBSTR(w_str);
	auto bStr2 = StringToBSTR(c_str, CP_ACP);*/

	using namespace http;

    HttpClient client;
    RequestData rqData;

	//rqData.connect.server.assign(_T("www.microsoft.com"));
	/*rqData.connect.server.assign(_T("price.moyo.ua"));
	rqData.createReq.objName.assign(_T("/oplata_chastyami.xml"));*/
    /*rqData.connect.server.assign(_T("www.kmp.ua"));
    rqData.createReq.objName.assign(_T("/wp-json/wp/v2/users/1"));*/
	//rqData.createReq.objName.assign(_T("wp-json/wp/v2/posts")); 
	//rqData.createReq.requestFlags = FLAGS(UNSECURE);
    //rqData._reqHeaders._headers = ;

	std::wcout << _T("Server name: ");
	{
		std::wstring sName{};
		std::wcin >> sName;
		rqData.connect.server.assign(std::move(sName));
	}

	std::wcout << _T("Object name: ");
	{
		std::wstring objName{};
		std::wcin >> objName;
		rqData.createReq.objName.assign(std::move(objName));
	}

	std::wcout << _T("Use a secure connection (y/n)? ");
	{
		wchar_t secure{};
		std::wcin >> secure;
		rqData.createReq.requestFlags = ('y' == secure) ? FLAGS(SECURE) : FLAGS(UNSECURE);
	}

    client.SetParam(std::move(rqData));
    auto res = client.SyncRequest();
	if (!res.success)
	{
		std::wcout << std::endl << res.message.c_str() << std::endl;
		Exit(L"");
	}

    if(client.IsHeadersEmpty())
    {
		Exit(L"Headers empty");
    }
    std::wstring wstr;
    std::wistringstream wheaders = client.GetHeadersAsStrings();

    printf("------------------- Headers ------------------- \n");
    while(std::getline(wheaders, wstr, _TCHAR('\n')))
    {
        std::wcout << wstr << std::endl;
    }

    if(client.IsContentEmpty())
    {
		Exit(L"Content is empty");
    }

	std::wcout << std::endl << _T("Output content in console (y/n)? ");
	{
		wchar_t cOut{};
		std::wcin >> cOut; std::wcout << std::endl;

		if ('y' == cOut)
		{
			auto locale = setlocale(LC_ALL, "");
			std::string str;
			std::istringstream content = client.GetContentAsStrings();
			printf("------------------- Content -------------------\n");
			while (std::getline(content, str, ','))
			{
				std::wcout << StringToWstring(str) << std::endl;
			}
		}
	}

	std::wcout << std::endl << _T("Save content in file (y/n)? ");
	{
		wchar_t sFile{};
		std::wcin >> sFile; std::wcout << std::endl;

		if ('y' == sFile)
		{
			std::string str;
			std::istringstream content = client.GetContentAsStrings();
			std::ofstream fout("Content.txt");
			while (std::getline(content, str, ','))
			{
				fout << str << std::endl;
			}

			fout.close();
		}
	}

	std::wcout << _T("Press any key to exit...");
    _getch();

    return 0;
}

