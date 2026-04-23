#include <cstdio>
#include "miosix.h"

using namespace std;
using namespace miosix;

using Mco2 = miosix::Gpio<GPIOC_BASE, 9>;
using Pin = miosix::Gpio<GPIOB_BASE, 1>;

enum class InterruptTrigger
{
    RISING_EDGE,
    FALLING_EDGE,
    RISING_FALLING_EDGE
};

constexpr unsigned ConvertGPIO_BASEtoUnsigned(unsigned P)
{
    // clang-format off
    return  P == GPIOA_BASE? 0 :
            P == GPIOB_BASE? 1 :
            P == GPIOC_BASE? 2 :
            P == GPIOD_BASE? 3 :
            P == GPIOE_BASE? 4 :
            P == GPIOF_BASE? 5 :
            P == GPIOG_BASE? 6 :
            P == GPIOH_BASE? 7 :
            P == GPIOI_BASE? 8 :
#if (defined (STM32F427xx) || defined (STM32F437xx) || defined (STM32F429xx) || defined (STM32F439xx))
            P == GPIOJ_BASE? 9 :
            P == GPIOK_BASE? 10 :
#endif
            0;
    // clang-format on
}

constexpr unsigned GetEXTI_IRQn(unsigned N)
{
    // clang-format off

    return N==0? EXTI0_IRQn :
           N==1? EXTI1_IRQn :
           N==2? EXTI2_IRQn :
           N==3? EXTI3_IRQn :
           N==4? EXTI4_IRQn :
           (N>=5&&N<=9)? EXTI9_5_IRQn :
           EXTI15_10_IRQn;

    // clang-format on
}

constexpr unsigned GetEXTICR_register_value(unsigned P, unsigned N)
{
    return (ConvertGPIO_BASEtoUnsigned(P) << ((N % 4) * 4));
}

constexpr unsigned GetEXTICR_register_mask(unsigned P, unsigned N)
{
    return (0b1111 << ((N % 4) * 4));
}

void enableExternalInterrupt(unsigned int gpioPort, unsigned int gpioNum,
                             InterruptTrigger trigger, unsigned int priority)
{
    auto exticrRegValue = GetEXTICR_register_value(gpioPort, gpioNum);

    {
        miosix::GlobalIrqLock dLock;

        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
        SYSCFG->EXTICR[int(gpioNum / 4)] |= exticrRegValue;
    }

    EXTI->IMR |= 1 << gpioNum;

    if (trigger == InterruptTrigger::RISING_EDGE ||
        trigger == InterruptTrigger::RISING_FALLING_EDGE)
    {
        EXTI->RTSR |= 1 << gpioNum;
    }

    if (trigger == InterruptTrigger::FALLING_EDGE ||
        trigger == InterruptTrigger::RISING_FALLING_EDGE)
    {
        EXTI->FTSR |= 1 << gpioNum;
    }

    NVIC_EnableIRQ(static_cast<IRQn_Type>(GetEXTI_IRQn(gpioNum)));
    NVIC_SetPriority(static_cast<IRQn_Type>(GetEXTI_IRQn(gpioNum)), priority);
}

#define DBG(num) do {} while(0)
// #define DBG(num) printf("%d\n", num)

miosix::Thread* THREAD = nullptr;

void exti1(void*) {
    EXTI->PR = EXTI_PR_PR1;
    if (THREAD) {
        THREAD->IRQwakeup();
    }
}

void other(void*) {
    DBG(1);
    miosix::Thread::wait();
    DBG(3);
    Pin::low();
}

int main() {
    Mco2::mode(miosix::Mode::ALTERNATE);
    Mco2::alternateFunction(0);

    Pin::mode(miosix::Mode::OUTPUT);
    Pin::speed(miosix::Speed::_100MHz);
    Pin::low();
    
    {
        miosix::GlobalIrqLock dLock;
        IRQregisterIrq(dLock, EXTI1_IRQn, exti1);
    }
    
    enableExternalInterrupt(GPIOB_BASE, 1, InterruptTrigger::RISING_EDGE, 1);
    
    THREAD = miosix::Thread::create(other, 16 * 1024, miosix::DEFAULT_PRIORITY + 1, nullptr);
    DBG(2);
    Pin::high();
    DBG(4);
    
    while (1) {}
}