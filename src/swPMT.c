#include "swpmt.h"
#include "swcrc.h"
#include "swlist.h"
#include "swpat.h"
#include "swmain.h"

int sw_parser_pmt_data(unsigned char *data,int pid,sw_ts_pmt_t *pmt_data,
														sw_pat_program_t *pat_program_head)
{
	if(data == NULL || pmt_data== NULL)
		return -1;
	
	memset(pmt_data,0,sizeof(sw_ts_pmt_t));
	pmt_data->table_id                   = data[0];
    pmt_data->section_syntax_indicator   = data[1] >> 7;
    pmt_data->zero                       = data[1] >> 6 & 0x01; 
    pmt_data->reserved_1                 = data[1] >> 4 & 0x03;
    pmt_data->section_length             = (data[1] & 0x0F) << 8 | data[2];    
    pmt_data->program_number             = (data[3] << 8 )| data[4];
    pmt_data->reserved_2                 = data[5] >> 6;
    pmt_data->version_number             = (data[5] >> 1) & 0x1F;
    pmt_data->current_next_indicator     = data[5] &0x1;
    pmt_data->section_number             = data[6];
    pmt_data->last_section_number        = data[7];
    pmt_data->reserved_3                 = data[8] >> 5;
    pmt_data->pcr_pid					  = ((data[8] << 8) | data[9]) & 0x1FFF;
    pmt_data->reserved_4                 = data[10] >> 4;
    pmt_data->program_info_length        = ((data[10] & 0x0F)<<8) | data[11]; 
 	int len = 0;
    len = pmt_data->section_length + 3;    
    pmt_data->crc_32                  	  = (data[len-4] & 0x000000FF) << 24
 										   |(data[len-3] & 0x000000FF) << 16
  										   |(data[len-2] & 0x000000FF) << 8
  										   |(data[len-1] & 0x000000FF); 
	if(pmt_data->table_id != 0x02)
	{
		return -1;
	}
 	int pos = 12;
    // program info descriptor
    if ( pmt_data->program_info_length != 0 )
        pos += pmt_data->program_info_length;    
    // Get stream type and PID    
    for ( ; pos <= (pmt_data->section_length + 2 ) -  4; )
    {
  		sw_pmt_stream_t *pmt_stream=malloc(sizeof(sw_pmt_stream_t));
		memset(pmt_stream,0,sizeof(sw_pmt_stream_t));
		if(pmt_stream == NULL)
			return -1;
  		pmt_stream->stream_type =data[pos];
  		pmt_data->reserved_5=data[pos+1] >> 5;
  		pmt_stream->elementary_pid =((data[pos+1] << 8) | data[pos+2]) & 0x1FFF;
  		pmt_data->reserved_6=data[pos+3] >> 4;
  		pmt_stream->es_info_length =(data[pos+3] & 0x0F) << 8 | data[pos+4];
  
  		pmt_stream->descriptor = 0x00;
  		if(pmt_stream->es_info_length != 0)
  		{
   			pmt_stream->descriptor = data[pos + 5];
			int len;
   			for(len = 2; len <= pmt_stream->es_info_length; len ++ )
   			{
   				 pmt_stream->descriptor = pmt_stream->descriptor<<8|data[pos + 4 + len];
   			}
  	 		pos += pmt_stream->es_info_length;
 		 }
  		pos += 5;
		
		//先根据program_number查找pat_program_head链表中的节点
		//如果有则根据stream_type查找pat_program_head成员pmt_stream_head的节点
		//如果没有则添加进去
		sw_pat_program_t *tmp=sw_find_pat_node(pmt_data->program_number,pat_program_head);
		if( tmp != NULL)
		{
			sw_pmt_stream_t *ttmp=sw_find_pmt_stream_node(pmt_stream->stream_type,
												&(tmp->pmt_stream_head->list));
			if(ttmp == NULL)
			{
				sw_add_list_node(&(pmt_stream->list),&(tmp->pmt_stream_head->list));
			}
			else
				free(pmt_stream);
		}

		else
		{
			free(pmt_stream);
			printf("not find program_number\n");
		}

    }
	return 0;
}
