#include "algo.h"

void do_smooth_mean_u8_bgr888(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius)
{
    for(int32_t h=0;h<height;h++ )
    {
        for(int32_t w=0;w<width;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                uint32_t effect_nb=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        {
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch];
                            effect_nb++;
                        }
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/effect_nb);
            }
        }
    }
}
void do_smooth_gaussian_u8_bgr888(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel)
{
    const uint32_t gaussianK_len = kernel_Radius*2+1;
    const uint32_t gaussianK_nb  = gaussianK_len*gaussianK_len;
    for(int32_t h=0;h<height;h++ )
    {
        for(int32_t w=0;w<width;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                uint32_t kernel_sum=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        {
                            uint32_t gaussianKernelID = (p_h-h+kernel_Radius) * gaussianK_len + (p_w-w+kernel_Radius);
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * gaussKernel[gaussianKernelID];
                            kernel_sum+=gaussKernel[gaussianKernelID];
                        }
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum);
            }
        }
    }
}