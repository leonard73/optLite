#include "algo.h"
/*
     @do_threshold_u8 is to set the max value if the pixel value is larger than threshold,the lower ones are all zero
*/
void do_bgr888_to_gray_optV1(uint8_t * src,uint8_t * dst,uint32_t pixelNB)
{
    for(uint32_t pixel=0;pixel<pixelNB*3;pixel+=3)
    {
         uint32_t weight_sum = src[pixel]*30 + src[pixel+1]*150 + src[pixel+2] *76;
         uint8_t  gray       = (uint8_t)(weight_sum >> 8);
         dst[pixel]          = gray;
    }
}
void do_bgr888_to_gray_optV2(uint8_t * src,uint8_t * dst,uint32_t pixelNB)
{
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
          uint32_t weight_sum  = (*src++)*30;
                   weight_sum += (*src++)*150;
                   weight_sum += (*src++)*76;
          uint8_t  gray       = (uint8_t)(weight_sum >> 8);
          *dst++              = gray;
    }
}
#ifdef _USE_AVX128_ISA_
#include <immintrin.h>
void do_bgr888_to_gray_optV3(uint8_t * src,uint8_t * dst,uint32_t pixelNB)  
{
    for(uint32_t pixel=0;pixel<pixelNB;pixel+=8)
    {
         for(uint32_t j=0;j<8;j++)
         {
               uint32_t  weight_sum  = src[pixel+j]*30;
                         weight_sum += src[pixel+j+1]*150;
                         weight_sum += src[pixel+j+2]*76;
               uint8_t  gray         = (uint8_t)(weight_sum >> 8);
               dst[pixel+j]                = gray;
               dst[pixel+j+1]              = gray;
               dst[pixel+j+2]              = gray;
         }
    }
}
#endif