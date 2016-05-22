#include <stdio.h>
#include <memory.h>
#include "aes.h"

int main() {

    UCHAR iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
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

    UCHAR plain[17] = "ala ma kota12345";
    UCHAR local_iv[16] = {0};
    memcpy(local_iv, iv, 16);
    UCHAR cipher[16] = {0};

    aesData.expanded_key = expanded_key;
    aesData.num_blocks = 1;
    aesData.iv = local_iv;
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
    memcpy(local_iv, iv, 16);

    sAesData decryptAesData;
    decryptAesData.expanded_key = expanded_key;
    decryptAesData.in_block = cipher;
    decryptAesData.out_block = decipherPlain;
    decryptAesData.iv = local_iv;
    decryptAesData.num_blocks = 1;

    iDec128_CBC(&decryptAesData);

    for (int i = 0; i < 16; i++) {
        printf("%02X ", decipherPlain[i]);
        if (i && (i + 1) % 16 == 0) {
            puts("<- deciphered text");
        }
    }
}

