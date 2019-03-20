#include <iostream>
#include <vector>
#include <sstream>
#include <array>
#include <assert.h>

#include "HttpClientHelper.h"

namespace http
{
	// Mapping status flags to winhttp errors
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
		auto reqResult = reinterpret_cast<RequestResult*>(context);
		if (!reqResult)
			return;

		DWORD statInf{};
		auto& error = reqResult->error;
		if(statusInfo)
			statInf = *(static_cast<DWORD*>(statusInfo));

		switch(status)
		{
		case WINHTTP_CALLBACK_FLAG_RESOLVE_NAME:
			//error = ::GetLastError();						// This case isn't error
			break;
		case WINHTTP_CALLBACK_FLAG_CONNECT_TO_SERVER:
			break;
		case WINHTTP_CALLBACK_FLAG_SEND_REQUEST:
			break;
		case WINHTTP_CALLBACK_FLAG_RECEIVE_RESPONSE:
			break;
		case WINHTTP_CALLBACK_FLAG_CLOSE_CONNECTION:
			break;
		case WINHTTP_CALLBACK_FLAG_HANDLES:
			break;
		case WINHTTP_CALLBACK_FLAG_DETECTING_PROXY:
			break;
		case WINHTTP_CALLBACK_FLAG_REDIRECT:
			break;
		case WINHTTP_CALLBACK_FLAG_INTERMEDIATE_RESPONSE:
			break;
		case WINHTTP_CALLBACK_FLAG_SECURE_FAILURE:			// This case is error
#pragma region  Temporary Console Log
			//#ifdef _DEBUG
			//			std::cout << std::endl << std::endl << "Secure failure" << std::endl;
			//			auto stInfo = *(static_cast<DWORD*>(statusInfo));
			//			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_REV_FAILED)
			//				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_REV_FAILED" << std::endl;
			//			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CERT)
			//				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CERT" << std::endl;
			//			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_REVOKED)
			//				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_REVOKED" << std::endl;
			//			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CA)
			//				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_INVALID_CA" << std::endl;
			//			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_CN_INVALID)
			//				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_CN_INVALID" << std::endl;
			//			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_CERT_DATE_INVALID)
			//				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_CERT_DATE_INVALID" << std::endl;
			//			if (stInfo & WINHTTP_CALLBACK_STATUS_FLAG_SECURITY_CHANNEL_ERROR)
			//				std::cout << std::endl << "WINHTTP_CALLBACK_STATUS_FLAG_SECURITY_CHANNEL_ERROR" << std::endl;
			//#endif // _DEBUG
#pragma endregion
			reqResult->success = false;
			for (auto& element : statusToError)
			{
				error = (statInf & element.first) ? 
							element.second : 0;
				if (error)
					break;
			}
			break;
		case WINHTTP_CALLBACK_FLAG_SENDREQUEST_COMPLETE:
			break;
		case WINHTTP_CALLBACK_FLAG_HEADERS_AVAILABLE:
			break;
		case WINHTTP_CALLBACK_FLAG_DATA_AVAILABLE:
			break;
		case WINHTTP_CALLBACK_FLAG_READ_COMPLETE:
			break;
		case WINHTTP_CALLBACK_FLAG_WRITE_COMPLETE:
			break;
		case WINHTTP_CALLBACK_FLAG_REQUEST_ERROR:			// This case is error
			break;
		}
	}

	static void GetErrorMessage(RequestResult* const reqResult)
	{
		// Receive the system message
		LPWSTR buffer{};
		HMODULE hModule{};

		if (IS_WINHTTP_ERROR(reqResult->error))
		{
			hModule = LoadLibrary(_T("wininet.dll"));
		}
		/*else if ()
		{
			// TODO: Load appropriate lib
		}*/
		
		if (hModule)
		{
			auto nTCHARs = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
				hModule,
				reqResult->error,
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				(LPWSTR)&buffer,
				0,
				NULL);
			FreeLibrary(hModule);
			if (nTCHARs)
			{
				reqResult->message.assign(_T("Error code: ") + std::to_wstring(reqResult->error) + _T('\t'));
				reqResult->message.append(buffer, nTCHARs - 2); // -2 to remove \r\n symbols

				::LocalFree(buffer);
			}
		}
	}

	bool HttpOpenSyncSession(const SessionData& session, HINTERNET& hSession) noexcept
	{
		hSession = ::WinHttpOpen(
			session.agent.c_str(),
			session.accessType,
			session.proxyName,
			session.proxyBypass,
			session.sessionFlags);

		return NULL != hSession;
	}

	bool HttpConnectToServer(const HINTERNET hSession, const ConnectData& connect, HINTERNET& hConnect) noexcept
	{
		hConnect = ::WinHttpConnect(
			hSession,
			connect.server.c_str(),
			connect.port, 0);

		return NULL != hConnect;
	}

	bool HttpCreateRequest(const HINTERNET hConnect, const CreateRequest& createReq, HINTERNET& hRequest) noexcept
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

	bool HttpAddHeaders(const HINTERNET& hRequest, const RequestHeaders& reqHeaders) noexcept
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
									WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, NULL);

			if (WINHTTP_INVALID_STATUS_CALLBACK == callbackSet)
			{
				// TODO: ???
				reqResult->success = false;
				SET_ERROR_CODE(reqResult->error);
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
			reqResult->success = succeeded;
			//TODO: Temporarry error code
			auto err = ::GetLastError();
			SET_ERROR_CODE(reqResult->error);

			if (reqResult->error)
			{
				// Receive the system message
				GetErrorMessage(reqResult);
			}
		}

		return succeeded;
	}

	bool HttpWaitResponse(const HINTERNET hRequest, LPVOID reserved/* = NULL*/) noexcept
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
			headers.assign(reinterpret_cast<wchar_t*>(buffer.data()));
		}

		return TRUE == bResult;
	}

	bool HttpReadData(const HINTERNET& hRequest, std::string& msgBody, std::wstring& errorMessage)
	{
		DWORD dataSize{};
		DWORD downloadedData{};
		std::vector<BYTE> content{};

		msgBody.clear();
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
				if (msgBody.size())
				{
					return true;
				}

				errorMessage.assign(_T("No response data\n"));
				return false;
			}

			// Allocate space for the buffer
			content.resize(dataSize, '\0');

			// Read the data										IN ASYNCHRONOUSLY MODE MUST BE NULL
			//														https://docs.microsoft.com/en-us/windows/desktop/api/winhttp/nf-winhttp-winhttpreaddata	
			if (!::WinHttpReadData(hRequest, &content[0], dataSize, &downloadedData))
			{
				errorMessage = std::wstring(_T("Error "))
					+ std::to_wstring(GetLastError())
					+ std::wstring(_T(" in WinHttpReadData.\n"));
				return false;
			}

			auto data = reinterpret_cast<PCHAR>(content.data());
			msgBody.append(data, data + downloadedData);
			
			if (dataSize != downloadedData)
			{
				// TODO: ???
				return false;
			}
		}

		return false;
	}

	bool HttpCloseSession(HINTERNET& hSession, HINTERNET& hCconnect, HINTERNET& hRequest) noexcept
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

