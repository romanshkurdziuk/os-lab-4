#include <iostream>
#include <windows.h>
#include <string>
#include "fstream"
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
    cout << "Sender connected to file and sync objects." << endl;
    cout << "Ready to work!" << endl;
    cin.get();
    return 0;
}