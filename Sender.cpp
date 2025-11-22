#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include "message.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Error: Filename not provided." << endl;
        cin.get();
        return 1;
    }
    string fileName = argv[1];
    fstream file(fileName, ios::binary | ios::in | ios::out);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file: " << fileName << endl;
        cin.get();
        return 1;
    }

    HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "Lab4_Mutex");
    HANDLE hSemEmpty = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "Lab4_SemEmpty");
    HANDLE hSemFull = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "Lab4_SemFull");

    if (hMutex == NULL || hSemEmpty == NULL || hSemFull == NULL)
    {
        cerr << "Error: Could not open synchronization objects." << endl;
        cin.get();
        return 1;
    }

    file.seekg(0, ios::end);
    int fileSize = (int)file.tellg();
    int recordsCount = (fileSize - 2 * sizeof(int)) / sizeof(Message);
    cout << "Connected. Queue size: " << recordsCount << endl;
    int command;
    while(true)
    {
        cout << "1. Send message\n0. Exit\nChoice: ";
        cin >> command;
        if (command == 0)
        {
            break;
        }
        if (command != 1) continue;
        Message msg;
        cout << "Enter message (max 20 symbols): " ;
        cin >> msg.text;
        cout << "Waiting for free text..." << endl;
        WaitForSingleObject(hSemEmpty, INFINITE);
        WaitForSingleObject(hMutex, INFINITE);
        int writePos;
        file.seekg(0, ios::beg);
        file.read((char*)&writePos, sizeof(int));
        int offset = 2 * sizeof(int) + writePos * sizeof(Message);
        file.seekp(offset, ios::beg);
        file.write((char*)&msg, sizeof(Message));
        writePos = (writePos + 1) % recordsCount;
        file.seekp(0, ios::beg);
        file.write((char*)& writePos, sizeof(int));
        ReleaseMutex(hMutex);
        ReleaseSemaphore(hSemFull, 1, NULL);
        cout << "Message sent!" << endl;
    }
    file.close();
    CloseHandle(hMutex);
    CloseHandle(hSemEmpty);
    CloseHandle(hSemFull);
    cin.get();
    return 0;
}