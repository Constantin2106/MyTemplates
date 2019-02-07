
#include <vector>

#include "HttpClientHelper.h"

bool HttpOpenSyncSession(HINTERNET& _hSession, SessionData& _session)
{
   _hSession = WinHttpOpen(
      _session._agent.c_str()
      , _session._accessType
      , _session._proxyName
      , _session._proxyBypass
      , _session._sessionFlags);

   return NULL != _hSession;
}

bool HttpConnectToServer(HINTERNET& _hConnect, HINTERNET _hSession, ConnectData& _connect)
{
   _hConnect = WinHttpConnect(
      _hSession
      , _connect._server.c_str()
      , _connect._port, 0);

   return NULL != _hConnect;
}

bool HttpCreateRequest(HINTERNET& _hRequest, HINTERNET _hConnect, CreateRequest& _createReq)
{
   _hRequest = WinHttpOpenRequest(
      _hConnect
      , _createReq._verb
      , _createReq._objName.c_str()
      , _createReq._version
      , _createReq._referrer
      , _createReq._acceptTypes
      , _createReq._requestFlags);

   return NULL != _hRequest;
}

bool HttpAddHeaders(HINTERNET& _hRequest, RequestHeaders& _reqHeaders)
{
   return TRUE == WinHttpAddRequestHeaders(
      _hRequest
      , _reqHeaders._headers.c_str()
      , ULONG(-1L)
      , _reqHeaders._modifiers);
}

bool HttpSendRequest(HINTERNET _hRequest, SendRequest& _sendReq)
{
   return TRUE == WinHttpSendRequest(
      _hRequest
      , _sendReq._headers
      , _sendReq._headersLength
      , _sendReq._optional
      , _sendReq._optionalLength
      , _sendReq._totalLength
      , _sendReq._context);
}

bool HttpWaitAnswer(HINTERNET _hRequest, LPVOID _reserved/* = NULL*/)
{
   return TRUE == WinHttpReceiveResponse(_hRequest, _reserved);
}

bool HttpReadHeaders( HINTERNET& _hRequest, ResponseHeaders& _resHeaders, std::wstring& _headers)
{
   DWORD hdSize{};
   BOOL bResult = FALSE;
   DWORD downloaded{};

   _headers.clear();

   // Use WinHttpQueryHeaders to obtain the size of the buffer.
   if (WinHttpQueryHeaders(
      _hRequest
      , _resHeaders._infoLevel
      , _resHeaders._name
      , NULL, &hdSize
      , _resHeaders._index))
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
   BYTE* buffer{};
   try
   {
      buffer = new BYTE[hdSize + 1];
   }
   catch (std::bad_alloc&)
   {
      return false;
   }

   ZeroMemory(buffer, hdSize);

   // Use WinHttpQueryHeaders to retrieve the header.
   bResult = WinHttpQueryHeaders(
      _hRequest
      , _resHeaders._infoLevel
      , _resHeaders._name
      , buffer, &hdSize
      , _resHeaders._index);

   if (bResult)
   {
      _headers.assign(reinterpret_cast<PTCHAR>(buffer));
   }

   return TRUE == bResult;
}

bool HttpReadAnswer( HINTERNET& _hRequest, std::string& _answer, std::wstring& _errorMessage)
{
   DWORD dataSize{};
   DWORD downloadedData{};
   BYTE* dataBuffer{};

   _answer.clear();
   _errorMessage.assign(_T("Succeeded\n"));

   while (true)
   {
      // Check for data available
      if (!WinHttpQueryDataAvailable(_hRequest, &dataSize))
      {
         _errorMessage = std::wstring(_T("Error ")) 
            + std::to_wstring(GetLastError()) 
            + std::wstring(_T(" in WinHttpQueryDataAvailable.\n"));
         return false;
      }

      if (0 == dataSize)
      {
         if (_answer.size())
         {
            return true;
         }

          _errorMessage.assign(_T("No response data\n"));
          return false;
      }

      // Allocate space for the buffer
      try
      {
         dataBuffer = new BYTE[dataSize + 1];
      }
      catch (std::bad_alloc&)
      {
         _errorMessage.assign(_T("Out of memory\n"));
         return false;
      }

      ZeroMemory(dataBuffer, dataSize + 1);

      // Read the data
      if (!WinHttpReadData(_hRequest, dataBuffer, dataSize, &downloadedData))
      {
         _errorMessage = std::wstring(_T("Error ")) 
            + std::to_wstring(GetLastError()) 
            + std::wstring(_T(" in WinHttpReadData.\n"));
         return false;
      }

      _answer.append(reinterpret_cast<PCHAR>(dataBuffer));

      // Free the memory allocated to the buffer
      delete[] dataBuffer;

      if (dataSize != downloadedData)
      {
         return false;
      }

   }

   // Convert string to wstring
   //int wchars_num = MultiByteToWideChar(CP_UTF8, 0, _answer.c_str(), -1, NULL, 0);
   //wchar_t* wstr = new wchar_t[wchars_num];
   //wchars_num = MultiByteToWideChar(CP_UTF8, 0, _answer.c_str(), -1, wstr, wchars_num);
   //std::wstring _data;
   //_data.assign(wstr);
   //delete[] wstr;

   //return true;
}

bool HttpCloseSession(HINTERNET& _hSession, HINTERNET& _hConnect, HINTERNET& _hRequest)
{
   bool isClosed{};

   if (_hRequest)
      isClosed = (TRUE == WinHttpCloseHandle(_hRequest));

   if (_hConnect)
      isClosed &= (TRUE == WinHttpCloseHandle(_hConnect));

   if (_hSession)
      isClosed &= (TRUE == WinHttpCloseHandle(_hSession));

   return isClosed;
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

