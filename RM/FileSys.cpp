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

int FileSystem::generateNewDescriptor(std::string filename)
{
    IOControl control = IOControl();
    auto data = control.readAllDriveData();

    std::vector<int> strippedData;
    for(int i : data)
    {
        if(i != -1)
            strippedData.insert(strippedData.end(), i);
        else
            break;
    }

    fileDescriptor fd;
    fd.name = filename;
    fd.type = 1454;
    fd.size = 0;
    std::vector<int> dataToInsert;
    dataToInsert.insert(dataToInsert.end(), fd.type);
    dataToInsert.insert(dataToInsert.end(), fd.name.length());
    std::vector<char> strVec(filename.begin(), filename.end());
    for(auto c : strVec)
    {
        dataToInsert.insert(dataToInsert.end(), (int)c);
    }

    dataToInsert.insert(dataToInsert.end(), 0);
    dataToInsert.insert(dataToInsert.end(), -2);

    strippedData.insert(strippedData.end(), dataToInsert.begin(), dataToInsert.end());
    bool res = control.modifyDriveData(strippedData);
    if(res)
    {
        initializeFileIndex();
        return getIndexByName(fd.name);
    }
    else
    {
        return -3;
    }
}

FileSystem::~FileSystem()
{}

int FileSystem::getIndexByName(std::string filename)
{
    int i = 0;
    for(auto ind : fileIndex)
    {
        if(ind.name == filename)
            return i;
        i++;
    }
    return -3;
}

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