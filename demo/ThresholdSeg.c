#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include "../codes/include/include.h"
#define MAX_INPUT_SIZE (1024*1024*128)
ALGIN(16) uint8_t input[MAX_INPUT_SIZE];
ALGIN(16) uint8_t output[MAX_INPUT_SIZE];
void log_histogram_u8(uint8_t * hist);
int main()
{
    bitmap_file_header bmpFileHeader;
    bitmap_info_header bmpInfoHeader;
    readBMP((char*)_PIC1_BMP_PATH_3072x2048x3_,&bmpFileHeader,&bmpInfoHeader,input);
    uint32_t img_width  = bmpInfoHeader.bmpInfo_width;
    uint32_t img_height = bmpInfoHeader.bmpInfo_height;
    uint32_t img_ch     = bmpInfoHeader.bmpInfo_bitcount >> 3;
    //step1 bgr888 to gray8
    {
        record_ts_start();
        do_bgr888_to_gray(input,output,img_width*img_height);
        record_ts_end();
        printf("do_bgr888_to_gray runs:  %ld US\n",getLatencyUs());
        save_RawGray8_bmpFile((char*)_PIC1_RESULT_BMP_PATH_BGR2GRAY,output,img_width*img_height,img_width,img_height);
    } 
    //step2 gray8 histogram
    uint8_t histogram_tbl[256];
    {
        record_ts_start();
        uint8_t * grayU8 = output;
        do_gray8_histogram_optV3_quadCoreParallel(grayU8,histogram_tbl,img_width*img_height);
        // do_gray8_histogram_optV3(grayU8,histogram_tbl,img_width*img_height);
        record_ts_end();
        printf("do_gray8_histogram runs: %ld US\n",getLatencyUs());
        // log_histogram_u8(histogram_tbl);
    }
    uint8_t threshold;
    //step3 find best threshold
    {
        record_ts_start();
        threshold=do_bestThresholdByHistIteration_u8(histogram_tbl,100);
        record_ts_end();
        printf("do_bestThresholdByHistIteration_u8 runs: %ld US\n",getLatencyUs());
        // printf("best threshold=%d\n",threshold);
    }
    //step4 do threshold operation on img
    {
        record_ts_start();
        uint8_t * grayU8_opIn  = output;
        uint8_t * grayU8_opOut = output+img_width*img_height;
        do_threshold_u8_optV4(grayU8_opIn,grayU8_opOut,img_width*img_height,threshold);
        record_ts_end();
        printf("do_threshold_u8 runs: %ld US\n",getLatencyUs());
        save_RawGray8_bmpFile((char*)_PIC1_RESULT_BMP_PATH_THRESHOLD,grayU8_opOut,img_width*img_height,img_width,img_height);
    }
}