cmake_minimum_required(VERSION 3.10)

project(SyncProcessLab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(core src/core.cpp)

target_include_directories(core PUBLIC include)

add_executable(Receiver src/Receiver.cpp)
target_link_libraries(Receiver core)

add_executable(Sender src/Sender.cpp)
target_link_libraries(Sender core)

add_executable(TestQueue tests/Test.cpp)
target_link_libraries(TestQueue core)