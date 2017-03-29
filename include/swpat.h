#ifndef __SWPAT_H__
#define __SWPAT_H__

#include "swpat.h"

//TS���İ�ͷ��Ϣ
typedef struct __sw_TS_HEAD
{
	unsigned int sync_byte;//������е������ǲ������0x47��
	unsigned int transport_error_indicator;//��������־λ��һ�㴫�����Ļ��Ͳ��ᴦ���������
	unsigned int payload_unit_start_indicator;//Ϊ1ʱ����ǰ4���ֽ�֮�����һ�������ֽڣ�������ֵ�����˸������ݵľ��忪ʼλ�á�
	unsigned int transport_priority;//�������ȼ�λ��1��ʾ�����ȼ�
	unsigned int pid;//��Ч�������ݵ�����
	unsigned int transport_scrambling_control; //���ܱ�־λ,00��ʾδ����
	unsigned int adaptation_field_control; //�����ֶο���,01������Ч���أ�10���������ֶΣ�11���е����ֶκ���Ч���ء�
	unsigned int continuity_counte; //һ��4bit�ļ���������Χ0-15
}sw_ts_head_t;

//PAT��ṹ��
typedef struct __sw_TS_PAT
{
	unsigned int  table_id;                   //�̶�Ϊ0x00 ����־�Ǹñ���PAT��  
    unsigned int  section_syntax_indicator; //���﷨��־λ���̶�Ϊ1  
    unsigned int  zero;                       //0  
    unsigned int  reserved_1;                 // ����λ  
    unsigned int  section_length;            //��ʾ����һ���ֶο�ʼ��CRC32(��)֮�����õ��ֽ���  
    unsigned int  transport_stream_id;      //�ô�������ID��������һ��������������·���õ���  
    unsigned int  reserved_2;                // ����λ  
    unsigned int  version_number;            //��Χ0-31����ʾPAT�İ汾��  
    unsigned int  current_next_indicator;   //���͵�PAT�ǵ�ǰ��Ч������һ��PAT��Ч  
    unsigned int  section_number;            //�ֶεĺ��롣PAT���ܷ�Ϊ��δ��䣬��һ��Ϊ00���Ժ�ÿ���ֶμ�1����������256���ֶ�  
    unsigned int  last_section_number;      //���һ���ֶεĺ���  
   
    void     *program;  					   //ָ��sw_PAT_PROGRAM_t����
    unsigned int  reserved_3;                 // ����λ  
    unsigned int  network_pid;                //������Ϣ��NIT����PID,��Ŀ��Ϊ0ʱ��Ӧ��PIDΪnetwork_PID  
    unsigned int  crc_32;                     //CRC32У���� 
}sw_ts_pat_t;


#endif
