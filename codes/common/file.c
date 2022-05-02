#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "file.h"
void read_data_from_file(const char * path,uint32_t sizePerElem,uint32_t elem_nb,void* data_load_p)
{
    FILE * fp = fopen(path,"rb");
    if(fp==NULL)
    {
        printf("fail to open %s \n",path);
        return;
    }
    fread(data_load_p,sizePerElem,elem_nb,fp);
    fclose(fp);
}
void save_data_to_file(const char * path,uint32_t sizePerElem,uint32_t elem_nb,void* data_save_p)
{
    FILE * fp = fopen(path,"wb");
    if(fp==NULL)
    {
        printf("fail to open %s \n",path);
        return;
    }
    fwrite(data_save_p,sizePerElem,elem_nb,fp);
    fclose(fp);
}