#include "algo.h"
/*
     @do_gray8_histogram is to compute histogram of 256 level image
*/
void do_gray8_histogram(uint8_t const * src,uint8_t * histogramTbl,uint32_t pixelNB)
{
    // init histogram tbl
    memset(histogramTbl,0x00,256*sizeof(uint8_t));
    // accumulate pixel values
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
        uint8_t pixel_value = src[pixel];
        histogramTbl[pixel_value]=histogramTbl[pixel_value]+1;
    }
}
uint8_t do_bestThresholdByHistIteration_u8(uint8_t const * histogramTbl,uint32_t  iteration_times)
{
    uint8_t curThreshold=0;
    uint8_t newThreshold=0;
    //step1 find max and min gray level from hist
    uint8_t max_level=0,min_level=255;
    for(uint32_t i=0;i<256;i++)
    {
        max_level = (histogramTbl[i] > max_level) ? histogramTbl[i] : max_level;
        min_level = (histogramTbl[i] < min_level) ? histogramTbl[i] : min_level;
    }
    newThreshold = (max_level + min_level)>>1;
    //step2
    for(uint32_t iterator=0;(iterator < iteration_times) && (curThreshold!=newThreshold);iterator++)
    {
        curThreshold = newThreshold;
        uint32_t lowerTH_pixelNB   = 0, largerTH_pixelNB    = 0;
        uint32_t lowerTH_levelWSum = 0, largerTH_levelWSum  = 0;
        for(uint32_t j=0;j<curThreshold;j++)
        {
            lowerTH_pixelNB   += histogramTbl[j];
            lowerTH_levelWSum += histogramTbl[j] * j;
        }
        for(uint32_t j=curThreshold;j<256;j++)
        {
            largerTH_pixelNB   += histogramTbl[j];
            largerTH_levelWSum += histogramTbl[j] * j;
        }
        newThreshold = (lowerTH_levelWSum/lowerTH_pixelNB + largerTH_levelWSum/largerTH_pixelNB)>>1;
    }
    return newThreshold;
}
void do_threshold_u8(uint8_t const * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
        uint8_t pixel_value = src[pixel];
        if(src[pixel] < threshold)
        {
            dst[pixel] = 0;
        }
        else
        {
            dst[pixel] = 255;
        }
    }
}