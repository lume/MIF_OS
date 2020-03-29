#pragma once

#include <array>
#include <vector>
// IOControl is responsible for accessing disk and managing keyboard

#define DISK_NAME "devDrv.txt"
#define DISK_DIRECTORY "swapdisk/"
#define DISK_SIZE 1048576
#define PAGE_SIZE 4096
#define SECTOR_SIZE 4096
#define SECTOR_COUNT 1048576 / SECTOR_SIZE
#define CHAR_BUFFER_SIZE 4096 // amount of characters that can be displayed
//sectors are fixed size: 512 entries (or bytes)

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
        
        //These things should be launched in threads
        void WriteSwapData(int frameNumber, std::array<int, PAGE_SIZE> data);
        std::array<int, PAGE_SIZE> ReadSwapData(int frameNumber); // returns an array of data from a disk


        void PrintCharBuffer();
        void WriteIntoCharBuffer(int start, std::vector<char> data);

        // Initialize a disk if it still does not exist
        void InitDisk();
        void InitCharBuffer();
    private:
        static void* WriteSwapDataInternal(void* arg);
        std::array<int, PAGE_SIZE> ReadSwapDataInternal(int frameNumber); // returns an array of data from a disk
        bool DriveExists();
        std::fstream& GotoLine(std::fstream& file, int lineNum);
};