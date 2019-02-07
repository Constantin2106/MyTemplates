#pragma once
#include <windows.h>
#include <winhttp.h>
#include <tchar.h>
//#include <string>
#include <sstream>
//#include <iomanip>
//#include <vector>

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
   std::wstring _agent{ _T("agent") };

   DWORD _accessType = ACCESS_TYPE(NO_PROXY);
   LPCWSTR _proxyName = WINHTTP_NO_PROXY_NAME;
   LPCWSTR _proxyBypass = WINHTTP_NO_PROXY_BYPASS;
   DWORD _sessionFlags{};
};
struct ConnectData
{
   std::wstring _server;
   INTERNET_PORT _port = PORT(DEFAULT_PORT);
};
struct CreateRequest
{
   LPCWSTR _verb = nullptr;
   DWORD _requestFlags = FLAGS(SECURE);
   std::wstring _objName;
   LPCWSTR _version{};
   LPCWSTR _referrer = WINHTTP_NO_REFERER;
   LPCWSTR* _acceptTypes = WINHTTP_DEFAULT_ACCEPT_TYPES;
};
struct RequestHeaders
{
   std::wstring _headers{};
   DWORD _modifiers = WINHTTP_ADDREQ_FLAG_ADD;
};
struct SendRequest
{
   LPCWSTR _headers = WINHTTP_NO_ADDITIONAL_HEADERS;
   DWORD _headersLength{};
   LPVOID _optional = WINHTTP_NO_REQUEST_DATA;
   DWORD _optionalLength{};
   DWORD _totalLength{};
   DWORD_PTR _context{};
};
struct ResponseHeaders
{
   DWORD _infoLevel = WINHTTP_QUERY_RAW_HEADERS_CRLF;
   LPCWSTR _name = WINHTTP_HEADER_NAME_BY_INDEX;
   LPDWORD _index = WINHTTP_NO_HEADER_INDEX;
};

struct RequestData
{
   // Session data
   SessionData _session;

   // Connect data
   ConnectData _connect;

   // Create Request data
   CreateRequest _createReq;

   // Request headers
   RequestHeaders _reqHeaders;

   // Send request data
   SendRequest _sendReq;

   // Read headers data
   ResponseHeaders _resHeaders;
};

struct RequestResult
{
   bool success = true;
   DWORD error = WINHTTP_ERROR_BASE;
   std::wstring message;
};
#define RETURN_ERROR(result) { result.success = false; result.error = GetLastError(); return result;	}

