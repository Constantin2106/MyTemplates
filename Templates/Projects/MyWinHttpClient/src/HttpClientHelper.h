#pragma once

#include "HttpDataStruct.h"

namespace http
{
	using HttpCallback = void (CALLBACK*)(HINTERNET, DWORD_PTR, DWORD, LPVOID, DWORD);

	bool HttpOpenSyncSession(const SessionData& session, HINTERNET& hSession);

	bool HttpConnectToServer(const HINTERNET hSession, const ConnectData& connect, HINTERNET& hConnect);

	bool HttpCreateRequest(const HINTERNET hConnect, const CreateRequest& createReq, HINTERNET& hRequest);

	bool HttpAddHeaders(const HINTERNET& hRequest, const RequestHeaders& reqHeaders);

	bool HttpSendRequest(const HINTERNET hRequest, const SendRequest& sendReq, HttpCallback callback = nullptr);

	bool HttpWaitResponse(const HINTERNET hRequest, LPVOID reserved = {});

	bool HttpReadHeaders(const HINTERNET& hRequest, const ResponseHeaders& resHeaders, std::wstring& headers);

	bool HttpDataAvailable(HINTERNET hRequest);

	bool HttpReadData(const HINTERNET& hRequest, std::string& data, std::wstring& errorMessage);

	bool HttpCloseSession(HINTERNET& hSession, HINTERNET& hConnect, HINTERNET& hRequest);


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
std::size_t getSize(T(&)[N])
{
   return N;
}

// Convert byte sequence to string of char
std::string ByteArrayToString(const BYTE bytes[], std::size_t n);

template <std::size_t N>
std::string ByteArrayToString(const BYTE(&byte_array)[N])
{
   return ByteArrayToString(byte_array, N);
}

// Convert BSTR to string of char
std::string BSTRToString(const BSTR bstr);
std::string WstringToString(std::wstring wstr);
std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);

// Convert wstring to string
std::string Wstr2Str(const std::wstring& wstr);
std::wstring Str2Wstr(const std::string& str);

//std::wstring Utf8ToUtf16(const std::string& str);
