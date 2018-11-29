#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

class IOneDriveCommunicator
{
public:
    IOneDriveCommunicator();
    ~IOneDriveCommunicator();

    virtual UINT Registry() = 0;
    virtual UINT Connect() = 0;
    virtual UINT Send() = 0;

    virtual UINT Read() = 0;
    virtual UINT Write() = 0;

private:

};

IOneDriveCommunicator::IOneDriveCommunicator()
{
}

IOneDriveCommunicator::~IOneDriveCommunicator()
{
}