// Convert a wide Unicode string to an string
std::string WstringToString(const std::wstring& wstr, const UINT codePage/* = CP_UTF8*/)
{
	if (wstr.empty()) 
		return std::string();

	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd374130(v=vs.85).aspx
	const auto size_needed = ::WideCharToMultiByte(codePage, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, '\0');
	::WideCharToMultiByte(codePage, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
// Convert an string to a wide Unicode String
std::wstring StringToWstring(const std::string& str, const UINT codePage/* = CP_UTF8*/)
{
	if (str.empty()) 
		return std::wstring();

	int size_needed = ::MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, L'\0');
	::MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

std::wstring BSTRToWstring(const BSTR bstr)
{
	assert(nullptr != bstr);
	std::wstring wstr(bstr, ::SysStringLen(bstr));
	return wstr;
}
std::string BSTRToString(const BSTR bstr, const UINT codePage/* = CP_UTF8*/)
{
	std::wstring wstr = BSTRToWstring(bstr);
	return WstringToString(wstr, codePage);
}
BSTR WstringToBSTR(const std::wstring& wstr)
{
	return ::SysAllocStringLen((BSTR)(wstr.data()), wstr.size());
}
BSTR StringToBSTR(const std::string& str, const UINT codePage/* = CP_UTF8*/)
{
	std::wstring wstr = StringToWstring(str, codePage);
	return WstringToBSTR(wstr);
}

#include <codecvt>
#include <clocale>
std::string Wstr2Str(const std::wstring& wstr)
{
	using convType = std::codecvt<wchar_t, char, std::mbstate_t>;

	static std::locale loc("");
	static auto& facet = std::use_facet<convType>(loc);
	static std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t> converter(&facet);

	return converter.to_bytes(wstr);
}
std::wstring Str2Wstr(const std::string& str)
{
	using convType = std::codecvt<wchar_t, char, std::mbstate_t>;

	static std::locale loc("");
	static auto& facet = std::use_facet<convType>(loc);
	static std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t> converter(&facet);

	return converter.from_bytes(str);
}



