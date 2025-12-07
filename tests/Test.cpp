#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include "core.h"

#define RUN_TEST(func) \
    try { \
        std::cout << "[RUNNING] " << #func << "... "; \
        func(); \
        std::cout << "PASSED" << std::endl; \
    } catch (const std::exception& e) { \
        std::cout << "FAILED! Error: " << e.what() << std::endl; \
        exit(1); \
    }

void testSimpleTransfer() 
{
    std::string filename = "test_simple.bin";
    MessageQueue queue(filename, 5);

    Message msgIn;
    strncpy(msgIn.text, "Hello", 20);
    
    queue.send(msgIn);
    Message msgOut = queue.receive();

    assert(strcmp(msgOut.text, "Hello") == 0);
}

void testCircularLogic() 
{
    std::string filename = "test_circle.bin";
    int size = 2;
    MessageQueue queue(filename, size);

    Message m1, m2, m3;
    strncpy(m1.text, "Msg1", 20);
    strncpy(m2.text, "Msg2", 20);
    strncpy(m3.text, "Msg3", 20);

    queue.send(m1);
    queue.send(m2); 

    Message r1 = queue.receive(); 
    assert(strcmp(r1.text, "Msg1") == 0);

    queue.send(m3); 

    Message r2 = queue.receive();
    assert(strcmp(r2.text, "Msg2") == 0);
    
    Message r3 = queue.receive();
    assert(strcmp(r3.text, "Msg3") == 0);
}

void testDataIntegrity() 
{
    std::string filename = "test_data.bin";
    MessageQueue queue(filename, 5);

    Message m1;
    const char* complexStr = "Test 123 !@#";
    strncpy(m1.text, complexStr, 20);

    queue.send(m1);
    Message r1 = queue.receive();

    assert(strcmp(r1.text, complexStr) == 0);
}

int main() 
{
    std::cout << "=== STARTING UNIT TESTS ===" << std::endl;

    RUN_TEST(testSimpleTransfer);
    RUN_TEST(testCircularLogic);
    RUN_TEST(testDataIntegrity);

    std::cout << "=== ALL TESTS PASSED SUCCESSFULLY ===" << std::endl;
    return 0;
}