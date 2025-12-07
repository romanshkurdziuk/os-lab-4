cmake_minimum_required(VERSION 3.10)

project(SyncProcessLab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Создаем библиотеку Core (она не запускается сама, её используют другие)
add_library(core core.cpp)

# 2. Собираем Receiver и привязываем к нему библиотеку core
add_executable(Receiver Receiver.cpp)
target_link_libraries(Receiver core)

# 3. Собираем Sender и привязываем к нему библиотеку core
add_executable(Sender Sender.cpp)
target_link_libraries(Sender core)

# 4. Тест (Integration Test)
add_executable(TestQueue Test.cpp)
target_link_libraries(TestQueue core)