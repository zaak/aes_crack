
#ifndef AES_CRACK_CHARSET_PRODUCT_H
#define AES_CRACK_CHARSET_PRODUCT_H


typedef struct _sCharsetProduct
{
    char *charset;
    unsigned int charsetLength;
    unsigned int subsetLength;
    int indexes[255];
} sCharsetProduct;

void product_init(sCharsetProduct *p, char *charset);
void product_reset(sCharsetProduct *p);
void product_set_subset_length(sCharsetProduct *p, unsigned int subsetLength);
int product_next(sCharsetProduct *p, char *target);

#endif //AES_CRACK_CHARSET_PRODUCT_H
