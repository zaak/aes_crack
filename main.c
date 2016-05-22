#include <stdio.h>
#include "aes.h"

int main() {
    UCHAR key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    UCHAR __attribute__ ((aligned (16))) expanded_key[256] = {0};

    iEncExpandKey128(key, expanded_key);

    for (int i = 0; i < 256; i++) {
        printf("%02X ", expanded_key[i]);
        if (i && (i + 1) % 16 == 0) {
            puts("");
        }
    }

    sAesData aesData;

    UCHAR plain[16] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    UCHAR iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    UCHAR cipher[16] = {0};

    aesData.expanded_key = expanded_key;
    aesData.num_blocks = 1;
    aesData.iv = iv;
    aesData.in_block = plain;
    aesData.out_block = cipher;

    iEnc128_CBC(&aesData);

    for (int i = 0; i < 16; i++) {
        printf("%02X ", cipher[i]);
        if (i && (i + 1) % 16 == 0) {
            puts("<- cipher text");
        }
    }
}

