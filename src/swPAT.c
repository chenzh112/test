#include "swpat.h"
#include "swmain.h"
#include "swcrc.h"
#include "swlist.h"
#include "swpmt.h"
#include "swsdt.h"

int sw_packet_parser_pat_data(unsigned char *data,sw_ts_pat_t *pat_data,
						sw_pat_program_t *pat_program_head,sw_check_pid_t *check_pid_head)
{
	if(data == NULL || pat_data == NULL)
	{
		printf("param err!\n");
		return -1;
	}

	pat_data->table_id                    = data[0];  
    pat_data->section_syntax_indicator    = data[1] >> 7;  
    pat_data->zero                        = data[1] >> 6 & 0x1;  
    pat_data->reserved_1                  = data[1] >> 4 & 0x3;  
    pat_data->section_length              = (data[1] & 0x0F) << 8 | data[2];    
    pat_data->transport_stream_id         = data[3] << 8 | data[4];  
    pat_data->reserved_2                  = data[5] >> 6;  
    pat_data->version_number              = data[5] >> 1 &  0x1F;  
    pat_data->current_next_indicator      = data[5] & 0x1;  
    pat_data->section_number              = data[6];  
    pat_data->last_section_number         = data[7];   
  
    int len = 0;  
    len = 3 + pat_data->section_length;  
    pat_data->crc_32                       = (data[len-4] & 0x000000FF) << 24  
  											 |(data[len-3] & 0x000000FF) << 16  
  											 |(data[len-2] & 0x000000FF) << 8   
  											 |(data[len-1] & 0x000000FF);
	

	sw_pat_program_t *pat_program_tmp=NULL;

	if(pat_data->current_next_indicator ==0)
	{
		printf("currnt_next_indicator invalid!\n");
		return -1;
	 }
	else
	{
		int n=0;
		for(n=0;n<pat_data->section_length-12;n+=4)
		{
			pat_program_tmp=malloc(sizeof(sw_pat_program_t));
			if(pat_program_tmp == NULL)
			{
				printf("malloc err!\n");
				return -1;
			}
			memset(pat_program_tmp,0,sizeof(sw_pat_program_t));
			pat_program_tmp->program_number=(data[8 + n ]<<8)|data[9 + n ];
			pat_program_tmp->reserver=data[10+n]>>5;
			pat_program_tmp->program_map_pid=((data[10+n]&0x1F)<<8)|data[11 + n];
			pat_program_tmp->pmt_stream_head=NULL;
			sw_pat_program_t *tmp=sw_find_pat_node(pat_program_tmp->program_number,
																pat_program_head);
			if(tmp !=NULL || pat_program_tmp->program_number==0)
			{
				free(pat_program_tmp);
			}
			else
			{
				sw_add_check_pid(pat_program_tmp->program_map_pid,PMT,check_pid_head);
				pat_program_tmp->pmt_stream_head=malloc(sizeof(sw_pmt_stream_t));
				if(pat_program_tmp->pmt_stream_head==NULL)
				{
					printf("malloc err!\n");
					return -1;
				}
				memset(pat_program_tmp->pmt_stream_head,0,sizeof(sw_pmt_stream_t));
				sw_init_list_head(&(pat_program_tmp->pmt_stream_head->list));
				sw_add_list_node( &pat_program_tmp->list,&pat_program_head->list);
			}
			//Ìí¼ÓÖÁÁ´±íÎ²
		}
	}

    return 0; 
}

int sw_packet_parser_head(unsigned char *data,sw_ts_head_t *pat_head)
{
	if(data == NULL || pat_head== NULL )
	{
		printf("param err!\n");
		return -1;
	}
	memset(pat_head,0,sizeof(sw_ts_head_t));
	pat_head->sync_byte=data[0];
	pat_head->transport_error_indicator=data[1]>>7;
	pat_head->payload_unit_start_indicator=(data[1]>>6)&0x1;
	pat_head->transport_priority=(data[1]>>5)&0x1;
	pat_head->pid=((data[1]&0x1F)<<8)|data[2];
	pat_head->transport_scrambling_control=data[3]>>6;
	pat_head->adaptation_field_control=data[3]>>4 & 0x3;
	pat_head->continuity_counte=data[3]&0xf;

	return 0;
}

int sw_process(int file_fd,sw_pat_program_t *pat_program_head,sw_check_pid_t *check_pid_head)
{
	if(pat_program_head==NULL||check_pid_head==NULL||file_fd<0)
		return -1;
	unsigned char buf[256]={0};
	sw_ts_head_t pat_head={0};
	unsigned char *position=NULL;

	int ret=sw_get_one_packet(file_fd,buf,PACKET_SIZE_188);
	while(ret != -1 && ret !=0)
	{
		position = buf;
		memset(&pat_head,0,sizeof(pat_head));
		sw_packet_parser_head(buf,&pat_head);
		
		if(pat_head.transport_error_indicator == 1)  //´«Êä´íÎóµÄ°ü£¬¶ªÆú
		{
			ret=sw_get_one_packet(file_fd,buf,PACKET_SIZE_188);
			continue;
		}
		
		if(pat_head.pid == NULL_PACKET)  //¿Õ°ü£¬¶ªÆú
		{
			ret=sw_get_one_packet(file_fd,buf,PACKET_SIZE_188);
			continue;
		}

		if(pat_head.payload_unit_start_indicator == 0)
				position += 4;
		else if(pat_head.payload_unit_start_indicator == 1)
				position += 5;
			
		struct list_head *pos=NULL,*tmp=NULL;
		sw_for_list_loop(pos,tmp,&(check_pid_head->list))
		{
			sw_check_pid_t *p = sw_get_list_entry(pos,sw_check_pid_t,list);
			if(p==NULL)
				continue;
			if(p->pid == pat_head.pid)
				sw_parser_pid(position,p->pid,p->type,pat_program_head,check_pid_head);
		}
		ret=sw_get_one_packet(file_fd,buf,PACKET_SIZE_188);
	}
	return 0;
}

int sw_parser_pid(unsigned char *data,int pid,int type,
						sw_pat_program_t *pat_program_head,sw_check_pid_t *check_pid_head)
{
	if(data == NULL||pat_program_head==NULL||check_pid_head==NULL)
		return -1;
	
	sw_ts_pat_t pat_data={0};
	sw_ts_pmt_t pmt_data={0};
	sw_ts_sdt_t sdt_data={0};
	switch(type)
	{
		case PAT:
			memset(&pat_data,0,sizeof(pat_data));
			sw_packet_parser_pat_data(data,&pat_data,pat_program_head,check_pid_head);
			break;

		case PMT:
			memset(&pmt_data,0,sizeof(pmt_data));
			sw_parser_pmt_data(data,pid,&pmt_data,pat_program_head);
			break;

		case SDT:
			if( !list_empty(&pat_program_head->list) )
			{
				memset(&sdt_data,0,sizeof(sdt_data));
				sw_parser_sdt_data(data,&sdt_data,pat_program_head);
			}
			break;
		default:
			printf("type err!\n");
			printf("type:%d\n",type);
			break;
	}
	return 0;
}
