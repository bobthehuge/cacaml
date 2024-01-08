#include "cml_utils.h"

size_t getlongdigits(int64_t value)
{
    int64_t l = !value;

    while(value)
    { 
        l++; 
        value/=10; 
    }

    return l;
}
