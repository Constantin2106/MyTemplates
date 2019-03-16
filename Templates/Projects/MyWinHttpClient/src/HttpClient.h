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

		bool IsHeadersEmpty();
		std::wstring GetHeaders();
		std::wistringstream GetHeadersAsStrings();

		bool IsContentEmpty();
		std::string GetContent();
		std::istringstream GetContentAsStrings();

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;

	};
}

