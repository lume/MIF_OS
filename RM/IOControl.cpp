#include "IOControl.h"
#include <fstream>
#include <iostream>

IOControl::IOControl(){}

void IOControl::WriteData(int frameNumber, std::array<int, PAGE_SIZE>)
{
    int sectorSectionStartAddress = (frameNumber - 256) * PAGE_SIZE;
    
}

std::array<int, PAGE_SIZE> IOControl::ReadData(int frameNumber){}

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