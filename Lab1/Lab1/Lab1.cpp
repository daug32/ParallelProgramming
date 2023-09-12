#include <windows.h>
#include <string>
#include <iostream>

struct ThreadData
{
    int ThreadNumber;
    
    ThreadData(int threadIndex)
        : ThreadNumber(threadIndex)
    {
    }
};

DWORD WINAPI ThreadProc(void* pointerToData)
{
    ThreadData threadData = *static_cast<ThreadData*>(pointerToData);
    
    std::cout << "Thread " << threadData.ThreadNumber << std::endl;
    
    ExitThread(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    // Parse number of threads
    if ( argc != 2 )
    {
        std::cout << "Expected arguments: <number of threads>" << std::endl;
        return EXIT_FAILURE;
    }
    
    int threadsNumber = atoi(argv[1]);
    if ( threadsNumber < 0 )
    {
        std::cout << "Number of threads can't be less then 0" << std::endl;
        return EXIT_FAILURE;
    } 
    
    std::cout << "Number of threads: " << threadsNumber << std::endl;

    // Create threads
    auto handles = new HANDLE[threadsNumber];
    auto handlesData = new ThreadData*[threadsNumber];
    
    for (int i = 0; i < threadsNumber; i++)
    {
        handlesData[i] = new ThreadData(i);
        handles[i] = CreateThread(nullptr, 0, &ThreadProc, handlesData[i], CREATE_SUSPENDED, nullptr);
    }

    // Start threads
    for (int i = 0; i < threadsNumber; i++)
    {
        ResumeThread(handles[i]);
    }

    // Wait for threads
    WaitForMultipleObjects(threadsNumber, handles, true, INFINITE);

    // Exit
    delete[] handlesData;

    return EXIT_SUCCESS;
}

