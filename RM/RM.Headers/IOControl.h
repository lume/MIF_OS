#pragma once

#include <array>
#include <vector>
#include <semaphore.h>
#include "SizeDefinitions.h"

#define DRIVE "drive"

// This mutex is to be used for file read/write operations
inline pthread_mutex_t swapMutex;

typedef struct rwSwapData
{
    int frameNumber;
    std::array<int, PAGE_SIZE> data;
} rwSwapData;

inline std::array<char, CHAR_BUFFER_SIZE> charBuffer; 
inline std::array<char, CHAR_BUFFER_SIZE> tempCharBuffer; 

class IOControl
{
    public:
        IOControl();
        ~IOControl();
        
        //These things should be launched in threads
        void WriteSwapData(int frameNumber, std::array<int, PAGE_SIZE> data);
        std::array<int, PAGE_SIZE> ReadSwapData(int frameNumber); // returns an array of data from a disk


        void PrintCharBuffer();
        void WriteIntoCharBuffer(int start, std::vector<char> data);

        // Initialize a disk if it still does not exist
        void InitDisk();
        void InitCharBuffer();

        std::vector<int> FindProgramCode(std::string programName);
    private:
        static void* WriteSwapDataInternal(void* arg);
        static void* ReadSwapDataInternal(void* arg); // returns an array of data from a disk
        bool DriveExists();
        std::fstream& GotoLine(std::fstream& file, int lineNum);
};