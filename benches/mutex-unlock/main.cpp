#include <cstdio>
#include <miosix.h>

using Pin = miosix::Gpio<GPIOB_BASE, 1>;

#define DBG(num) do {} while(0)
// #define DBG(num) printf("%d\n", num)

miosix::FastMutex mutex;

void other(void*) {
    DBG(2);
    mutex.lock();
    DBG(3);
    miosix::Thread::yield();
    DBG(5);
    Pin::high();
    DBG(6);
    mutex.unlock();
    DBG(7);
    miosix::Thread::yield();
}

int main() {
    Pin::mode(miosix::Mode::OUTPUT);
    Pin::speed(miosix::Speed::_100MHz);
    Pin::low();
    
    Pin::high();
    {
        mutex.lock();
        mutex.unlock();
    }
    Pin::low();

    miosix::Thread::create(other, 16 * 1024, miosix::DEFAULT_PRIORITY, nullptr);
    DBG(1);
    miosix::Thread::yield();
    DBG(4);
    mutex.lock();
    DBG(8);
    Pin::low();
    
    while (1) {}
}