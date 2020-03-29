#include "IOControl.h"
#include <fstream>
#include <iostream>
#include <algorithm>

IOControl::IOControl(){}

void IOControl::WriteData(int frameNumber, std::array<int, PAGE_SIZE> data)
{
    //TODO: Just create new files instead of seek/replace in existing..
    int sectorSectionStartAddress = (frameNumber - 256) * PAGE_SIZE;
    std::fstream disk(DISK_NAME);
    GotoLine(disk, sectorSectionStartAddress);
    for(auto i : data)
    {
        i += 55;
        disk.write(reinterpret_cast<const char*>(&i), sizeof(i));
    }
    disk.close();
}

std::array<int, PAGE_SIZE> IOControl::ReadData(int frameNumber)
{
    int sectorSectionStartAddress = (frameNumber - 256) * PAGE_SIZE;
    std::ifstream disk;
    disk.open(DISK_NAME);
    disk.seekg(sectorSectionStartAddress);
    //char temp[PAGE_SIZE] = reinterpret_cast<const char*>(data.data());
    //std::transform(data.begin, data.end, std::begin(temp), [](int i){return '0'+i;});
    //isk.write(reinterpret_cast<const char*>(data.data()), sizeof(data.data())); // It looks a bit... messy
    
    //disk.read()

    disk.close();
}

std::vector<char> IOControl::ReadFromCharBuffer(int start, int end){}

void IOControl::WriteIntoCharBuffer(int start, std::vector<char> data){}

void IOControl::InitDisk()
{
    if(!DriveExists())
    {
        std::cout << "drive does not exist" << std::endl;
        std::ofstream disk;
        disk.open(DISK_NAME, std::ios_base::app);
        for(int i = 0; i < SECTOR_COUNT; i++)
        {
            for(int j = 0; j < SECTOR_SIZE; j++)
            {
                disk << "0 ";
            }
            disk << "\n";
        }
        disk.close();
    }
}

bool IOControl::DriveExists()
{
    std::ifstream infile(DISK_NAME); 
    return infile.good();
}

std::fstream& IOControl::GotoLine(std::fstream& file, int lineNum)
{
    file.seekg(std::ios::beg);
    for(int i = 0; i < lineNum - 1; i++)
    {
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return file;
}