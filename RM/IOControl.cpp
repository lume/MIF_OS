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
    int dat;
    std::vector<int> driveData;
    std::ifstream file(DRIVE);
    pthread_mutex_lock(&swapMutex);
    if(file.is_open())
    {
        while(file >> dat)
        {
            driveData.insert(driveData.end(), dat);
        }
        file.close();
    } 
    pthread_mutex_unlock(&swapMutex);

    //TODO: refactor
    int targetCodeLocation = -1;
    int iter = 0;
    int targetCodeSize = 0;
    for(auto i : driveData)
    {
        if(i == 1453)
        {
            int x = programName.size();
            int y = driveData[iter+1];
            if(x == y)
            {
                std::vector<char> t;
                for(int j = iter+2; j <= iter+1+driveData[iter+1]; j++)
                {
                    t.insert(t.end(), driveData[j]);
                }
                std::string a(t.begin(), t.end());
                if(strcmp(a.c_str(), programName.c_str()) == 0)
                {
                    targetCodeSize = driveData[iter+1+ a.length()+1];
                    targetCodeLocation = iter+driveData[iter+1]+3;
                    break;
                }
            }
        }
        iter++;
    }

    std::vector<int> code;
    for(int i = 0; i < targetCodeSize; i++)
    {
        code.insert(code.end(), driveData[targetCodeLocation+i]);
    }   

    return code;
}

std::vector<std::vector<int>> IOControl::SplitDriveDataIntoProgramPieces()
{
    int dat;
    std::vector<int> driveData;
    std::ifstream file(DRIVE);
    pthread_mutex_lock(&swapMutex);
    if(file.is_open())
    {
        while(file >> dat)
        {
            driveData.insert(driveData.end(), dat);
        }
        file.close();
    } 
    pthread_mutex_unlock(&swapMutex);

    std::vector<std::vector<int>> programPieces;

    bool fileStarted = false;
    std::vector<int> programPiece;
    for(int i : driveData)
    {
        
        if(i == -2)
        {
            programPieces.insert(programPieces.end(), programPiece);
            programPiece.clear();
        }
        else if(i != -1)
            programPiece.insert(programPiece.end(), i);
    }

    return programPieces;
}