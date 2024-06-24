#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"

void _printString(char const *string)
{
    while (*string != '\0')
    {
        __putc(*string);
        string++;
    }
}

void _printInteger(uint64 integer, int base)
{
    static char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (integer < 0)
    {
        neg = 1;
        x = -integer;
    } else
    {
        x = integer;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % base];
    } while ((x /= base) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0) { __putc(buf[i]); }
}