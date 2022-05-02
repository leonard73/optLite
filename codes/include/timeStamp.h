#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__
void record_ts_start();
void record_ts_end();
int64_t getLatencyUs();
#endif //__TIMESTAMP_H__