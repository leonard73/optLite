#include "algo.h"
/*
     @do_threshold_u8 is to set the max value if the pixel value is larger than threshold,the lower ones are all zero
*/
void do_bgr888_to_gray(uint8_t * src,uint8_t * dst,uint32_t pixelNB)
{
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
         uint32_t weight_sum = src[pixel*3]*30 + src[pixel*3+1]*150 + src[pixel*3+2] *76;
         uint8_t  gray       = weight_sum >> 8;
         dst[pixel*3+0]      = gray;
         dst[pixel*3+1]      = gray;
         dst[pixel*3+2]      = gray;
    }
}
