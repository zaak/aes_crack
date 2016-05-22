#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "aes.h"
#include "charset_product.h"
#include "print_beautifiers.h"
#include "charset.h"

#define ALIGN16 __attribute__ ((aligned (16)))
#define BLOCK_LENGTH 16

int condition(UCHAR *s)
{
    return NULL != strstr((const char*)s, " kota");
}

int main() {
    char charset[] = CHARSET_ALNUM;

    // ala ma kota / foobar
    UCHAR ALIGN16 ciphertext[17] = "\x2e\xe1\x67\x5b\xcc\xc4\xc8\x46\x19\xb1\x54\x82\x83\xb8\xc9\x31";
    UCHAR ALIGN16 iv[17] =         "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

    UCHAR ALIGN16 key[16] = {0};
    UCHAR ALIGN16 local_iv[16] = {0};
    UCHAR ALIGN16 expanded_key[256] = {0};

    sCharsetProduct p;
    product_init(&p, charset);

    UCHAR deciphered[17] = {0};

    sAesData decryptAesData;
    decryptAesData.expanded_key = expanded_key;
    decryptAesData.in_block = ciphertext;
    decryptAesData.out_block = deciphered;
    decryptAesData.iv = local_iv;
    decryptAesData.num_blocks = 1;


    for(int n = 1; n < 17; ++n) {
        printf(FORMAT_INFO("Increasing key length to: %d"), n);
        product_set_subset_length(&p, n);
        while (product_next(&p, (char*)key)) {
            iDecExpandKey128(key, expanded_key);

            __asm__ (
                "movdqa %1, %%xmm0;" /* xmm0 <- iv */
                "movdqa %%xmm0, %0;" /* xmm0 -> local_iv */
                : "=m"(*local_iv)    /* output operands, %0 */
                : "m"(*iv)           /* input operands, %1 */
                : "%xmm0"            /* clobbered registers */
            );

            iDec128_CBC(&decryptAesData);

            if (condition(deciphered)) {
                print_success("Found a candidate key!");
                print_bold("Key:");
                hex_dump(key, 16);
                print_bold("Deciphered block:");
                hex_dump(deciphered, 16);
//                exit(0);
            }
        }
    }
}