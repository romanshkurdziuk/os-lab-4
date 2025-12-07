#pragma once
#include <windows.h>
#include <string>
#include <fstream>
#include <stdexcept>
#include "logic.h"

class MessageQueue 
{
    public:
       
        MessageQueue(const std::string& filename, int recordsCount);

        MessageQueue(const std::string& filename);

        ~MessageQueue();

        void send(const Message& msg);

        Message receive();

    private:
        std::string filename;
        int recordsCount;
        HANDLE hMutex;
        HANDLE hSemEmpty;
        HANDLE hSemFull;

        void initializeFile();
};