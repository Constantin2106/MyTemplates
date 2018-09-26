
#include <conio.h>
#include <iostream>

#include "File.h"
#include "FileService.h"

void VisitorTest()
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

}