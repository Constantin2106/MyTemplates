// MyVisitor.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include "src\File.h"
#include "src\FileService.h"

int main()
{
    TextFile tFile;
    BinFile bFile;

    FileOpener fOpener;
    FileReader fReader;
    FileWriter fWriter;

    std::cout << "Text File" << std::endl;

    tFile.Accept(fOpener);
    //fOpener.Visit(tFile);
    tFile.Accept(fReader);
    tFile.Accept(fWriter);

    std::cout << "\n\nBinary File" << std::endl;

    bFile.Accept(fOpener);
    bFile.Accept(fReader);
    bFile.Accept(fWriter);

    _getch();

    return 0;
}

