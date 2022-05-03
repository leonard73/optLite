#include "algo.h"
void do_smooth_gaussian_u8_bgr888_V1(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel)
{
    const uint32_t gaussianK_len = kernel_Radius*2+1;
    const uint32_t gaussianK_nb  = gaussianK_len*gaussianK_len;
    uint32_t local_kernel_array[gaussianK_nb];
    memcpy(local_kernel_array,gaussKernel,gaussianK_nb*sizeof(uint32_t));
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
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * local_kernel_array[gaussianKernelID];
                            kernel_sum+=local_kernel_array[gaussianKernelID];
                        }
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum);
            }
        }
    }
}
void do_smooth_gaussian_u8_bgr888_V2(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel)
{
    const uint32_t gaussianK_len = kernel_Radius*2+1;
    const uint32_t gaussianK_nb  = gaussianK_len*gaussianK_len;
    uint32_t local_kernel_array[gaussianK_nb];
    memcpy(local_kernel_array,gaussKernel,gaussianK_nb*sizeof(uint32_t));
    for(int32_t h=0;h<height;h++ )
    {
        for(int32_t w=0;w<width;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                uint32_t kernel_sum=0;
                uint32_t k_id=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        {
                            // uint32_t gaussianKernelID = (p_h-h+kernel_Radius) * gaussianK_len + (p_w-w+kernel_Radius);
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * local_kernel_array[k_id];
                            kernel_sum+=local_kernel_array[k_id];
                        }
                        k_id++;
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum);
            }
        }
    }
}
void do_smooth_gaussian_u8_bgr888_V3(uint8_t* bgr888_src,uint8_t *bgr888_dst,int32_t width,int32_t height,int32_t kernel_Radius,uint32_t *gaussKernel)
{
    const uint32_t gaussianK_len = kernel_Radius*2+1;
    const uint32_t gaussianK_nb  = gaussianK_len*gaussianK_len;
    uint32_t local_kernel_array[gaussianK_nb];
    memcpy(local_kernel_array,gaussKernel,gaussianK_nb*sizeof(uint32_t));
    uint32_t kernel_sum_main_area = 0;
    for(uint32_t k=0;k<gaussianK_nb;k++) {kernel_sum_main_area+=local_kernel_array[k];}
    /***********************************************************************/
    //main area
    for(int32_t h=gaussianK_len;h<height-gaussianK_len;h++ )
    {
        for(int32_t w=gaussianK_len;w<width-gaussianK_len;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                // uint32_t kernel_sum=0;
                uint32_t k_id=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        // if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        // {
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * local_kernel_array[k_id++];
                            // kernel_sum+=local_kernel_array[k_id];
                        // }
                        // k_id++;
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum_main_area);
            }
        }
    }
    /***********************************************************************/
    //border up
    for(int32_t h=0;h<gaussianK_len;h++ )
    {
        for(int32_t w=0;w<width;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                uint32_t kernel_sum=0;
                uint32_t k_id=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        {
                            // uint32_t gaussianKernelID = (p_h-h+kernel_Radius) * gaussianK_len + (p_w-w+kernel_Radius);
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * local_kernel_array[k_id];
                            kernel_sum+=local_kernel_array[k_id];
                        }
                        k_id++;
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum);
            }
        }
    }
    //border down
    for(int32_t h=height-gaussianK_len;h<height;h++)
    {
        for(int32_t w=0;w<width;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                uint32_t kernel_sum=0;
                uint32_t k_id=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        {
                            // uint32_t gaussianKernelID = (p_h-h+kernel_Radius) * gaussianK_len + (p_w-w+kernel_Radius);
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * local_kernel_array[k_id];
                            kernel_sum+=local_kernel_array[k_id];
                        }
                        k_id++;
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum);
            }
        }
    }
    //border left
    for(int32_t h=0;h<height;h++)
    {
        for(int32_t w=0;w<gaussianK_len;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                uint32_t kernel_sum=0;
                uint32_t k_id=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        {
                            // uint32_t gaussianKernelID = (p_h-h+kernel_Radius) * gaussianK_len + (p_w-w+kernel_Radius);
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * local_kernel_array[k_id];
                            kernel_sum+=local_kernel_array[k_id];
                        }
                        k_id++;
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum);
            }
        }
    }
    //border right
    for(int32_t h=0;h<height;h++)
    {
        for(int32_t w=width-gaussianK_len;w<width;w++)
        {
            uint32_t pixel_id = h*width*3+w*3;
            for(int32_t ch=0;ch<3;ch++)
            {
                uint32_t sum_v=0;
                uint32_t kernel_sum=0;
                uint32_t k_id=0;
                /************************************************************/
                for(int32_t p_h=h-kernel_Radius;p_h<=h+kernel_Radius;p_h++)
                {
                    for(int32_t p_w=w-kernel_Radius;p_w<=w+kernel_Radius;p_w++)
                    {
                        if((p_h>=0 && p_h<height) &&(p_w>=0 && p_w<width))
                        {
                            // uint32_t gaussianKernelID = (p_h-h+kernel_Radius) * gaussianK_len + (p_w-w+kernel_Radius);
                            sum_v += bgr888_src[p_h*width*3+p_w*3+ch] * local_kernel_array[k_id];
                            kernel_sum+=local_kernel_array[k_id];
                        }
                        k_id++;
                    }
                }
                /************************************************************/
                bgr888_dst[pixel_id+ch] = (uint8_t)(sum_v/kernel_sum);
            }
        }
    }
}