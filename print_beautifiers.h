#ifndef AES_CRACK_PRINTF_BEAUTIFIERS_H
#define AES_CRACK_PRINTF_BEAUTIFIERS_H

#include <stdio.h>
#include "aes.h"

#define FORMAT_INFO(F) "\033[1;34m"F"\033[0m\n"

void print_info(char *s)
{
    printf(FORMAT_INFO("%s"), s);
}

void print_bold(char *s)
{
    printf("\033[1;0m%s\033[0m\n", s);
}

void print_success(char *s)
{
    printf("\033[0;32m%s\033[0m\n", s);
}


void hex_dump(unsigned char *s, int length)
{
    for (int i = 0; i < length; i++) {
        printf("%02x ", s[i]);
        if (i && (i + 1) % 16 == 0) {
            puts((const char*)s);
        }
    }
}

#endif //AES_CRACK_PRINTF_BEAUTIFIERS_H
