#include "IOControl.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <bits/stdc++.h>
#include <pthread.h>

IOControl::IOControl()
{
    InitDisk();
    int mutexStatus = pthread_mutex_init(&swapMutex, NULL);
    if(mutexStatus == -1)
    {
        std::perror("An error occured when initialising a mutex!");
    }
}

IOControl::~IOControl()
{
    pthread_mutex_destroy(&swapMutex);
}

void IOControl::WriteSwapData(int frameNumber, std::array<int, PAGE_SIZE> data)
{
    pthread_t writeThread;
    rwSwapData rwData = {frameNumber, data};
    pthread_create(&writeThread, NULL, WriteSwapDataInternal, (void*)&rwData);
    pthread_join(writeThread, NULL);
}

std::array<int, PAGE_SIZE> IOControl::ReadSwapData(int frameNumber)
{
    pthread_t readThread;
    rwSwapData rwData = {frameNumber, NULL}; // TODO: Possibly a different struct should be created
    
    void* returnedData;
    
    pthread_create(&readThread, NULL, ReadSwapDataInternal, (void*)&rwData);
    pthread_join(readThread, &returnedData);
    
    rwSwapData *resultRwData = (rwSwapData*)returnedData;
    return resultRwData->data;
}

void *(IOControl::WriteSwapDataInternal)(void* arg)
{
    rwSwapData *rwData = (rwSwapData*)arg;
    std::string swapName = DISK_DIRECTORY + std::to_string(rwData->frameNumber);
    std::ofstream disk;
    
    // Critical section:
    pthread_mutex_lock(&swapMutex);
    disk.open(swapName, std::ofstream::out | std::ofstream::trunc);
    for(int i : rwData->data)
    {
        std::string temp = std::to_string(i);
        disk << temp << " ";
    }
    disk.close();
    // End of critical section
    pthread_mutex_unlock(&swapMutex);
    return rwData;
}

void *(IOControl::ReadSwapDataInternal)(void *arg)
{
    rwSwapData *rwData = (rwSwapData*)arg;
    std::string swapName = DISK_DIRECTORY + std::to_string(rwData->frameNumber);
    std::ifstream disk;
    pthread_mutex_lock(&swapMutex);
    // Critical section:
    disk.open(swapName);

    std::string temp((std::istreambuf_iterator<char>(disk)),
                    std::istreambuf_iterator<char>());
    disk.close();
    int status = remove(swapName.c_str());
    if(status == -1)
    {
        std::perror("Could not remove swap file.");
    }
    // End of critical section
    pthread_mutex_unlock(&swapMutex);
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
    rwData->data = data;
    return rwData;
}

void IOControl::PrintCharBuffer(){}

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

std::vector<int> IOControl::FindProgramCode(std::string programName)
{
    std::vector<int> a;
    return a;
}