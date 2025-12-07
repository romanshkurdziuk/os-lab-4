#pragma once
#include <windows.h>
#include <string>
#include <fstream>
#include <stdexcept>
#include "logic.h"

// Класс, который инкапсулирует (прячет) всю работу с Windows API и файлами
class MessageQueue {
public:
    // Конструктор для Receiver (СОЗДАЕТ очередь и файл)
    MessageQueue(const std::string& filename, int recordsCount);

    // Конструктор для Sender (ПОДКЛЮЧАЕТСЯ к существующей очереди)
    MessageQueue(const std::string& filename);

    // Деструктор (Закрывает дескрипторы)
    ~MessageQueue();

    // Метод отправки (вызовет Sender)
    void send(const Message& msg);

    // Метод получения (вызовет Receiver)
    Message receive();

private:
    std::string filename;
    int recordsCount;
    HANDLE hMutex;
    HANDLE hSemEmpty;
    HANDLE hSemFull;

    // Вспомогательная функция для инициализации файла
    void initializeFile();
};