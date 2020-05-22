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
        int generateNewDescriptor(std::string filename); // TODO: add interupt for that
        bool deleteFile(std::string filename); // TODO: add interrupt for that
        bool modifyFile(std::string filename, std::string newFilename); // TODO: Add interrrupt for that
        bool pipeToFile(fileDescriptor file, std::vector<int> data); // TODO: add interrupt for that
        std::string getFilenamesInDrive(); // TODO: add interrupt for that {ls}
        void initializeFileIndex(); // call if after each drive modification

    private:
        int getIndexByName(std::string filename);
        std::vector<int> getFileDescriptorCode(fileDescriptor fd);
};