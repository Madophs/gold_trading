#ifndef __THREAD_UTILS_H__
#define __THREAD_UTILS_H__

#include <iostream>
#include <thread>
#include <atomic>
#include <pthread.h>
#include <utility>
#include <chrono>

namespace mds
{
    /** Forward declarations **/
    template <typename T, typename... A>

    inline std::thread* createAndRunThread(int coreId, std::string name, T&& func, A&&... args);

    inline int setThreadCoreId(int coreId);

    /** implementation **/
    template <typename T, typename... A>
    inline std::thread* createAndRunThread(int coreId, std::string name, T&& func, A&&... args)
    {
        std::atomic<bool> isRunning{false}, isFailed{false};
        auto threadBody = [&] {
            if (coreId >= 0 && setThreadCoreId(coreId) != 0) {
                std::cerr << "Failed to set thread affinity for " << name <<
                    pthread_self() << " to " << coreId << std::endl;
                isFailed = true;
                return;
            }

            std::cout << "Set thread affinity for " << name << " " <<
                pthread_self() << " to " << coreId << std::endl;

            std::forward<T>(func)((std::forward<A>(args))...);
            isRunning = true;
        };

        std::thread* pThread = new std::thread(threadBody);

        while (!isRunning && !isFailed) {
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

        if (isFailed) {
            pThread->join();

            delete pThread;
            pThread = nullptr;
        }

        return pThread;
    }

    inline int setThreadCoreId(int coreId)
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(coreId, &cpuset);
        return pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
    }
}

#endif
