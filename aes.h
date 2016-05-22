#ifndef AES_CRACK_AES_H
#define AES_CRACK_AES_H

typedef unsigned char UCHAR;

typedef struct _sAesData
{
    UCHAR	*in_block;
    UCHAR	*out_block;
    UCHAR	*expanded_key;
    UCHAR	*iv;					// for CBC mode
    size_t	num_blocks;
} sAesData;

void iEncExpandKey128(UCHAR *key, UCHAR *expanded_key);
void iEnc128_CBC(sAesData *data);

void iDecExpandKey128(UCHAR *key, UCHAR *expanded_key);
void iDec128_CBC(sAesData *data);

#endif //AES_CRACK_AES_H