#include "FileSys.h"
#include "IOControl.h"

FileSystem::FileSystem()
{
}

std::string FileSystem::getFileDescriptorString(int index)
{
    initializeFileIndex();
    fileDescriptor fd = fileIndex[index];
    std::string str;
    str += std::to_string(fd.type) + " " + fd.name + " " + std::to_string(fd.size);
    return str;
}

FileSystem::~FileSystem()
{}

void FileSystem::initializeFileIndex()
{
    IOControl io = IOControl();
    fileIndex.clear();
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