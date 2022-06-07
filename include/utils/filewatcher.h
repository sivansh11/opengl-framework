#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <experimental/filesystem>

#include "event.h"

class FileChangeEvent : public Event
{
public:
    FileChangeEvent(const char *filePath) : filePath(filePath), Event("FILE_CHANGE") {}
    std::string filePath;
};

class FileWatcher
{
public:
    FileWatcher(const char *filePath) : filePath(filePath) 
    {
        lastWrite = std::experimental::filesystem::last_write_time(filePath);
    }

    bool hasChanged()
    {
        std::experimental::filesystem::v1::file_time_type newWrite = std::experimental::filesystem::last_write_time(filePath);
        if (newWrite != lastWrite)
        {
            lastWrite = newWrite;
            return true;
        }
        return false;
    }

    std::string filePath;
private:
    std::experimental::filesystem::v1::file_time_type lastWrite;
};


#endif