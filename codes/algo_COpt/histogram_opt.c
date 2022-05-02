#include "algo.h"
#include "omp.h"
/*
     @do_gray8_histogram is to compute histogram of 256 level image
*/
void do_gray8_histogram_optV1(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB)
{
    // init histogram tbl
    memset(histogramTbl,0x00,256*sizeof(uint8_t));
    // accumulate pixel values
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
        histogramTbl[*src++]++;
    }
}
void do_gray8_histogram_optV2(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB)
{
    // init histogram tbl
    memset(histogramTbl,0x00,256*sizeof(uint8_t));
    // accumulate pixel values
    for(uint32_t pixel=0;pixel<pixelNB;pixel+=4)
    {
        histogramTbl[*src++]++;
        histogramTbl[*src++]++;
        histogramTbl[*src++]++;
        histogramTbl[*src++]++;
    }
}
void do_gray8_histogram_optV3(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB)
{
    uint8_t * hist1 = (uint8_t*)calloc(256,sizeof(uint8_t));
    uint8_t * hist2 = (uint8_t*)calloc(256,sizeof(uint8_t));
    uint8_t * hist3 = (uint8_t*)calloc(256,sizeof(uint8_t));
    uint8_t * hist4 = (uint8_t*)calloc(256,sizeof(uint8_t));
    uint32_t  quad_pixelNB = pixelNB>>2;
    register uint8_t *src1 = src;
    register uint8_t *src2 = src1+quad_pixelNB;
    register uint8_t *src3 = src2+quad_pixelNB;
    register uint8_t *src4 = src3+quad_pixelNB;
    do
    {
        hist1[*src1++]++;
        hist2[*src2++]++;
        hist3[*src3++]++;
        hist4[*src4++]++;
        quad_pixelNB--;
    } while (quad_pixelNB);
    for(uint32_t i=0;i<256;i++)
    {
        *histogramTbl++ = hist1[i] + hist2[i] + hist3[i] + hist4[i];
    }
    free(hist1);free(hist2);free(hist3);free(hist4);
}
void do_gray8_histogram_optV4(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB)
{
    uint8_t * hist1 = (uint8_t*)calloc(256,sizeof(uint8_t));
    uint8_t * hist2 = (uint8_t*)calloc(256,sizeof(uint8_t));
    uint8_t * hist3 = (uint8_t*)calloc(256,sizeof(uint8_t));
    uint8_t * hist4 = (uint8_t*)calloc(256,sizeof(uint8_t));
    register uint8_t *src1 = src;
    register uint8_t *src2 = src1+(pixelNB>>2);
    register uint8_t *src3 = src2+(pixelNB>>2);
    register uint8_t *src4 = src3+(pixelNB>>2);
    #define DO_HIST_ACC(HIST_X,SRC_X)   for(int pixel=0;pixel<pixelNB;pixel+=4)\
                                        {                                      \
                                            HIST_X[*SRC_X++]++;                \
                                        }
    omp_set_num_threads(4);
    #pragma omp parallel num_threads(4)
    {
        int ID=omp_get_thread_num();
        switch(ID)
        {
            case 0: DO_HIST_ACC(hist1,src1);break;
            case 1: DO_HIST_ACC(hist2,src2);break;
            case 2: DO_HIST_ACC(hist3,src3);break;
            case 3: DO_HIST_ACC(hist4,src4);break;
            default:break;
        }
    }
    for(uint32_t i=0;i<256;i++)
    {
        *histogramTbl++ = hist1[i] + hist2[i] + hist3[i] + hist4[i];
    }
    free(hist1);free(hist2);free(hist3);free(hist4);
}
void do_threshold_u8_optV1(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
        *dst++  = ((*src++) < threshold )? 0 : 255;
    }
}
void do_threshold_u8_optV2(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    for(uint32_t pixel=0;pixel<(pixelNB>>2);pixel++)
    {
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
    }
}
#ifdef _USE_NEON_ISA_
void do_threshold_u8_optV3(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    uint8x16_t vThreshold = {   threshold,threshold,threshold,threshold,threshold,threshold,threshold,threshold,
                                threshold,threshold,threshold,threshold,threshold,threshold,threshold,threshold };
    for(uint32_t pixel=0;pixel<pixelNB;pixel+=16)
    {
        uint8x16_t vLoad_src = vld1q_u8(src+pixel);
        uint8x16_t vStore_dst= vcgtq_u8(vLoad_src,vThreshold);
        vst1q_u8(dst+pixel,vStore_dst);
    }
}
void do_threshold_u8_optV4(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    uint8x16_t vThreshold = {   threshold,threshold,threshold,threshold,threshold,threshold,threshold,threshold,
                                threshold,threshold,threshold,threshold,threshold,threshold,threshold,threshold };
    for(uint32_t pixel=0;pixel<pixelNB;pixel+=32)
    {
        uint8x16_t vLoad_src1 = vld1q_u8(src+pixel);
        uint8x16_t vStore_dst1= vcgtq_u8(vLoad_src1,vThreshold);
        vst1q_u8(dst+pixel,vStore_dst1);
        uint8x16_t vLoad_src2 = vld1q_u8(src+pixel+16);
        uint8x16_t vStore_dst2= vcgtq_u8(vLoad_src2,vThreshold);
        vst1q_u8(dst+pixel+16,vStore_dst2);
    }
}
#endif
#ifdef _USE_AVX256_ISA_
void do_threshold_u8_optV3(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    for(uint32_t pixel=0;pixel<pixelNB;pixel++)
    {
        __m256i vLoad_src  = _mm256_load_si256(src+pixel);
        __m256i vStore_dst = 
         _mm256_store_si256 (dst+pixel, __m256i a)
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
    }
}
void do_threshold_u8_optV4(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    for(uint32_t pixel=0;pixel<(pixelNB>>2);pixel++)
    {
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
        *dst++  = ((*src++) < threshold )? 0 : 255;
    }
}
#endif