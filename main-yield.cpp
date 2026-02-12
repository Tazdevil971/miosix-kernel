
#include <cstdio>
#include "miosix.h"

using namespace std;
using namespace miosix;

using Mco2 = miosix::Gpio<GPIOC_BASE, 9>;
using Pin = miosix::Gpio<GPIOB_BASE, 0>;

#define DBG(num) do {} while(0)
// #define DBG(num) printf("%d\n", num)

void other(void*) {
    DBG(2);
    miosix::Thread::yield();
    DBG(5);
    Pin::low();
}

int main() {
    Pin::mode(miosix::Mode::OUTPUT);
    Pin::speed(miosix::Speed::_100MHz);

    miosix::Thread::create(other, 16 * 1024, miosix::MAIN_PRIORITY, nullptr);
    DBG(1);
    miosix::Thread::yield();
    DBG(3);
    Pin::high();
    DBG(4);
    miosix::Thread::yield();
    DBG(6);
}
