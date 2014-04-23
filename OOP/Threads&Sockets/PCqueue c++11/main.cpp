#include <sys/wait.h>
#include <iostream>
#include <thread>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
//This function will be called from a thread
static const int num_threads = 10;
std::mutex mtx;
std::condition_variable cv;

bool ready = false;
void hello(const int i)
{
    //std::unique_lock< std::mutex > lck(mtx);
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    std::cout<< __func__ << "thread " << i <<  std::endl;
}
void go()
{
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}
int main()
{
    std::cout << __func__ <<": "<< ready << '\n';
    std::thread pool[num_threads];
    for (size_t i = 0; i< num_threads; ++i)
    {
        pool[i] = std::thread(hello, i);
    }
    go();
    //Join the thread with the main thread
    for (auto & i : pool)
    i.join();
    return 0;
}

