#ifndef DATACHANGE_H
#define DATACHANGE_H

#include <stdio.h>

class DataChange
{
public:
    DataChange();
    //执行转换：例如"123"->"31 32 33 "
    int hex_to_str(char *ptr /*after*/, int index /*before index*/, char *buf /*before*/, int len /*length*/);
    //执行转换：例如"31 32 33 "->"123"
    int str_to_hex(char *ch /*before*/, char *cbuf /*after*/, int len /*length*/);
};

#endif // DATACHANGE_H
