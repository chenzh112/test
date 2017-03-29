#ifndef __SWSDT_H__
#define __SWSDT_H__

typedef struct
{
	unsigned int table_id;//������0x42,��ʾ�������ǵ�ǰ������Ϣ,Ҳ������0x46,��ʾ������������Ϣ(EPGʹ�ô˲���)
	unsigned int section_syntax_indicator;
	unsigned int reserve_future_use_1;
	unsigned int reserve_1;
	unsigned int section_length;
	unsigned int transport_stream_id;
	unsigned int reserve_2;
	unsigned int version_number;
	unsigned int current_next_indicator;
	unsigned int section_number;
	unsigned int last_section_number;
	unsigned int original_network_id;
	unsigned int reserve_future_use_2;
	unsigned int crc32;
}sw_ts_sdt_t;

typedef struct
{
	unsigned int descriptor_tag;
	unsigned int descriptor_length;
	unsigned int service_type;
	unsigned int service_provider_name_length;
	unsigned char provider_name[256];
	unsigned int service_name_length;
	unsigned char serverice_name[256];
}sw_sdt_descriptor_t;



#endif
