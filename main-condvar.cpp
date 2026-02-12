#include <cstdio>
#include <miosix.h>

using Pin = miosix::Gpio<GPIOB_BASE, 0>;

#define DBG(num) do {} while(0)
// #define DBG(num) printf("%d\n", num)

miosix::FastMutex mutex;
miosix::ConditionVariable condvar;

void other(void*) {
    DBG(2);
    miosix::Thread::yield();
    DBG(5);
    Pin::high();
    DBG(6);
    condvar.signal();
    DBG(7);
    miosix::Thread::yield();
}

int main() {
    Pin::mode(miosix::Mode::OUTPUT);
    Pin::speed(miosix::Speed::_100MHz);

    miosix::Thread::create(other, 16 * 1024, miosix::DEFAULT_PRIORITY, nullptr);
    DBG(1);
    miosix::Thread::yield();
    DBG(3);
    mutex.lock();
    DBG(4);
    condvar.wait(mutex);
    DBG(8);
    Pin::low();
}