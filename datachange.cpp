#include "datachange.h"

DataChange::DataChange()
{

}
//执行转换：例如"123"->"31 32 33 "
int DataChange::hex_to_str(char *ptr /*after*/, int index /*before index*/, char *buf /*before*/, int len /*length*/)
{   //执行转换：例如"123"->"31 32 33 "
    int size=0;
    ptr+=index;
    for(int i = 0; i < len; i++)
    {
        if(i==len-1)
        {
            sprintf(ptr, "%02x ",buf[i]);
            ptr += 2;
            size+=2;
        }
        else if (i==0)
        {
             if(index!=0)
             {
                    sprintf(ptr, ",%02x ",buf[i]);
                    ptr += 4;
                    size+=4;
             }
             else
             {
                    sprintf(ptr, "%02x ",buf[i]);
                    ptr += 3;
                    size+=3;
             }
        }
        else
        {
           sprintf(ptr, "%02x ",buf[i]);
           ptr += 3;
           size+=3;
        }
    }
    return size;
}
//执行转换：例如"31 32 33 "->"123"
int DataChange::str_to_hex(char *ch /*before*/, char *cbuf /*after*/, int len /*length*/)
{   //执行转换：例如"31 32 33 "->"123"
    char high, low;
    int idx, ii=0;
    for (idx=0; idx<len; idx+=3)
    {
        high = ch[idx];
        low = ch[idx+1];

        if(high>='0' && high<='9')
            high = high-'0';
        else if(high>='A' && high<='F')
            high = high - 'A' + 10;
        else if(high>='a' && high<='f')
            high = high - 'a' + 10;
        else
            return -1;

        if(low>='0' && low<='9')
            low = low-'0';
        else if(low>='A' && low<='F')
            low = low - 'A' + 10;
        else if(low>='a' && low<='f')
            low = low - 'a' + 10;
        else
            return -1;

        cbuf[ii++] = high<<4 | low;
    }
    cbuf[ii]='\0';      //添加结束符'\0'
    return 0;
}
