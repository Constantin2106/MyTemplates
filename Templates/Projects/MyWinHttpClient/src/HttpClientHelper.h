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

	bool HttpWaitAnswer(const HINTERNET hRequest, LPVOID _reserved = {});

	bool HttpReadHeaders(const HINTERNET& hRequest, const ResponseHeaders& resHeaders, std::wstring& _headers);

	bool HttpDataAvailable(HINTERNET _hRequest);

	bool HttpReadAnswer(const HINTERNET& hRequest, std::string& _answer, std::wstring& _errorMessage);

	bool HttpCloseSession(HINTERNET& _hSession, HINTERNET& _hConnect, HINTERNET& _hRequest);


#define IS_SECURE_FAILURE(err) (							\
		(err == ERROR_WINHTTP_SECURE_CERT_REV_FAILED)	||	\
		(err == ERROR_WINHTTP_SECURE_INVALID_CERT)		||	\
		(err == ERROR_WINHTTP_SECURE_CERT_REVOKED)		||	\
		(err == ERROR_WINHTTP_SECURE_INVALID_CA)		||	\
		(err == ERROR_WINHTTP_SECURE_CERT_CN_INVALID)	||	\
		(err == ERROR_WINHTTP_SECURE_CERT_DATE_INVALID)	||	\
		(err == ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE)	||	\
		(err == ERROR_WINHTTP_SECURE_CHANNEL_ERROR))
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

// Convert wstring to string
std::string Wstr2Str(const std::wstring& wstr);
std::wstring Str2Wstr(const std::string& str);

