#include <iostream>
#include <vector>
#include <sstream>
#include <array>

#include "HttpClientHelper.h"

namespace http
{
	static const std::array<std::pair<DWORD, DWORD>, 8> statusToError({
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_CERT_REV_FAILED,			ERROR_WINHTTP_SECURE_CERT_REV_FAILED),
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CERT,				ERROR_WINHTTP_SECURE_INVALID_CERT),
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_CERT_REVOKED,				ERROR_WINHTTP_SECURE_CERT_REVOKED),
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CA,				ERROR_WINHTTP_SECURE_INVALID_CA),
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_CERT_CN_INVALID,			ERROR_WINHTTP_SECURE_CERT_CN_INVALID),
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_CERT_DATE_INVALID,		ERROR_WINHTTP_SECURE_CERT_DATE_INVALID),
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_CERT_WRONG_USAGE,			ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE),
		std::make_pair<DWORD, DWORD>(WINHTTP_CALLBACK_STATUS_FLAG_SECURITY_CHANNEL_ERROR,	ERROR_WINHTTP_SECURE_CHANNEL_ERROR)
		});

	static void CALLBACK HttpSendRequestCallback(
		HINTERNET hInternet,
		DWORD_PTR context,
		DWORD status,
		LPVOID statusInfo,
		DWORD statusInfoLength)
	{
		if (WINHTTP_CALLBACK_FLAG_SECURE_FAILURE == status)
		{
#pragma region  Temporary Console Log
#ifdef _DEBUG
			std::cout << std::endl << std::endl << "Secure failure" << std::endl;
			auto stInfo = *(static_cast<DWORD*>(statusInfo));
			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_REV_FAILED)
				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_REV_FAILED" << std::endl;
			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CERT)
				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CERT" << std::endl;
			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_REVOKED)
				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_REVOKED" << std::endl;
			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CA)
				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CA" << std::endl;
			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_CN_INVALID)
				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_CN_INVALID" << std::endl;
			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_DATE_INVALID)
				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_DATE_INVALID" << std::endl;
			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_SECURITY_CHANNEL_ERROR)
				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_SECURITY_CHANNEL_ERROR" << std::endl;
