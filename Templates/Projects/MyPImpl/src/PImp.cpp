
#include <iostream>

#include "PImp.h"


class PImpl::Impl
{
public:
    Impl()
    { 
        std::cout << "Implementation constructor" << std::endl; 
    }

    ~Impl()
    { 
        std::cout << "Implementation destructor" << std::endl; 
    }

    void Set()
    {
        std::cout << "Implementation Set" << std::endl;
    }

    bool Check()
    {
        bool ok = true;
        std::cout << "Implementation Check" << std::endl;
        return ok;
    }

    bool Execute()
    {
        bool ok = true;
        std::cout << "Implementation Execute" << std::endl;
        return ok;
    }

protected:

private:

};

PImpl::PImpl()
{
    std::cout << "Constructor" << std::endl;
    m_Imp = std::make_unique<Impl>();
}

PImpl::~PImpl()
{
    std::cout << "Destructor" << std::endl;
    if(m_Imp)
    {
        auto ptr = m_Imp.release();
        delete ptr;
    }
}

void PImpl::Set() 
{ 
    std::cout << "Set" << std::endl; 
    m_Imp->Set(); 
}

bool PImpl::Check() 
{ 
    std::cout << "Check" << std::endl; 
    return m_Imp->Check(); 
}

bool PImpl::Execute() 
{ 
    std::cout << "Execute" << std::endl; 
    return m_Imp->Execute(); 
}