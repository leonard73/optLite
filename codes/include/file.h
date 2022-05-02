#ifndef __FILE_H__
#define __FILE_H__
#include <stdint.h>
void read_data_from_file(const char * path,uint32_t sizePerElem,uint32_t elem_nb,void* data_load_p);
void save_data_to_file(const char * path,uint32_t sizePerElem,uint32_t elem_nb,void* data_save_p);
#endif //__FILE_H__