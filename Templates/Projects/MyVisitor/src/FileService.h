#pragma once

#include <iostream>

class TextFile;
class BinFile;

class FileService
{
public:
    FileService() {}
    ~FileService(){}

    virtual bool Visit(TextFile&) = 0;
    virtual bool Visit(BinFile&) = 0;

private:
};

class FileOpener : public FileService
{
public:
    FileOpener() {}
    ~FileOpener() {}

    bool Visit(TextFile&) override;
    bool Visit(BinFile&) override;
};

class FileReader : public FileService
{
public:
    FileReader() {}
    ~FileReader() {}

    bool Visit(TextFile&) override;
    bool Visit(BinFile&) override;
};

class FileWriter : public FileService
{
public:
    FileWriter() {}
    ~FileWriter() {}

    bool Visit(TextFile&) override;
    bool Visit(BinFile&) override;
};



