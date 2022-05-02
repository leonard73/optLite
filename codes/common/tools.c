#include "tools.h"
void log_histogram_u8(uint8_t * hist)
{
    for(int i=0;i<16;i++)
    {
        for(int j=0;j<16;j++)
        {
            printf(" %3d[%4d] ",i*16+j,hist[i*16+j]);
        }
        printf("\n");
    }
}