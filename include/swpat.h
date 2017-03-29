#ifndef __SWPAT_H__
#define __SWPAT_H__

#include "swpat.h"

//TS流的包头信息
typedef struct __sw_TS_HEAD
{
	unsigned int sync_byte;//后面包中的数据是不会出现0x47的
	unsigned int transport_error_indicator;//传输错误标志位，一般传输错误的话就不会处理这个包了
	unsigned int payload_unit_start_indicator;//为1时，在前4个字节之后会有一个调整字节，它的数值决定了负载内容的具体开始位置。
	unsigned int transport_priority;//传输优先级位，1表示高优先级
	unsigned int pid;//有效负载数据的类型
	unsigned int transport_scrambling_control; //加密标志位,00表示未加密
	unsigned int adaptation_field_control; //调整字段控制,01仅含有效负载，10仅含调整字段，11含有调整字段和有效负载。
	unsigned int continuity_counte; //一个4bit的计数器，范围0-15
}sw_ts_head_t;

//PAT表结构体
typedef struct __sw_TS_PAT
{
	unsigned int  table_id;                   //固定为0x00 ，标志是该表是PAT表  
    unsigned int  section_syntax_indicator; //段语法标志位，固定为1  
    unsigned int  zero;                       //0  
    unsigned int  reserved_1;                 // 保留位  
    unsigned int  section_length;            //表示从下一个字段开始到CRC32(含)之间有用的字节数  
    unsigned int  transport_stream_id;      //该传输流的ID，区别于一个网络中其它多路复用的流  
    unsigned int  reserved_2;                // 保留位  
    unsigned int  version_number;            //范围0-31，表示PAT的版本号  
    unsigned int  current_next_indicator;   //发送的PAT是当前有效还是下一个PAT有效  
    unsigned int  section_number;            //分段的号码。PAT可能分为多段传输，第一段为00，以后每个分段加1，最多可能有256个分段  
    unsigned int  last_section_number;      //最后一个分段的号码  
   
    void     *program;  					   //指向sw_PAT_PROGRAM_t类型
    unsigned int  reserved_3;                 // 保留位  
    unsigned int  network_pid;                //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID  
    unsigned int  crc_32;                     //CRC32校验码 
}sw_ts_pat_t;


#endif
