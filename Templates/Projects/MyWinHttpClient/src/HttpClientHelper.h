#pragma once

#include "HttpDataStruct.h"

namespace http
{
	using HttpCallback = void (CALLBACK*)(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD);

	bool HttpOpenSyncSession(const SessionData& session, HINTERNET& hSession) noexcept;

	bool HttpConnectToServer(const HINTERNET hSession, const ConnectData& connect, HINTERNET& hConnect) noexcept;

	bool HttpCreateRequest(const HINTERNET hConnect, const CreateRequest& createReq, HINTERNET& hRequest) noexcept;

	bool HttpAddHeaders(const HINTERNET& hRequest, const RequestHeaders& reqHeaders) noexcept;

	bool HttpSendRequest(const HINTERNET hRequest, const SendRequest& sendReq, HttpCallback callback = nullptr);

	bool HttpWaitResponse(const HINTERNET hRequest, LPVOID reserved = {}) noexcept;

	bool HttpReadHeaders(const HINTERNET& hRequest, const ResponseHeaders& resHeaders, std::wstring& headers);

	bool HttpDataAvailable(HINTERNET hRequest) noexcept;

	bool HttpReadData(const HINTERNET& hRequest, std::string& data, std::wstring& errorMessage);

	bool HttpCloseSession(HINTERNET& hSession, HINTERNET& hConnect, HINTERNET& hRequest) noexcept;


#define IS_SECURE_FAILURE(err) (							\
		(err == ERROR_WINHTTP_SECURE_CERT_REV_FAILED)	||	\
		(err == ERROR_WINHTTP_SECURE_INVALID_CERT)		||	\
		(err == ERROR_WINHTTP_SECURE_CERT_REVOKED)		||	\
		(err == ERROR_WINHTTP_SECURE_INVALID_CA)		||	\
		(err == ERROR_WINHTTP_SECURE_CERT_CN_INVALID)	||	\
		(err == ERROR_WINHTTP_SECURE_CERT_DATE_INVALID)	||	\
		(err == ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE)	||	\
		(err == ERROR_WINHTTP_SECURE_CHANNEL_ERROR))

#define IS_WINHTTP_ERROR(err)(err > WINHTTP_ERROR_BASE && err <= WINHTTP_ERROR_LAST)
}

// Get size of array
template <typename T, std::size_t N>
std::size_t getSize(T(&)[N]) noexcept
{
   return N;
}

// Convert BSTR to string/wstring and vice versa
std::wstring BSTRToWstring(const BSTR bstr);
std::string BSTRToString(const BSTR bstr);
BSTR WstringToBSTR(const std::string& wstr);
BSTR StringToBSTR(const std::string& str);


std::string WstringToString(const std::wstring& wstr);
std::wstring StringToWstring(const std::string& str);

// Convert wstring to string
std::string Wstr2Str(const std::wstring& wstr);
std::wstring Str2Wstr(const std::string& str);

//std::wstring Utf8ToUtf16(const std::string& str);
