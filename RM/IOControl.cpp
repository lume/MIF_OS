#include "IOControl.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <bits/stdc++.h>

IOControl::IOControl(){}

void IOControl::WriteData(int frameNumber, std::array<int, PAGE_SIZE> data)
{
    std::string swapName = DISK_DIRECTORY + std::to_string(frameNumber);
    std::ofstream disk;
    disk.open(swapName, std::ofstream::out | std::ofstream::trunc);
    for(int i : data)
    {
        std::string temp = std::to_string(i);
        disk << temp << " ";
    }
    disk.close();
}

std::array<int, PAGE_SIZE> IOControl::ReadData(int frameNumber)
{
    std::string swapName = DISK_DIRECTORY + std::to_string(frameNumber);
    std::ifstream disk;
    disk.open(swapName);

    std::string temp((std::istreambuf_iterator<char>(disk)),
                    std::istreambuf_iterator<char>());
    disk.close();

    std::array<int, PAGE_SIZE> data;
    int dataIterator = 0;
    std::istringstream ss(temp);

    do{
        if((ss >> data[dataIterator]).fail())
        {
            std::cout << "failed Reading data from a swap:"; 
            std::perror("Error \n");
        }
        dataIterator++;
    }while(dataIterator < PAGE_SIZE);

    return data;
}

std::vector<char> IOControl::ReadFromCharBuffer(int start, int end){}

void IOControl::WriteIntoCharBuffer(int start, std::vector<char> data){}

void IOControl::InitDisk()
{
    if(!DriveExists())
    {
        int status = mkdir(DISK_DIRECTORY, 0777);

        if(status == -1)
        {
            perror("A following error occured when trying to create swap directory");
        }
    }
}

bool IOControl::DriveExists()
{
    struct stat info;
    if(stat(DISK_DIRECTORY, &info) == 0)
        if(info.st_mode & S_IFDIR)
            return true;

    return false;
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