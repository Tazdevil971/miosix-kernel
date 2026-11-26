/*
 * Linker script for stm32f405rg
 * For compiling Miosix as a unikernel.
 * Allocates all RAM for the kernel and kernelspace application, no process pool
 */

MEMORY
{
    /* The beginning of the flash is occupied by the crazyflie bootloader */
    flash(rx)   : ORIGIN = 0x08004000, LENGTH = 1008K
    ram1(wx)    : ORIGIN = 0x10000000, LENGTH = 64K
    ram2(wx)    : ORIGIN = 0x20000000, LENGTH = 128K
}

INCLUDE ldscripts/miosix-flash-2ram-separate-heap.ld
