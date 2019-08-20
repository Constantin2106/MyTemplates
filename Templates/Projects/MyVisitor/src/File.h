#pragma once

#include <iostream>

#include "FileService.h"

class File
{
public:
    File() 
        : m_opened(false)
        , m_readed(false)
        , m_writed(false){}
    ~File() {}

    virtual void Accept(FileService&) = 0;

    void Opened() { m_opened = true; }
    void Readed() { m_readed = true; }
    void Writed() { m_writed = true; }

    bool isOpened() { return m_opened; }
    bool isReaded() { return m_readed; }
    bool isWrited() { return m_writed; }

private:
    bool m_opened;
    bool m_readed;
    bool m_writed;
};

class TextFile : public File
{
public:
    TextFile() {}
    ~TextFile() {}

    void Accept(FileService& service) override 
    { 
        std::cout << "Call service" << std::endl; 
        
        service.Visit(*this); 

        std::cout << "IsOpened = " << isOpened() << std::endl;
        std::cout << "IsReaded = " << isReaded() << std::endl;
        std::cout << "IsWrited = " << isWrited() << std::endl;
    }
};

class BinFile : public File
{
public:
    BinFile() {}
    ~BinFile() {}

    void Accept(FileService& service) override 
    { 
        std::cout << "Call service" << std::endl; 

        service.Visit(*this);

        std::cout << "IsOpened = " << isOpened() << std::endl;
        std::cout << "IsReaded = " << isReaded() << std::endl;
        std::cout << "IsWrited = " << isWrited() << std::endl;
    }
};


