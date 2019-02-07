#pragma once

#include "HttpDataStruct.h"

bool HttpOpenSyncSession(HINTERNET& _hSession, SessionData& _session);

bool HttpConnectToServer(HINTERNET& _hConnect, HINTERNET _hSession, ConnectData& _connect);

bool HttpCreateRequest(HINTERNET& _hRequest, HINTERNET _hConnect, CreateRequest& _createReq);

bool HttpAddHeaders(HINTERNET& _hRequest, RequestHeaders& _reqHeaders);

bool HttpSendRequest(HINTERNET _hRequest, SendRequest& _sendReq);

bool HttpWaitAnswer(HINTERNET hRequest, LPVOID _reserved = {});

bool HttpReadHeaders(HINTERNET& _hRequest, ResponseHeaders& _resHeaders, std::wstring& _headers);

bool HttpDataAvailable(HINTERNET _hRequest);

bool HttpReadAnswer(HINTERNET& _hRequest, std::string& _answer, std::wstring& _errorMessage);

bool HttpCloseSession(HINTERNET& _hSession, HINTERNET& _hConnect, HINTERNET& _hRequest);

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

