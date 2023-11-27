#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <thread>
#include <vector>
#include <mutex>

#include "thread_utils.hpp"
#include "memory_pool.hpp"

void func(int32_t until)
{
    for (int32_t i = 0; i < until; ++i) {
        std::cout << "Cpu #" << sched_getcpu() << ", CPU id " << std::this_thread::get_id() << " => " << i << std::endl;
    }
}

class Person
{
public:
    Person():m_name(""), m_age(0)
    {
    }

    Person(std::string name, int age):
        m_name(name), m_age(age)
    {
    }

    std::string getName() const
    {
        return m_name;
    }

    int getAge() const
    {
        return m_age;
    }

    friend std::ostream& operator<<(std::ostream& out, const Person& p)
    {
        out << p.getName() << " " << p.getAge();
        return out;
    }
private:
    std::string m_name;
    int m_age;
};

int main()
{
    //std::thread* pThread1 = mds::createAndRunThread(9, "printing thread #1", func, 10);
    //pThread1->join();
    mds::MemoryPool<Person> memoryPool(10);
    Person* p = memoryPool.allocate("jair", 22);
    std::cout << *p << std::endl;
    Person* p2 = memoryPool.allocate("jehu", 27);
    std::cout << *p2 << std::endl;
    memoryPool.deallocate(p);
    std::cout << *p << std::endl;
    return EXIT_SUCCESS;
}
