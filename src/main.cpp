#include "../lib/console.h"

extern "C" void trap();

void interrupt_handler()
{

}


int main()
{
    asm volatile("csrw stvec, %0" ::"r"(trap));

    int a = 0;
    a++;
    __putc('v');
    __putc('\n');
    return a;
}