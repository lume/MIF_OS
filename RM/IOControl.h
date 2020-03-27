#pragma once

#include <array>
#include <vector>
// IOControl is responsible for accessing disk and managing keyboard

#define DISK_NAME "devDrv.txt"
#define DISK_SIZE 1048576
#define PAGE_SIZE 4096
#define SECTOR_SIZE 512
#define SECTOR_COUNT 1048576 / SECTOR_SIZE
#define CHAR_BUFFER 4096 // amount of characters that can be displayed
//sectors are fixed size: 512 entries (or bytes)

struct Sector
{
    bool used;
    int startAddress;
};

inline std::array<Sector, SECTOR_COUNT> sectors; 

class IOControl
{
    public:
        IOControl();
        
        //These things should be launched in threads
        void WriteData(int frameNumber, std::array<int, PAGE_SIZE>);
        std::array<int, PAGE_SIZE> ReadData(int frameNumber); // returns an array of data from a disk

        std::vector<char> ReadFromCharBuffer(int start, int end);
        void WriteIntoCharBuffer(int start, std::vector<char> data);

        // Initialize a disk if it still does not exist
        void InitDisk();
    private:
        bool DriveExists();
};