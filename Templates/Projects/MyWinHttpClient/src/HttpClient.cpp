
#include <sstream>

#include "HttpClient.h"
#include "HttpClientHelper.h"

#pragma comment(lib, "winhttp.lib")

namespace http
{
	class HttpClient::Impl
	{
		RequestData m_request;

		std::wstring m_headers;
		std::string m_content;

	public:
		void SetParam(RequestData&& _rqData)
		{
			m_request = std::move(_rqData);
			m_request.connect.port = m_request.createReq.requestFlags & FLAGS(SECURE) ? 
										PORT(HTTPS_PORT) : PORT(HTTP_PORT);
		}

		RequestResult syncRequest()
		{
			RequestResult result;

			auto secureConnection = !!(m_request.createReq.requestFlags & FLAGS(SECURE));

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

			if (secureConnection)
			{
				m_request.sendReq.context = (DWORD_PTR)(&result);
			}

			if (!HttpSendRequest(hRequest, m_request.sendReq))
			{
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

			if (!HttpReadAnswer(hRequest, m_content, result.message))
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

		bool IsContentEmpty() { return m_content.empty(); }
		std::string GetContent() { return m_content; }
		std::istringstream GetContentAsStrings() { return std::istringstream(m_content); }
	};


	//------------------------------------------------------------------------//
	HttpClient::HttpClient() : m_pImpl(std::make_unique<Impl>()) {}

	HttpClient::~HttpClient() = default;

	void HttpClient::SetParam(RequestData&& _rqData)
	{
		m_pImpl->SetParam(std::move(_rqData));
	}

	RequestResult HttpClient::SyncRequest()
	{
		return m_pImpl->syncRequest();
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

	bool HttpClient::IsContentEmpty()
	{
		return m_pImpl->IsContentEmpty();
	}

	std::string HttpClient::GetContent()
	{
		return m_pImpl->GetContent();
	}

	std::istringstream HttpClient::GetContentAsStrings()
	{
		return m_pImpl->GetContentAsStrings();
	}
}
