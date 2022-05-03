#ifndef __ALGO_H__
#define __ALGO_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef _PLATFORM_ARM64_
#include <arm_neon.h>
#endif
#ifdef _PLATFORM_AMD64_
#include <immintrin.h>
#endif
void do_bgr888_to_gray(uint8_t * src,uint8_t * dst,uint32_t pixelNB);
void do_bgr888_to_gray_optV1(uint8_t * src,uint8_t * dst,uint32_t pixelNB);
void do_bgr888_to_gray_optV2(uint8_t * src,uint8_t * dst,uint32_t pixelNB);
void do_gray8_histogram(uint8_t const * src,uint8_t * histogramTbl,uint32_t pixelNB);
void do_gray8_histogram_optV1(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB);
void do_gray8_histogram_optV2(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB);
void do_gray8_histogram_optV3(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB);
void do_gray8_histogram_optV4(uint8_t * src,uint8_t * histogramTbl,uint32_t pixelNB);
uint8_t do_bestThresholdByHistIteration_u8(uint8_t const * histogramTbl,uint32_t  iteration_times);
void do_threshold_u8(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold);
void do_threshold_u8_optV1(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold);
void do_threshold_u8_optV2(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold);
void do_threshold_u8_optV3(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold);
void do_threshold_u8_optV4(uint8_t * src,uint8_t * dst,uint32_t pixelNB,uint8_t threshold);
void do_smooth_mean_u8_bgr888(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius);
void do_smooth_gaussian_u8_bgr888(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel);
void do_smooth_gaussian_u8_bgr888_V1(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel);
void do_smooth_gaussian_u8_bgr888_V2(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel);
void do_smooth_gaussian_u8_bgr888_V3(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel);
#endif //__ALGO_H__