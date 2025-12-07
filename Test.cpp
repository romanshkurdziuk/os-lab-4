#include <iostream>
#include <cassert> // Библиотека для проверок (assert)
#include <cstring>
#include "core.h"

void runTest() {
    std::string testFilename = "test_data.bin";
    std::string expectedText = "HelloWorld";
    
    std::cout << "[TEST] 1. Initializing Queue..." << std::endl;
    // Создаем очередь на 5 сообщений
    MessageQueue queue(testFilename, 5);

    std::cout << "[TEST] 2. Sending message: " << expectedText << std::endl;
    Message msgIn;
    // Копируем текст в структуру (безопасный аналог strcpy)
    strncpy(msgIn.text, expectedText.c_str(), 20);
    
    // Отправляем (эмулируем Sender)
    queue.send(msgIn);

    std::cout << "[TEST] 3. Receiving message..." << std::endl;
    // Получаем (эмулируем Receiver)
    Message msgOut = queue.receive();

    std::cout << "[TEST] 4. Verifying..." << std::endl;
    // Проверяем, что отправили то же самое, что получили
    // strcmp возвращает 0, если строки равны
    assert(strcmp(msgOut.text, expectedText.c_str()) == 0);

    std::cout << "[TEST] SUCCESS! Data matched." << std::endl;
}

int main() {
    try {
        runTest();
    } catch (const std::exception& e) {
        std::cerr << "[TEST] FAILED with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}