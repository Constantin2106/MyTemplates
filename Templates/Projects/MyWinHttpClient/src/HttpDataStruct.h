#pragma once
#include <windows.h>
#include <winhttp.h>
#include <tchar.h>
#include <string>

#pragma comment( lib, "oleaut32.lib" )

enum class AccessType : DWORD
{
   NO_PROXY = WINHTTP_ACCESS_TYPE_NO_PROXY,
   DEF_PROXY = WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
   NAMED_PROXY = WINHTTP_ACCESS_TYPE_NAMED_PROXY,
   AUTO_PROXY = WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY
};
#define ACCESS_TYPE(TYPE) (DWORD)(AccessType::##TYPE)

enum class InternetPort : INTERNET_PORT
{
   HTTP_PORT = INTERNET_DEFAULT_HTTP_PORT,
   HTTPS_PORT = INTERNET_DEFAULT_HTTPS_PORT,
   DEFAULT_PORT = INTERNET_DEFAULT_PORT
};
#define PORT(PORT) (INTERNET_PORT)(InternetPort::##PORT)

enum class OpenRequestFlags : DWORD
{
   PROXY_CACHE = WINHTTP_FLAG_BYPASS_PROXY_CACHE,
   ESCAPE_DISABLE = WINHTTP_FLAG_ESCAPE_DISABLE,
   ESCAPE_DISABLE_QUERY = WINHTTP_FLAG_ESCAPE_DISABLE_QUERY,
   ESCAPE_PERCENT = WINHTTP_FLAG_ESCAPE_PERCENT,
   CODEPAGE = WINHTTP_FLAG_NULL_CODEPAGE,
   REFRESH = WINHTTP_FLAG_REFRESH,
   SECURE = WINHTTP_FLAG_SECURE,
   UNSECURE = 0
};
#define FLAGS(FLAGS) (DWORD)(OpenRequestFlags::##FLAGS)

#define VERB(V) _T(#V)

struct SessionData
{
   std::wstring agent{ _T("agent") };

   DWORD accessType = ACCESS_TYPE(DEF_PROXY);
   LPCWSTR proxyName = WINHTTP_NO_PROXY_NAME;
   LPCWSTR proxyBypass = WINHTTP_NO_PROXY_BYPASS;
   DWORD sessionFlags{};
};
struct ConnectData
{
   std::wstring server;
   INTERNET_PORT port = PORT(DEFAULT_PORT);
};
struct CreateRequest
{
   LPCWSTR verb = VERB(GET);
   DWORD requestFlags = FLAGS(SECURE);
   std::wstring objName;
   LPCWSTR version{};
   LPCWSTR referrer = WINHTTP_NO_REFERER;
   LPCWSTR* acceptTypes = WINHTTP_DEFAULT_ACCEPT_TYPES;
};
struct RequestHeaders
{
   std::wstring headers{};
   DWORD modifiers = WINHTTP_ADDREQ_FLAG_ADD;
};
struct SendRequest
{
   LPCWSTR headers = WINHTTP_NO_ADDITIONAL_HEADERS;
   DWORD headersLength{};
   LPVOID optional = WINHTTP_NO_REQUEST_DATA;
   DWORD optionalLength{};
   DWORD totalLength{};
   DWORD_PTR context{};
};
struct ResponseHeaders
{
   DWORD infoLevel = WINHTTP_QUERY_RAW_HEADERS_CRLF;
   LPCWSTR name = WINHTTP_HEADER_NAME_BY_INDEX;
   LPDWORD index = WINHTTP_NO_HEADER_INDEX;
};

struct RequestData
{
   // Session data
   SessionData session;

   // Connect data
   ConnectData connect;

   // Create Request data
   CreateRequest createReq;

   // Request headers
   RequestHeaders reqHeaders;

   // Send request data
   SendRequest sendReq;

   // Read headers data
   ResponseHeaders resHeaders;
};

struct RequestResult
{
   bool success = true;
   DWORD error = WINHTTP_ERROR_BASE;
   DWORD status{};

   std::wstring message;
};
#define REQUEST_SUCCEEDED(resError) (resError <= WINHTTP_ERROR_BASE)
#define REQUEST_FAILED(resError) (resError > WINHTTP_ERROR_BASE)
#define RETURN_ERROR(result) { result.success = false; result.error = ::GetLastError(); return result; }
