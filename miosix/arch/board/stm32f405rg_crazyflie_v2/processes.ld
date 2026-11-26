/*
 * Linker script for stm32f405rg
 * For compiling Miosix as a fluid kernel.
 * Separates RAM in two partitions, the first used by the kernel and
 * kernelspace applications, the second being the process pool for userspace
 * applications.
 */

_process_pool_size = 32K;

MEMORY
{
    /* The beginning of the flash is occupied by the crazyflie bootloader */
    flash(rx)   : ORIGIN = 0x08004000, LENGTH = 1008K
    ram1(wx)    : ORIGIN = 0x10000000, LENGTH = 64K
    ram2(wx)    : ORIGIN = 0x20000000, LENGTH = 128K
}

INCLUDE ldscripts/miosix-flash-2ram-separate-heap.ld
