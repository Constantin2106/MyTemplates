// MyWinHttpClient.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include <sstream>
#include "src\HttpClient.h"

#define Exit(message)					\
	std::cout << message << std::endl;	\
	_getch();							\
	return{}

int main()
{
    //int x[100];
    //auto s = getSize(x);

    //--------------------------------------------------------//
    //const BYTE bytes[] = { 72, 101, 108, 108, 111 };
    //std::string strBytes = ByteArrayToString(bytes);
    //std::cout << strBytes << std::endl;

    //const BSTR bstr = ::SysAllocString(L"Hello world");
    //std::string strn = BSTRToString(bstr);
    //std::cout << strn << std::endl;
    //::SysFreeString(bstr);

    //const std::wstring widestr(L"Hello again");
    //std::string charstr = WstringToString(widestr);
    //std::cout << charstr << std::endl;
    //--------------------------------------------------------//


    HttpClient client;
    RequestData rqData;

    rqData.connect.server.assign(_T("www.kmp.ua"));
    rqData.createReq.objName.assign(_T("/wp-json/wp/v2/users/1"));
    //rqData._reqHeaders._headers = ;

    client.SetParam(std::move(rqData));
    auto res = client.SyncRequest();

    if(client.IsHeadersEmpty())
    {
		Exit("Headers empty");
    }
    std::wstring wstr;
    std::wistringstream wheaders = client.GetHeadersAsStrings();

    printf("-------------- Header contents -------------- \n");
    while(std::getline(wheaders, wstr, _TCHAR('\n')))
    {
        std::wcout << wstr << std::endl;
    }

    if(client.IsAnswerEmpty())
    {
		Exit("Answer empty");
    }
    std::string str;
    std::istringstream answers = client.GetAnswerAsStrings();
    printf("-------------- Response contents --------------\n");
    while(std::getline(answers, str, '\n'))
    {
        std::cout << str << std::endl;
    }

    _getch();

    return 0;
}

