
#include <sstream>

#include "HttpClient.h"
#include "HttpClientHelper.h"

#pragma comment(lib, "winhttp.lib")

class HttpClient::Impl
{
	RequestData m_request;

	DWORD m_error;
	std::wstring m_headers;
	std::string m_answer;

public:
	void SetParam(RequestData&& _rqData)
	{
		m_request = std::move(_rqData);
		m_request.connect.port = m_request.createReq.requestFlags & FLAGS(SECURE)
			? PORT(HTTPS_PORT) : PORT(HTTP_PORT);
	}

	RequestResult syncRequest()
	{
		RequestResult result;

		HINTERNET hSession{};
		if (!HttpOpenSyncSession(m_request.session, hSession))
		{
			RETURN_ERROR(result);
		}

		HINTERNET hConnect{};
		if (!HttpConnectToServer(hSession, m_request.connect, hConnect))
		{
			RETURN_ERROR(result);
		}

		HINTERNET hRequest{};
		if (!HttpCreateRequest(hConnect, m_request.createReq, hRequest))
		{
			RETURN_ERROR(result);
		}

		m_error = 0;
		m_request.sendReq.context = (DWORD_PTR)(&m_error);
		if (!HttpSendRequest(hRequest, m_request.sendReq))
		{
			result.status = m_error;
			RETURN_ERROR(result);
		}

		if (!HttpWaitAnswer(hRequest))
		{
			RETURN_ERROR(result);
		}

		if (!HttpReadHeaders(hRequest, m_request.resHeaders, m_headers))
		{
			RETURN_ERROR(result);
		}

		if (!HttpReadAnswer(hRequest, m_answer, result.message))
		{
			RETURN_ERROR(result);
		}

		// Close all opened handles
		if (!HttpCloseSession(hSession, hConnect, hRequest))
		{
			RETURN_ERROR(result);
		}

		return result;
	}

	bool IsHeadersEmpty() { return m_headers.empty(); }
	std::wstring GetHeaders() { return m_headers; }
	std::wistringstream GetHeadersAsStrings() { return std::wistringstream(m_headers); }

	bool IsAnswerEmpty() { return m_answer.empty(); }
	std::string GetAnswer() { return m_answer; }
	std::istringstream GetAnswerAsStrings() { return std::istringstream(m_answer); }
};


//------------------------------------------------------------------------//
HttpClient::HttpClient() : m_pImpl(std::make_unique<Impl>()) {}

HttpClient::~HttpClient() = default;

void HttpClient::SetParam(RequestData&& _rqData)
{
	m_pImpl->SetParam(std::move(_rqData));
}

bool HttpClient::SyncRequest()
{
	auto res = m_pImpl->syncRequest();
	if (!res.success)
	{

	}

	return res.success;
}

bool HttpClient::IsHeadersEmpty()
{
	return m_pImpl->IsHeadersEmpty();
}

std::wstring HttpClient::GetHeaders()
{
	return m_pImpl->GetHeaders();
}

std::wistringstream HttpClient::GetHeadersAsStrings()
{
	return m_pImpl->GetHeadersAsStrings();
}

bool HttpClient::IsAnswerEmpty()
{
	return m_pImpl->IsAnswerEmpty();
}

std::string HttpClient::GetAnswer()
{
	return m_pImpl->GetAnswer();
}

std::istringstream HttpClient::GetAnswerAsStrings()
{
	return m_pImpl->GetAnswerAsStrings();
}

