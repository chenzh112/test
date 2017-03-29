#ifndef __SWPMT_H__
#define __SWPMT_H__

//PMT ��ṹ��
typedef struct __sw_TS_PMT
{
    unsigned table_id                       ; //�̶�Ϊ0x02, ��ʾPMT��8λ
    unsigned section_syntax_indicator    ; //�̶�Ϊ0x01   1λ
    unsigned zero							 ; //0x01   1λ      
    unsigned reserved_1                     ; //0x03   2λ
    unsigned section_length                 ;//������λbit��Ϊ00����ָʾ�ε�byte�����ɶγ�����ʼ������CRC��12λ
    unsigned program_number                ;// ָ���ý�Ŀ��Ӧ�ڿ�Ӧ�õ�Program map PID   16λ
    unsigned reserved_2                     ; //0x03  2λ
    unsigned version_number                 ; //ָ��TS����Program map section�İ汾�� 5λ
    unsigned current_next_indicator       ; //����λ��1ʱ����ǰ���͵�Program map section���ã�1λ
      //����λ��0ʱ��ָʾ��ǰ���͵�Program map section�����ã���һ��TS����Program map section��Ч��
    unsigned section_number                ; //�̶�Ϊ0x00  8λ
    unsigned last_section_number           ; //�̶�Ϊ0x00  8λ
    unsigned reserved_3                     ; //0x07     3λ
    unsigned pcr_pid                       ; //ָ��TS����PIDֵ����TS������PCR�� 13λ
            //��PCRֵ��Ӧ���ɽ�Ŀ��ָ���Ķ�Ӧ��Ŀ��
            //�������˽���������Ľ�Ŀ������PCR�޹أ�������ֵ��Ϊ0x1FFF��
    unsigned reserved_4                     ; //Ԥ��Ϊ0x0F  4λ
    unsigned program_info_length           ; //ǰ��λbitΪ00������ָ���������Խ�Ŀ��Ϣ��������byte����12λ
    
    unsigned reserved_5                     ; //0x07   3λ
    unsigned reserved_6                     ; //0x0F   4λ
    unsigned crc_32                         ; //32 λ}sw_ts_pmt_t;
}sw_ts_pmt_t;



#endif
