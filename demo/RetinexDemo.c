#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include "../codes/include/include.h"
#define MAX_INPUT_SIZE (1024*1024*128)
ALGIN(16) uint8_t input[MAX_INPUT_SIZE];
ALGIN(16) uint8_t output[MAX_INPUT_SIZE];
int main()
{
    bitmap_file_header bmpFileHeader;
    bitmap_info_header bmpInfoHeader;
    readBMP((char*)_PIC1_BMP_PATH_3072x2048x3_,&bmpFileHeader,&bmpInfoHeader,input);
    uint32_t img_width  = bmpInfoHeader.bmpInfo_width;
    uint32_t img_height = bmpInfoHeader.bmpInfo_height;
    uint32_t img_ch     = bmpInfoHeader.bmpInfo_bitcount >> 3;
    //step1 bgr888 to gray
    {
        record_ts_start();
        do_bgr888_to_gray_optV2(input,output,img_width*img_height);
        record_ts_end();
        printf("do_bgr888_to_gray runs: %ld US\n",getLatencyUs());
        save_RawGray8_bmpFile((char*)_PIC1_RESULT_BMP_PATH_BGR2GRAY,output,img_width*img_height,img_width,img_height);
    } 
    //step2 gray img 
    printf("hello world demo_main\n");
}