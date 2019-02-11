// MyWinHttpClient.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include <sstream>
#include "src\HttpClient.h"
#include "src\HttpClientHelper.h"

#define Exit(message)					\
	std::cout << message << std::endl;	\
	_getch();							\
	return{}

int main()
{
    //int x[100];
    //auto s = getSize(x);

    //--------------------------------------------------------//
    /*const BYTE bytes[] = { 72, 101, 108, 108, 111 };
    std::string strBytes = ByteArrayToString(bytes);
    std::cout << strBytes << std::endl;

    const BSTR bstr = ::SysAllocString(L"Hello world");
    std::string strn = BSTRToString(bstr);
    std::cout << strn << std::endl;
    ::SysFreeString(bstr);

    const std::wstring widestr(L"Hello again");
    std::string charstr = WstringToString(widestr);
    std::cout << charstr << std::endl;*/

	/*std::wstring widestr(L"ÉÖÓÊÅÍ");
	std::string strng = Wstr2Str(widestr);
	std::cout << strng << std::endl;*/
    //--------------------------------------------------------//


    HttpClient client;
    RequestData rqData;

    rqData.connect.server.assign(_T("www.kmp.ua"));
    rqData.createReq.objName.assign(_T("/wp-json/wp/v2/users/1"));
    //rqData._reqHeaders._headers = ;

    client.SetParam(std::move(rqData));
    auto res = client.SyncRequest();
	if (!res.success)
	{
		std::wcout << res.message.c_str() << std::endl;
		//std::string msg = WstringToString(res.message);
		//const int size = res.message.size();
		size_t size = res.message.length();
		char* buff = new char[size];
		wcstombs(buff, res.message.c_str(), size);
		std::string msg(buff);
		std::cout << std::endl << buff << std::endl;
		//delete[] buff;
		Exit("");
	}

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

