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

		bool IsAnswerEmpty();
		std::string GetAnswer();
		std::istringstream GetAnswerAsStrings();

	private:
		RequestResult m_result;

		class Impl;
		std::unique_ptr<Impl> m_pImpl;

	};
}

