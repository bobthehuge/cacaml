#include <stdio.h>
#include <stdint.h>
#include "common.h"

void escprints(char *s)
{
    while (*s)
    {
        switch (*s++)
        {
        case 0: printf("\\0"); break;
        case 1: printf("\\SOH"); break;
        case 2: printf("\\STX"); break;
        case 3: printf("\\ETX"); break;
        case 4: printf("\\EOT"); break;
        case 5: printf("\\ENQ"); break;
        case 6: printf("\\ACK"); break;
        case 7: printf("\\a"); break;
        case 8: printf("\\b"); break;
        case 9: printf("\\t"); break;
        case 10: printf("\\n"); break;
        case 11: printf("\\v"); break;
        case 12: printf("\\f"); break;
        case 13: printf("\\r"); break;
        case 14: printf("\\SO"); break;
        case 15: printf("\\SI"); break;
        case 16: printf("\\DLE"); break;
        case 17: printf("\\DC1"); break;
        case 18: printf("\\DC2"); break;
        case 19: printf("\\DC3"); break;
        case 20: printf("\\DC4"); break;
        case 21: printf("\\NAK"); break;
        case 22: printf("\\SYN"); break;
        case 23: printf("\\ETB"); break;
        case 24: printf("\\CAN"); break;
        case 25: printf("\\EM"); break;
        case 26: printf("\\SUB"); break;
        case 27: printf("\\ESC"); break;
        case 28: printf("\\FS"); break;
        case 29: printf("\\GS"); break;
        case 30: printf("\\RS"); break;
        case 31: printf("\\US"); break;
        case 127: printf("\\DEL"); break;
        default: printf("%c", *(s-1));
        }
    }

    fflush(stdout);
}

void nputchar(char c, unsigned int n)
{
    for (uint32_t i = 0; i < n; i++)
        putchar(c);
}
