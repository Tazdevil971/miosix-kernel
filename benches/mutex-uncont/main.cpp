#include <cstdio>
#include <miosix.h>

using Pin = miosix::Gpio<GPIOB_BASE, 1>;

#define DBG(num) do {} while(0)
// #define DBG(num) printf("%d\n", num)

miosix::FastMutex mutex;

int main() {
    Pin::mode(miosix::Mode::OUTPUT);
    Pin::speed(miosix::Speed::_100MHz);
    Pin::low();
    
    {
        mutex.lock();
        mutex.unlock();
    }
    
    Pin::high();
    {
        mutex.lock();
        mutex.unlock();
    }
    Pin::low();
    
    while (1) {}
}