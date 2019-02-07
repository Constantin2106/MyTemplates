#pragma once

#include <memory>
#include "HttpDataStruct.h"

class HttpClient
{
public:
   HttpClient();
   ~HttpClient();

   void SetParam(RequestData&& _rqData);
   bool SyncRequest();

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

