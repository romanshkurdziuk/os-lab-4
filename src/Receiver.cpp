#include <iostream>
#include <vector>
#include <windows.h>
#include "core.h"

int main() 
{
    std::string filename;
    int recordsCount;
    int senderCount;

    std::cout << "Enter filename: ";
    std::cin >> filename;
    std::cout << "Enter records count: ";
    std::cin >> recordsCount;
    std::cout << "Enter senders count: ";
    std::cin >> senderCount;

    try 
    {
        MessageQueue queue(filename, recordsCount);
        std::cout << "Queue initialized.\n";

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        std::string cmdLine = "Sender.exe " + filename;
        
        for (int i = 0; i < senderCount; ++i) 
        {
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            std::vector<char> buf(cmdLine.begin(), cmdLine.end());
            buf.push_back(0);
            
            if (CreateProcessA(NULL, &buf[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) 
            {
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
        }

        while (true) 
        {
            int choice;
            std::cout << "1. Read\n0. Exit\n> ";
            std::cin >> choice;
            if (choice == 0) break;
            
            std::cout << "Waiting...\n";
            Message msg = queue.receive(); 
            std::cout << "Received: " << msg.text << std::endl;
        }

    } catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}