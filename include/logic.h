#pragma once

const char* const MUTEX_NAME = "Lab4_Mutex";
const char* const SEM_EMPTY_NAME = "Lab4_SemEmpty";
const char* const SEM_FULL_NAME = "Lab4_SemFull";

struct Message 
{
    char text[20];
};