#endif // _DEBUG
#pragma endregion

			auto reqResult = reinterpret_cast<RequestResult*>(context);
			if (!reqResult)
				return;

			auto stat = *(static_cast<DWORD*>(statusInfo));
			reqResult->success = false;

			for (auto& element : statusToError)
			{
				reqResult->status = stat & element.first ? element.second : 0;
				if (reqResult->status)
					break;
			}
		}
	}

	bool HttpOpenSyncSession(const SessionData& session, HINTERNET& hSession)
	{
		hSession = ::WinHttpOpen(
			session.agent.c_str(),
			session.accessType,
			session.proxyName,
			session.proxyBypass,
			session.sessionFlags);

		return NULL != hSession;
	}

	bool HttpConnectToServer(const HINTERNET hSession, const ConnectData& connect, HINTERNET& hConnect)
	{
		hConnect = ::WinHttpConnect(
			hSession,
			connect.server.c_str(),
			connect.port, 0);

		return NULL != hConnect;
	}

	bool HttpCreateRequest(const HINTERNET hConnect, const CreateRequest& createReq, HINTERNET& hRequest)
	{
		hRequest = ::WinHttpOpenRequest(
			hConnect,
			createReq.verb,
			createReq.objName.c_str(),
			createReq.version,
			createReq.referrer,
			createReq.acceptTypes,
			createReq.requestFlags);

		return NULL != hRequest;
	}

	bool HttpAddHeaders(const HINTERNET& hRequest, const RequestHeaders& reqHeaders)
	{
		return TRUE == ::WinHttpAddRequestHeaders(
			hRequest,
			reqHeaders.headers.c_str(),
			ULONG(-1L),
			reqHeaders.modifiers);
	}

	bool HttpSendRequest(const HINTERNET hRequest, const SendRequest& sendReq, HttpCallback callback/* = nullptr*/)
	{
		auto reqResult = reinterpret_cast<RequestResult*>(sendReq.context);

		if (reqResult)
		{
			HttpCallback statusCallback = callback ? callback : HttpSendRequestCallback;

			auto callbackSet = ::WinHttpSetStatusCallback(
									hRequest,
									reinterpret_cast<WINHTTP_STATUS_CALLBACK>(statusCallback),
									WINHTTP_CALLBACK_FLAG_SECURE_FAILURE, NULL);

			if (WINHTTP_INVALID_STATUS_CALLBACK == callbackSet)
			{
				reqResult->success = false;
				reqResult->error = ::GetLastError();
				return false;
			}
		}

		auto succeeded = TRUE == ::WinHttpSendRequest(
									hRequest,
									sendReq.headers,
									sendReq.headersLength,
									sendReq.optional,
									sendReq.optionalLength,
									sendReq.totalLength,
									sendReq.context);
		if (reqResult)
		{
			// TODO: Here need check if sendReq.context has RequestResult type
			// Because any variable can be passed here.
			reqResult->success = succeeded;
			reqResult->error = ::GetLastError();
		}

		return succeeded;
	}

	bool HttpWaitAnswer(const HINTERNET hRequest, LPVOID reserved/* = NULL*/)
	{
		return TRUE == ::WinHttpReceiveResponse(hRequest, reserved);
	}

	bool HttpReadHeaders(const HINTERNET& hRequest, const ResponseHeaders& resHeaders, std::wstring& headers)
	{
		DWORD hdSize{};
		BOOL bResult = FALSE;
		DWORD downloaded{};

		headers.clear();

		// Use WinHttpQueryHeaders to obtain the size of the buffer.
		if (::WinHttpQueryHeaders(
				hRequest,
				resHeaders.infoLevel,
				resHeaders.name,
				NULL, &hdSize,
				resHeaders.index))
		{
			return false;
		}

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			return false;
		}

		if (0 == hdSize)
		{
			return false;
		}

		// Allocate memory for the buffer.
		std::vector<BYTE> buffer(hdSize, '\0');
		
		// Use WinHttpQueryHeaders to retrieve the header.
		bResult = ::WinHttpQueryHeaders(
					hRequest,
					resHeaders.infoLevel,
					resHeaders.name,
					&buffer[0], &hdSize,
					resHeaders.index);

		if (bResult)
		{
			headers.assign(reinterpret_cast<PTCHAR>(buffer.data()));
		}

		return TRUE == bResult;
	}

	bool HttpReadAnswer(const HINTERNET& hRequest, std::string& answer, std::wstring& errorMessage)
	{
		DWORD dataSize{};
		DWORD downloadedData{};
		std::vector<BYTE> content{};

		answer.clear();
		errorMessage.assign(_T("Succeeded\n"));

		while (true)
		{
			// Check for data available
			if (!::WinHttpQueryDataAvailable(hRequest, &dataSize))
			{
				errorMessage = std::wstring(_T("Error "))
					+ std::to_wstring(GetLastError())
					+ std::wstring(_T(" in WinHttpQueryDataAvailable.\n"));
				return false;
			}

			if (0 == dataSize)
			{
				if (answer.size())
				{
					return true;
				}

				errorMessage.assign(_T("No response data\n"));
				return false;
			}

			// Allocate space for the buffer
			content.resize(dataSize + 1, '\0');

			// Read the data										IN ASYNCHRONOUSLY MODE MUST BE NULL
			//														https://docs.microsoft.com/en-us/windows/desktop/api/winhttp/nf-winhttp-winhttpreaddata	
			if (!::WinHttpReadData(hRequest, &content[0], dataSize, &downloadedData))
			{
				errorMessage = std::wstring(_T("Error "))
					+ std::to_wstring(GetLastError())
					+ std::wstring(_T(" in WinHttpReadData.\n"));
				return false;
			}

			answer.append(reinterpret_cast<PCHAR>(content.data()));

			if (dataSize != downloadedData)
			{
				return false;
			}

		}

		// Convert string to wstring
		//int wchars_num = MultiByteToWideChar(CP_UTF8, 0, answer.c_str(), -1, NULL, 0);
		//wchar_t* wstr = new wchar_t[wchars_num];
		//wchars_num = MultiByteToWideChar(CP_UTF8, 0, answer.c_str(), -1, wstr, wchars_num);
		//std::wstring _data;
		//_data.assign(wstr);
		//delete[] wstr;

		return false;
	}

	bool HttpCloseSession(HINTERNET& hSession, HINTERNET& hCconnect, HINTERNET& hRequest)
	{
		bool isClosed{};

		if (hRequest)
			isClosed = (TRUE == ::WinHttpCloseHandle(hRequest));

		if (hCconnect)
			isClosed &= (TRUE == ::WinHttpCloseHandle(hCconnect));

		if (hSession)
			isClosed &= (TRUE == ::WinHttpCloseHandle(hSession));

		return isClosed;
	}
}

std::string ByteArrayToString(const BYTE bytes[], std::size_t n)
{
    std::ostringstream stm;
    //stm << std::hex << std::uppercase;

    for (std::size_t i = 0; i < n; ++i)
        //stm << std::setw(2) << std::setfill('0') << unsigned(bytes[i]);
        stm << bytes[i];// << bytes[++i];

    return stm.str();
}

std::string WstringToString(std::wstring wstr)
{
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130(v=vs.85).aspx
    const auto sz = ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::vector<char> vec(sz);
    ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), vec.data(), (int)vec.size(), nullptr, nullptr);
    return { vec.begin(), vec.end() };
}

std::string BSTRToString(const BSTR bstr)
{
    return WstringToString(bstr);
}

#include <codecvt>
#include <clocale>
std::string Wstr2Str(const std::wstring& wstr)
{
	std::setlocale(LC_ALL, "");
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
std::wstring Str2Wstr(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

//std::wstring Utf8ToUtf16(const std::string& str)
//{
//	std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;
//	std::wstring wstr = converter.from_bytes(str);
//	return wstr;
//}


