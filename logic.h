#pragma once

// Константы для имен объектов синхронизации
// Теперь, если мы захотим поменять имя, мы поменяем его только тут
const char* const MUTEX_NAME = "Lab4_Mutex";
const char* const SEM_EMPTY_NAME = "Lab4_SemEmpty";
const char* const SEM_FULL_NAME = "Lab4_SemFull";

// Структура сообщения
struct Message {
    char text[20];
};