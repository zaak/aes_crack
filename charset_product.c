//
// Created by zaak on 22.05.16.
//

#include "charset_product.h"
#include <string.h>


void product_init(sCharsetProduct *p, char *charset)
{
    memset(p, 0, sizeof(p));
    p->charset = charset;
    p->charsetLength = (int) strlen(charset);
}

void product_reset(sCharsetProduct *p)
{
    memset(p->indexes, 0, sizeof(p->indexes));
    p->indexes[0] = -1;
}

void product_set_subset_length(sCharsetProduct *p, unsigned int subsetLength)
{
    p->subsetLength = subsetLength;
    product_reset(p);
}

int bump_index(sCharsetProduct *p, int index)
{
    if (index >= p->subsetLength) {
        return 0;
    }

    if (p->indexes[index] < p->charsetLength-1) {
        ++p->indexes[index];
        return 1;
    }

    p->indexes[index] = 0;
    return bump_index(p, index+1);
}

int bump_indexes(sCharsetProduct *p)
{
    return bump_index(p, 0);
}

int product_next(sCharsetProduct *p, char *target)
{
    if (!bump_indexes(p)) {
        return 0;
    }

    for (int i = 0; i < p->subsetLength; ++i) {
        target[i] = p->charset[p->indexes[i]];
    }

    return 1;
}