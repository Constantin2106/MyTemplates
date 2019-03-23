#pragma once

#include <memory>
#include "HttpDataStruct.h"

namespace http
{
	class HttpClient
	{
	public:
		HttpClient();
		~HttpClient();

		void SetParam(RequestData&& _rqData);
		RequestResult SyncRequest();

		bool IsHeadersEmpty() const;
		std::wstring GetHeaders() const;
		std::wistringstream GetHeadersAsStrings() const;

		bool IsContentEmpty() const;
		std::string GetContent() const;
		std::istringstream GetContentAsStrings() const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;

	};
}

