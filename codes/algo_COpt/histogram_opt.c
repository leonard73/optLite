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
        // vLoad_src ={src[p+0]|src[p+1]|src[p+2]|src[p+3]|src[p+4]|src[p+5]|src[p+6]|src[p+7]|src[p+8]|src[p+9]|src[p+10]|src[p+11]|src[p+12]|src[p+13]|src[p+14]|src[p+15]}
        uint8x16_t vStore_dst= vcgtq_u8(vLoad_src,vThreshold);
        vst1q_u8(dst+pixel,vStore_dst);
    }
}
void do_threshold_u8_optV4(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    uint8x16_t vThreshold = {   threshold,threshold,threshold,threshold,threshold,threshold,threshold,threshold,
                                threshold,threshold,threshold,threshold,threshold,threshold,threshold,threshold };
    const uint32_t half_pixelNB = pixelNB >> 1;
    register uint8x16_t * vsrc1 = (uint8x16_t *)src;
    register uint8x16_t * vsrc2 = (uint8x16_t *)(src + half_pixelNB);
    register uint8x16_t * vdst1 = (uint8x16_t *)dst;
    register uint8x16_t * vdst2 = (uint8x16_t *)(dst + half_pixelNB);
    for(uint32_t pixel=0;pixel<half_pixelNB;pixel+=16)
    {
        uint8x16_t vLoad_src1 = vld1q_u8((void*)(vsrc1++));
        uint8x16_t vLoad_src2 = vld1q_u8((void*)(vsrc2++));
        uint8x16_t vStore_dst1= vcgtq_u8(vLoad_src1,vThreshold);
        uint8x16_t vStore_dst2= vcgtq_u8(vLoad_src2,vThreshold);
        vst1q_u8((void*)(vdst1++),vStore_dst1);
        vst1q_u8((void*)(vdst2++),vStore_dst2);
    }
}
#endif
#ifdef _USE_AVX256_ISA_
void do_threshold_u8_optV3(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    __m256i vSFlag      =  _mm256_set1_epi8 (0x80);
    __m256i vThreshold  =  _mm256_set1_epi8 (((char)(threshold)) ^ 0x80);
    for(uint32_t pixel=0;pixel<pixelNB;pixel+=32)
    {
        __m256i vLoad_src   =  _mm256_loadu_si256 (src+pixel);
        __m256i vStore_dst  =  _mm256_cmpgt_epi8  (vLoad_src^vSFlag , vThreshold);
        _mm256_storeu_si256 (dst+pixel, vStore_dst);
    }
}
void do_threshold_u8_optV4(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold)
{
    __m256i vSFlag      =  _mm256_set1_epi8 (0x80);
    __m256i vThreshold  =  _mm256_set1_epi8 (((char)(threshold)) ^ 0x80);
    const uint32_t half_pixelNB = pixelNB >> 1;
    register __m256i * vsrc1 = (__m256i *)src;
    register __m256i * vsrc2 = (__m256i *)(src + half_pixelNB);
    register __m256i * vdst1 = (__m256i *)dst;
    register __m256i * vdst2 = (__m256i *)(dst + half_pixelNB);
    for(uint32_t pixel=0;pixel<half_pixelNB;pixel+=32)
    {
        __m256i vLoad_src1   =  _mm256_loadu_si256 ((void*)(vsrc1++));
        __m256i vLoad_src2   =  _mm256_loadu_si256 ((void*)(vsrc2++));
        __m256i vStore_dst1  =  _mm256_cmpgt_epi8  (vLoad_src1^vSFlag , vThreshold);
        __m256i vStore_dst2  =  _mm256_cmpgt_epi8  (vLoad_src2^vSFlag , vThreshold);
        _mm256_storeu_si256 ((void*)(vdst1++), vStore_dst1);
        _mm256_storeu_si256 ((void*)(vdst2++), vStore_dst2);
    }
}
#endif