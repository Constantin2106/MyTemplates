#pragma once

#include <memory>

class PImpl
{
public:
    explicit PImpl();
    ~PImpl();

    void Set();
    bool Check();
    bool Execute();

protected:

private:
    class Impl;

    std::unique_ptr<Impl> m_Imp;
};
