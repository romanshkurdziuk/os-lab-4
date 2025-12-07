#include <iostream>
#include "core.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No filename provided\n";
        return 1;
    }

    try 
    {
        MessageQueue queue(argv[1]);
        std::cout << "Connected to queue.\n";

        while (true) 
        {
            int choice;
            std::cout << "1. Send\n0. Exit\n> ";
            std::cin >> choice;
            if (choice == 0) break;

            Message msg;
            std::cout << "Text: ";
            std::cin >> msg.text;

            std::cout << "Sending...\n";
            queue.send(msg); 
            std::cout << "Sent!\n";
        }

    } catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cin.get();
        return 1;
    }
    
    return 0;
}