#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include "fstream"
#include "message.h"

using namespace std;

int main()
{
    string fileName;
    int recordsCount;
    cout << "Enter binary file name (e.g. data.bin): ";
    cin >> fileName;
    cout << "Enter records count: ";
    cin >> recordsCount;
    ofstream file(fileName, ios::binary | ios::out);
    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        return 1;
    }
    int initialIndex = 0;
    file.write((char*)& initialIndex, sizeof(int));
    file.write((char*)& initialIndex, sizeof(int));
    file.close();
    cout << "Binary file created successfully." << endl;





    int senderCount;
    cout << "Enter sender count: "<< endl;
    cin >> senderCount;
    HANDLE hMutex = CreateMutexA(NULL, FALSE, "Lab4_Mutex");
    HANDLE hSemEmpty = CreateSemaphoreA(NULL, recordsCount, recordsCount, "Lab4_SemEmpty");
    HANDLE hSemFull = CreateSemaphoreA(NULL, 0, recordsCount, "Lab4_SemFull");
    if (hMutex == NULL || hSemEmpty == NULL || hSemFull == NULL)
    {
        cerr << "Error creating synchronization objects." << std::endl;
        return 1;
    }
    cout << "Synchronization objects created." << std::endl;





    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    string cmdLine = "Sender.exe " + fileName;
    for (int i = 0; i < senderCount; ++i)
    {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        vector<char> cmdBuffer(cmdLine.begin(), cmdLine.end());
        cmdBuffer.push_back('\0');
        bool isCreated = CreateProcessA(NULL, &cmdBuffer[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
        if (isCreated)
        {
            cout << "Sender " << i + 1 << " started." << std::endl;
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else
        {
            DWORD errorCode = GetLastError();
            cerr << "Error creating sender process. Error code: " << errorCode << endl;
            return 1;
        }
    }
    cout << "All senders started." << endl;

    cout << "Press enter to exit" << endl;
    cin.ignore();
    cin.get();

    CloseHandle(hMutex);
    CloseHandle(hSemEmpty);
    CloseHandle(hSemFull);

    return 0;
}