#include <iostream>
#include <vector>

#include "HttpClientHelper.h"

static void CALLBACK HttpClientCallback(
                HINTERNET hInternet,
                DWORD_PTR context,
                DWORD status,
                LPVOID statusInfo,
                DWORD statusInfoLength)
{
    if (WINHTTP_CALLBACK_FLAG_SECURE_FAILURE == status)
    {
		std::cout << std::endl << std::endl << "Secure failure" << std::endl;
		DWORD stInfo = *((DWORD*)statusInfo);

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
    }
}

bool HttpOpenSyncSession(HINTERNET& hSession, const SessionData& session)
{
    hSession = ::WinHttpOpen(
                session._agent.c_str(),
                session._accessType,
                session._proxyName,
                session._proxyBypass,
                session._sessionFlags);

    return NULL != hSession;
}

bool HttpConnectToServer(HINTERNET& hCconnect, const HINTERNET hSession, const ConnectData& connect)
{
    hCconnect = ::WinHttpConnect(
                 hSession,
                 connect._server.c_str(),
                 connect._port, 0);

    return NULL != hCconnect;
}

bool HttpCreateRequest(HINTERNET& hRequest, const HINTERNET hCconnect, const CreateRequest& createReq)
{
    hRequest = ::WinHttpOpenRequest(
                hCconnect,
                createReq._verb,
                createReq._objName.c_str(),
                createReq._version,
                createReq._referrer,
                createReq._acceptTypes,
                createReq._requestFlags);

    return NULL != hRequest;
}

bool HttpAddHeaders(const HINTERNET& hRequest, const RequestHeaders& reqHeaders)
{
    return TRUE == ::WinHttpAddRequestHeaders(
                    hRequest,
                    reqHeaders._headers.c_str(),
                    ULONG(-1L),
                    reqHeaders._modifiers);
}

bool HttpSendRequest(const HINTERNET hRequest, const SendRequest& sendReq, bool checkCertificate/* = false*/)
{
    if (checkCertificate)
    {
        WINHTTP_STATUS_CALLBACK isCallback = ::WinHttpSetStatusCallback(
                            hRequest,
                            reinterpret_cast<WINHTTP_STATUS_CALLBACK>(HttpClientCallback),
                            WINHTTP_CALLBACK_FLAG_SECURE_FAILURE, NULL);

        if (WINHTTP_INVALID_STATUS_CALLBACK == isCallback)
        {
            auto err = GetLastError();

        }
    }

    return TRUE == ::WinHttpSendRequest(
                    hRequest,
                    sendReq._headers,
                    sendReq._headersLength,
                    sendReq._optional,
                    sendReq._optionalLength,
                    sendReq._totalLength,
                    sendReq._context);
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
        resHeaders._infoLevel,
        resHeaders._name,
        NULL, &hdSize,
        resHeaders._index))
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
    bResult = ::WinHttpQueryHeaders(
                hRequest,
                resHeaders._infoLevel,
                resHeaders._name,
                buffer, &hdSize,
                resHeaders._index);

    if (bResult)
    {
        headers.assign(reinterpret_cast<PTCHAR>(buffer));
    }

    return TRUE == bResult;
}

bool HttpReadAnswer(const HINTERNET& hRequest, std::string& answer, std::wstring& errorMessage)
{
    DWORD dataSize{};
    DWORD downloadedData{};
    BYTE* dataBuffer{};

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
        try
        {
            dataBuffer = new BYTE[dataSize + 1];
        }
        catch (std::bad_alloc&)
        {
            errorMessage.assign(_T("Out of memory\n"));
            return false;
        }

        ZeroMemory(dataBuffer, dataSize + 1);

        // Read the data
        if (!::WinHttpReadData(hRequest, dataBuffer, dataSize, &downloadedData))
        {
            errorMessage = std::wstring(_T("Error "))
                + std::to_wstring(GetLastError())
                + std::wstring(_T(" in WinHttpReadData.\n"));
            return false;
        }

        answer.append(reinterpret_cast<PCHAR>(dataBuffer));

        // Free the memory allocated to the buffer
        delete[] dataBuffer;

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

    //return true;
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

