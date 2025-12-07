#include "core.h"
#include <iostream>
#include <vector>

MessageQueue::MessageQueue(const std::string& filename, int recordsCount)
    : filename(filename), recordsCount(recordsCount) 
    {
    
    initializeFile();

    hMutex = CreateMutexA(NULL, FALSE, MUTEX_NAME);
    hSemEmpty = CreateSemaphoreA(NULL, recordsCount, recordsCount, SEM_EMPTY_NAME);
    hSemFull = CreateSemaphoreA(NULL, 0, recordsCount, SEM_FULL_NAME);

    if (!hMutex || !hSemEmpty || !hSemFull) 
    {
        throw std::runtime_error("Failed to create synchronization objects.");
    }
}

MessageQueue::MessageQueue(const std::string& filename)
    : filename(filename) 
    {
    
    hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
    hSemEmpty = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_EMPTY_NAME);
    hSemFull = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_FULL_NAME);

    if (!hMutex || !hSemEmpty || !hSemFull) 
    {
        throw std::runtime_error("Failed to connect to sync objects. Is Receiver running?");
    }

    std::ifstream file(filename, std::ios::binary | std::ios::ate); 
    if (!file.is_open()) 
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    int fileSize = (int)file.tellg();
    this->recordsCount = (fileSize - 2 * sizeof(int)) / sizeof(Message);
}

MessageQueue::~MessageQueue() 
{
    if (hMutex) CloseHandle(hMutex);
    if (hSemEmpty) CloseHandle(hSemEmpty);
    if (hSemFull) CloseHandle(hSemFull);
}

void MessageQueue::initializeFile() 
{
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Could not create file: " + filename);
    }

    int zero = 0;
    file.write((char*)&zero, sizeof(int)); 
    file.write((char*)&zero, sizeof(int)); 

    Message emptyMsg = {};
    for (int i = 0; i < recordsCount; ++i) 
    {
        file.write((char*)&emptyMsg, sizeof(Message));
    }
}

void MessageQueue::send(const Message& msg) 
{
    WaitForSingleObject(hSemEmpty, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    
    int writePos;
    file.seekg(0, std::ios::beg);
    file.read((char*)&writePos, sizeof(int));

    int offset = 2 * sizeof(int) + writePos * sizeof(Message);
    file.seekp(offset, std::ios::beg);
    file.write((char*)&msg, sizeof(Message));

    writePos = (writePos + 1) % recordsCount;
    file.seekp(0, std::ios::beg);
    file.write((char*)&writePos, sizeof(int));
    file.flush();

    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemFull, 1, NULL);
}

Message MessageQueue::receive() 
{
    WaitForSingleObject(hSemFull, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);

    int readPos;
    file.seekg(sizeof(int), std::ios::beg);
    file.read((char*)&readPos, sizeof(int));

    int offset = 2 * sizeof(int) + readPos * sizeof(Message);
    Message msg;
    file.seekg(offset, std::ios::beg);
    file.read((char*)&msg, sizeof(Message));

    readPos = (readPos + 1) % recordsCount;
    file.seekp(sizeof(int), std::ios::beg);
    file.write((char*)&readPos, sizeof(int));
    file.flush();

    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemEmpty, 1, NULL);

    return msg;
}