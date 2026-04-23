#include <cstdio>
#include "miosix.h"

using namespace std;
using namespace miosix;

using Mco2 = miosix::Gpio<GPIOC_BASE, 9>;
using Pin = miosix::Gpio<GPIOB_BASE, 1>;

#define DBG(num) do {} while(0)
// #define DBG(num) printf("%d\n", num)

void other(void*) {
    DBG(2);
    miosix::Thread::yield();
    DBG(5);
    Pin::low();
}

int main() {
    Mco2::mode(miosix::Mode::ALTERNATE);
    Mco2::alternateFunction(0);

    Pin::mode(miosix::Mode::OUTPUT);
    Pin::speed(miosix::Speed::_100MHz);
    Pin::low();
    
    miosix::Thread::create(other, 16 * 1024, miosix::DEFAULT_PRIORITY, nullptr);
    DBG(1);
    miosix::Thread::yield();
    DBG(3);
    Pin::high();
    DBG(4);
    miosix::Thread::yield();
    DBG(6);
}