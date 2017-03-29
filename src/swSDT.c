#include "swsdt.h"
#include "swmain.h"


int sw_parser_sdt_data(unsigned char *data,sw_ts_sdt_t *sdt_data,
													sw_pat_program_t *pat_program_head)
{
	if(data ==NULL || sdt_data==NULL || pat_program_head== NULL)
		return -1;
	
	sdt_data->table_id=data[0];
	sdt_data->section_syntax_indicator=data[1]>>7;
	sdt_data->reserve_future_use_1=data[1]>>6 &0x1;
	sdt_data->reserve_1=data[1]>>4 &0x3;
	sdt_data->section_length=((data[1]&0xf)<<8)|data[2];
	sdt_data->transport_stream_id= data[3]<<8 | data[4];
	sdt_data->reserve_2=data[5]>>6;
	sdt_data->version_number=data[5]>>1&0x1f;
	sdt_data->current_next_indicator=data[5]&0x1;
	sdt_data->section_number=data[6];
	sdt_data->last_section_number=data[7];
	sdt_data->original_network_id= data[8]<<8 | data[9];
	sdt_data->reserve_future_use_2=data[10];
	sw_sdt_info_t tmp={0};
	int n = 0;
	if(sdt_data->table_id == 0x42 )//PID=0x11的有可能是BAT表，对应表ID不同
	{

		for(n=0;n<sdt_data->section_length-12;)
		{
			memset(&tmp,0,sizeof(tmp));
			tmp.service_id=data[11+n]<<8 | data[12+n];
			tmp.etc_schedulu_flag =data[13+n]>>1 & 0x1;
			tmp.etc_present_following_flag=data[13+n]&0x1;
			tmp.running_status=data[14+n]>>5;
			tmp.free_ca_mode=data[14+n]>>4&0x1;
			tmp.description_loop_length=(data[14+n] & 0xf)|data[15+n];
			memmove(tmp.name,&data[16+n],tmp.description_loop_length);
			n+=(5+tmp.description_loop_length);

			struct list_head *pos=NULL,*ttmp=NULL;
			sw_for_list_loop(pos,ttmp,&(pat_program_head->list))
			{
				sw_pat_program_t *p = sw_get_list_entry(pos,sw_pat_program_t,list);
				if(p != NULL)
				{
					if( p ->program_number==tmp.service_id)  //找到对应的program_ID
					{
						
						if(p->sdt_data.service_id==0)  //频道信息为空，说明还没有添加信息
						{
							memmove(p->sdt_data.name,tmp.name,tmp.description_loop_length);
							p->sdt_data.service_id=tmp.service_id;
							p->sdt_data.etc_schedulu_flag=tmp.etc_schedulu_flag;
							p->sdt_data.etc_present_following_flag=tmp.etc_present_following_flag;
							p->sdt_data.description_loop_length=tmp.description_loop_length;
							p->sdt_data.free_ca_mode=tmp.free_ca_mode;
							p->sdt_data.running_status=tmp.running_status;
							return 0;
						}
					}
				}
			}
			
		}
	}
	return 0;
}

void sw_sdt_service_descriptor(unsigned char *data)
{
	sw_sdt_descriptor_t tmp={0};
	tmp.descriptor_tag =data[0];
	tmp.descriptor_length=data[1];
	tmp.service_type=data[2];
	tmp.service_provider_name_length=data[3];

	switch(tmp.service_type)
	{
		case 0x1:
			printf("service_type:0x%x(Digital TV business)\n",tmp.service_type);
			break;
		case 0x10:
			printf("service_type:0x%x(DVB MHP business)\n",tmp.service_type);
			break;
	}

	memmove(tmp.provider_name,&data[4],tmp.service_provider_name_length);
	printf("provider_name:%s\n",&tmp.provider_name[1]);
	//因为第一个字节为字符编码格式
	tmp.service_name_length=data[4+tmp.service_provider_name_length];
	memmove(tmp.serverice_name,&data[4+tmp.service_provider_name_length+1],tmp.service_name_length);
	printf("server_name:%s\n",(char *)tmp.serverice_name+1);

	
}

