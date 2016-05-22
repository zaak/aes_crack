#include <stdio.h>
#include <memory.h>
#include "aes.h"

#define ALIGN16 __attribute__ ((aligned (16)))

int main() {
    UCHAR ALIGN16 base_iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    UCHAR ALIGN16 iv[16] = {0};
    UCHAR ALIGN16 key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    UCHAR ALIGN16 expanded_key[256] = {0};

    iEncExpandKey128(key, expanded_key);

    for (int i = 0; i < 256; i++) {
        printf("%02X ", expanded_key[i]);
        if (i && (i + 1) % 16 == 0) {
            puts("");
        }
    }

    sAesData aesData;

    UCHAR plain[17] = "ala ma kota12345";

    __asm__ (
        "movdqa %1, %%xmm0;" /* xmm0 <- iv */
        "movdqa %%xmm0, %0;" /* xmm0 -> iv */
        : "=m"(*iv)          /* output operands, %0 */
        : "m"(*base_iv)      /* input operands, %1 */
        : "%xmm0"            /* clobbered registers */
    );

    for (int i = 0; i < 16; i++) {
        printf("%02X ", iv[i]);
        if (i && (i + 1) % 16 == 0) {
            puts("<- iv");
        }
    }

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

    UCHAR decipherPlain[17] = {0};

    iDecExpandKey128(key, expanded_key);
    memcpy(iv, base_iv, 16);

    sAesData decryptAesData;
    decryptAesData.expanded_key = expanded_key;
    decryptAesData.in_block = cipher;
    decryptAesData.out_block = decipherPlain;
    decryptAesData.iv = iv;
    decryptAesData.num_blocks = 1;

    iDec128_CBC(&decryptAesData);

    for (int i = 0; i < 16; i++) {
        printf("%02X ", decipherPlain[i]);
        if (i && (i + 1) % 16 == 0) {
            puts("<- deciphered text");
        }
    }
}

