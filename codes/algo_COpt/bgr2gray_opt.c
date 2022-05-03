#include "algo.h"
/*
     @do_threshold_u8 is to set the max value if the pixel value is larger than threshold,the lower ones are all zero
*/
void do_bgr888_to_gray_optV1(uint8_t * src,uint8_t * dst,uint32_t pixelNB)
{
    uint8_t * srcB  = src,*srcG = src+1,*srcR = src+2;
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
         uint32_t weight_sum = (*srcB)*30 + (*srcG)*150 + (*srcR) *76;srcB+=3;srcG+=3;srcR+=3;
         uint8_t  gray       = (uint8_t)(weight_sum >> 8);
         *dst++              = gray;
    }
}
void do_bgr888_to_gray_optV2(uint8_t * src,uint8_t * dst,uint32_t pixelNB)
{
    const uint32_t half_pixelNB = pixelNB>>1;
    uint8_t * srcB1  = src,             *srcG1 = src+1,             *srcR1 = src+2;
    uint8_t * srcB2  = src+half_pixelNB,*srcG2 = src+half_pixelNB+1,*srcR2 = src+half_pixelNB+2;
    uint8_t * dst1   = dst,             *dst2 = dst+half_pixelNB;
    for(uint32_t pixel=0;pixel<half_pixelNB;pixel++)
    {
         uint32_t weight_sum1 = (*srcB1)*30 + (*srcG1)*150 + (*srcR1) *76;srcB1+=3; srcG1+=3; srcR1+=3;
         uint32_t weight_sum2 = (*srcB2)*30 + (*srcG2)*150 + (*srcR2) *76;srcB2+=3; srcG2+=3; srcR2+=3;
         *dst1++       = (uint8_t)(weight_sum1 >> 8);
         *dst2++       = (uint8_t)(weight_sum2 >> 8);
    }
}