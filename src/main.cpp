#include "../lib/console.h"


void interrupt_handler()
{

}


int main()
{
    int a = 0;
    a++;
    __putc('v');
    __putc('\n');
    return a;
}