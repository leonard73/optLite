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
    //MEAN SMOOTH  
    {
      record_ts_start();
      do_smooth_mean_u8_bgr888(input,output,(int32_t)img_width,(int32_t)img_height,2);
      record_ts_end();
      save_RawRGB_bmpFile((char*)_PIC1_RESULT_BMP_PATH_MEAN_SMOOTH_,output,img_width*img_height*3,img_width,img_height);
      printf("do_smooth_mean_u8_bgr888     runs: %ld US\n",getLatencyUs());
    }
    //GAUSSIAN SMOOTH
    {
      record_ts_start();
      uint32_t gaussianKernel[]={1,2,1,2,4,2,1,2,1};
      do_smooth_gaussian_u8_bgr888_V3(input,output,(int32_t)img_width,(int32_t)img_height,1,gaussianKernel);
      record_ts_end();
      save_RawRGB_bmpFile((char*)_PIC1_RESULT_BMP_PATH_GAUSSIAN_SMOOTH_,output,img_width*img_height*3,img_width,img_height);
      printf("do_smooth_gaussian_u8_bgr888 runs: %ld US\n",getLatencyUs());
    }
    printf("hello world demo_main\n");
}