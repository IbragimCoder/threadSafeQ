#include <iostream>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>


class threadSafeQ
{
    std::queue<int> rawQ;
    std::mutex m;
public:
    int& retrieveAndDelete()
    {
        int front_value = 0;
        m.lock();
        if (!rawQ.empty())
        {
            front_value = rawQ.front();
            rawQ.pop();
        }
        m.unlock();
        return front_value;
    }

    threadSafeQ(std::queue<int> rawQ)
    {
        this->rawQ = rawQ;
    }

    void print()
    {
        m.lock();
        std::cout << "queue elements : ";
        while (!rawQ.empty())
        {
            std::cout << rawQ.front() << " ";
            rawQ.pop();
        }
        std::cout << std::endl;
        m.unlock();
    }

    void push(int val)
    {
        m.lock();
        rawQ.push(val);
        m.unlock();
    }
};

int main()
{  
    std::queue<int> raw1;
    for (int i = 0; i <= 5; i++)
    {
        raw1.push(i);
    }

    threadSafeQ object1(raw1);
    std::thread funtest1(&threadSafeQ::retrieveAndDelete, &object1);

    std::queue<int> raw2;
    for (int i = 5; i <= 10; i++)
    {
        raw2.push(i);
    }

    threadSafeQ object2(raw2);
    std::thread funtest2(&threadSafeQ::retrieveAndDelete, &object2);

    if (funtest1.joinable())
    {
        funtest1.join();
    }

    if (funtest2.joinable())
    {
        funtest2.join();
    }

    object1.print();
    object2.print();

    return 0;
}   

