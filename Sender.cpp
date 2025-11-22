#include <iostream>
#include <windows.h>
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
    cout << fileName << "Succesfully opened file" << endl;


    file.close();

    cout << "Press enter to exit" << endl;
    cin.ignore();
    cin.get();

    return 0;
}