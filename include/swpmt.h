#ifndef __SWPMT_H__
#define __SWPMT_H__

//PMT 表结构体
typedef struct __sw_TS_PMT
{
    unsigned table_id                       ; //固定为0x02, 表示PMT表，8位
    unsigned section_syntax_indicator    ; //固定为0x01   1位
    unsigned zero							 ; //0x01   1位      
    unsigned reserved_1                     ; //0x03   2位
    unsigned section_length                 ;//首先两位bit置为00，它指示段的byte数，由段长度域开始，包含CRC。12位
    unsigned program_number                ;// 指出该节目对应于可应用的Program map PID   16位
    unsigned reserved_2                     ; //0x03  2位
    unsigned version_number                 ; //指出TS流中Program map section的版本号 5位
    unsigned current_next_indicator       ; //当该位置1时，当前传送的Program map section可用；1位
      //当该位置0时，指示当前传送的Program map section不可用，下一个TS流的Program map section有效。
    unsigned section_number                ; //固定为0x00  8位
    unsigned last_section_number           ; //固定为0x00  8位
    unsigned reserved_3                     ; //0x07     3位
    unsigned pcr_pid                       ; //指明TS包的PID值，该TS包含有PCR域， 13位
            //该PCR值对应于由节目号指定的对应节目。
            //如果对于私有数据流的节目定义与PCR无关，这个域的值将为0x1FFF。
    unsigned reserved_4                     ; //预留为0x0F  4位
    unsigned program_info_length           ; //前两位bit为00。该域指出跟随其后对节目信息的描述的byte数。12位
    
    unsigned reserved_5                     ; //0x07   3位
    unsigned reserved_6                     ; //0x0F   4位
    unsigned crc_32                         ; //32 位}sw_ts_pmt_t;
}sw_ts_pmt_t;



#endif
