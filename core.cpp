#include "core.h"
#include <iostream>
#include <vector>

// --- Конструктор для RECEIVER (Создатель) ---
MessageQueue::MessageQueue(const std::string& filename, int recordsCount)
    : filename(filename), recordsCount(recordsCount) {
    
    // 1. Создаем и инициализируем файл
    initializeFile();

    // 2. Создаем объекты синхронизации
    hMutex = CreateMutexA(NULL, FALSE, MUTEX_NAME);
    hSemEmpty = CreateSemaphoreA(NULL, recordsCount, recordsCount, SEM_EMPTY_NAME);
    hSemFull = CreateSemaphoreA(NULL, 0, recordsCount, SEM_FULL_NAME);

    // Если не удалось создать - кидаем исключение
    if (!hMutex || !hSemEmpty || !hSemFull) {
        throw std::runtime_error("Failed to create synchronization objects.");
    }
}

// --- Конструктор для SENDER (Подключающийся) ---
MessageQueue::MessageQueue(const std::string& filename)
    : filename(filename) {
    
    // 1. Открываем существующие объекты
    hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
    hSemEmpty = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_EMPTY_NAME);
    hSemFull = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_FULL_NAME);

    if (!hMutex || !hSemEmpty || !hSemFull) {
        throw std::runtime_error("Failed to connect to sync objects. Is Receiver running?");
    }

    // 2. Вычисляем recordsCount из размера файла
    std::ifstream file(filename, std::ios::binary | std::ios::ate); // ate = at the end
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    int fileSize = (int)file.tellg();
    this->recordsCount = (fileSize - 2 * sizeof(int)) / sizeof(Message);
}

// --- Деструктор ---
MessageQueue::~MessageQueue() {
    if (hMutex) CloseHandle(hMutex);
    if (hSemEmpty) CloseHandle(hSemEmpty);
    if (hSemFull) CloseHandle(hSemFull);
}

// --- Инициализация файла (Приватный метод) ---
void MessageQueue::initializeFile() {
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filename);
    }

    int zero = 0;
    file.write((char*)&zero, sizeof(int)); // Write Index
    file.write((char*)&zero, sizeof(int)); // Read Index

    Message emptyMsg = {}; // Заполняем нулями
    for (int i = 0; i < recordsCount; ++i) {
        file.write((char*)&emptyMsg, sizeof(Message));
    }
}

// --- Метод ОТПРАВКИ ---
void MessageQueue::send(const Message& msg) {
    // Ждем место и доступ
    WaitForSingleObject(hSemEmpty, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    
    // Читаем куда писать
    int writePos;
    file.seekg(0, std::ios::beg);
    file.read((char*)&writePos, sizeof(int));

    // Пишем сообщение
    int offset = 2 * sizeof(int) + writePos * sizeof(Message);
    file.seekp(offset, std::ios::beg);
    file.write((char*)&msg, sizeof(Message));

    // Обновляем индекс
    writePos = (writePos + 1) % recordsCount;
    file.seekp(0, std::ios::beg);
    file.write((char*)&writePos, sizeof(int));
    file.flush();

    // Освобождаем
    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemFull, 1, NULL);
}

// --- Метод ПОЛУЧЕНИЯ ---
Message MessageQueue::receive() {
    // Ждем сообщение и доступ
    WaitForSingleObject(hSemFull, INFINITE);
    WaitForSingleObject(hMutex, INFINITE);

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);

    // Читаем откуда читать (пропуская writePos)
    int readPos;
    file.seekg(sizeof(int), std::ios::beg);
    file.read((char*)&readPos, sizeof(int));

    // Читаем сообщение
    int offset = 2 * sizeof(int) + readPos * sizeof(Message);
    Message msg;
    file.seekg(offset, std::ios::beg);
    file.read((char*)&msg, sizeof(Message));

    // Обновляем индекс
    readPos = (readPos + 1) % recordsCount;
    file.seekp(sizeof(int), std::ios::beg);
    file.write((char*)&readPos, sizeof(int));
    file.flush();

    // Освобождаем
    ReleaseMutex(hMutex);
    ReleaseSemaphore(hSemEmpty, 1, NULL);

    return msg;
}