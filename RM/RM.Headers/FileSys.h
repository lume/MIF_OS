#pragma once

#include <string>
#include <vector>

typedef struct fileDescriptor
{
    int type;
    int size;
    std::string name;
} fileDescriptor;

inline std::vector<fileDescriptor> fileIndex;

class FileSystem
{
    public:
        FileSystem();
        ~FileSystem();

        std::string getFileDescriptorString(int index);
        bool createNewFile(fileDescriptor file); // TODO: add interrupt for that
        fileDescriptor generateNewDescriptor(); // TODO: add interupt for that
        bool deleteFile(fileDescriptor file); // TODO: add interrupt for that
        bool modifyFile(fileDescriptor file); // TODO: Add interrrupt for that
        bool pipeToFile(fileDescriptor file, std::vector<int> data); // TODO: add interrupt for that
        std::string getFilenamesInDrive(); // TODO: add interrupt for that {ls}
        void initializeFileIndex(); // call if after each drive modification

    private:
};