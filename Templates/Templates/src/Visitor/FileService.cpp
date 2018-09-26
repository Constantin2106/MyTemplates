#include "File.h"
#include "FileService.h"

//=========== FileOpener ===========//
bool FileOpener::Visit(TextFile& file)
{
    std::cout << "\tOpen Text File" << std::endl;
    file.Opened();
    return true;
}
bool FileOpener::Visit(BinFile& file)
{ 
    std::cout << "\tOpen Binary File" << std::endl; 
    file.Opened();
    return true;
}

//=========== FileReader ===========//
bool FileReader::Visit(TextFile& file)
{ 
    std::cout << "\tRead Text File" << std::endl; 
    file.Readed();
    return true;
}
bool FileReader::Visit(BinFile& file)
{ 
    std::cout << "\tRead Binary File" << std::endl; 
    file.Readed();
    return true;
}

//=========== FileWriter ===========//
bool FileWriter::Visit(TextFile& file)
{ 
    std::cout << "\tWrite Text File" << std::endl; 
    file.Writed();
    return true;
}
bool FileWriter::Visit(BinFile& file)
{ 
    std::cout << "\tWrite Binary File" << std::endl; 
    file.Writed();
    return true;
}