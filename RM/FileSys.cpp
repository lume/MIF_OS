#include "FileSys.h"
#include "IOControl.h"

FileSystem::FileSystem()
{
    initializeFileIndex();
}

//TODO: rework this to return vector of prepared file descriptor strings
fileDescriptor getFileByName(std::string name)
{
}

FileSystem::~FileSystem()
{}

void FileSystem::initializeFileIndex()
{
    IOControl io = IOControl();

    auto pieces = io.SplitDriveDataIntoProgramPieces();
    
    for(auto piece : pieces)
    {
        fileDescriptor fd;
        fd.type = piece[0];
        int namelen = piece[1];
        std::vector<int> nameData;
        for(int i = 0; i < namelen; i++)
        {
            nameData.insert(nameData.end(), piece[2+i]);
        }
        std::string a(nameData.begin(), nameData.end());
        fd.name = a;
        fd.size = piece.size();
        fileIndex.insert(fileIndex.end(), fd);
    }
}   