#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
static struct timeval ts_start,ts_end;
void record_ts_start()
{
    gettimeofday(&ts_start,NULL);
}
void record_ts_end()
{
    gettimeofday(&ts_end,NULL);    
}
int64_t getLatencyUs()
{
    return ((ts_end.tv_sec-ts_start.tv_sec)*1000000+(ts_end.tv_usec-ts_start.tv_usec));
}