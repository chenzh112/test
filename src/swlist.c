#include "swlist.h"
#include "swpat.h"
#include "swmain.h"


int sw_init_list(sw_pat_program_t **pat_program_head,sw_check_pid_t  **check_pid_head)
{
	*pat_program_head=malloc(sizeof(sw_pat_program_t));
	if( pat_program_head != NULL )
	{
		memset(*pat_program_head,0,sizeof(sw_pat_program_t));
		sw_init_list_head(&((*pat_program_head)->list));
	}
	else
	{
		printf("calloc pat_program_head err!\n");
		return -1;
	}

	*check_pid_head=malloc(sizeof(sw_check_pid_t));
	if( check_pid_head != NULL )
	{
		memset(*check_pid_head,0,sizeof(sw_check_pid_t));
		sw_init_list_head(&((*check_pid_head)->list));
	}
	else
	{
		printf("calloc check_pid_head err!\n");
		return -1;
	}
	return 0;
}



int sw_free_list(sw_pat_program_t *pat_program_head,sw_check_pid_t  *check_pid_head)
{
	if(pat_program_head==NULL || check_pid_head==NULL)
		return -1;

	struct list_head *pos=NULL,*n=NULL;

	sw_for_list_loop(pos,n,&(pat_program_head-> list))
	{
		sw_pat_program_t *p=sw_get_list_entry(pos,sw_pat_program_t,list);
		if(p==NULL)
			continue;
		struct list_head *ppos=NULL,*nn=NULL;
		sw_for_list_loop(ppos,nn,&(p->pmt_stream_head->list))
		{
			sw_pmt_stream_t *pp =sw_get_list_entry(ppos,sw_pmt_stream_t,list);
			if(pp==NULL)
				continue;
			sw_del_list_node(&(pp->list ));
			free(pp);
		}
		sw_del_list_node(&(p ->list ));
		free(p);
	}

	sw_for_list_loop(pos,n,&check_pid_head->list)
	{
		sw_check_pid_t *pp = sw_get_list_entry(pos,sw_check_pid_t,list);
		if(pp==NULL)
			continue;
		sw_del_list_node(&(pp->list ));
		free(pp);
	}

	free(check_pid_head);
	free(pat_program_head);
	return 0;
}

int sw_add_check_pid(int pid,int type,sw_check_pid_t  *check_pid_head)
{
	if(check_pid_head == NULL)
		return -1;
	sw_check_pid_t *node=malloc(sizeof(sw_check_pid_t));
	if(node ==NULL)
	{
		printf("malloc err!%s\n",__func__);
		return -1;
	}
	memset(node,0,sizeof(sw_check_pid_t));
	node->pid=pid;
	node->type=type;
	sw_add_list_node(&(node->list),&(check_pid_head->list));  //添加至链表尾
	return 0;
}

int sw_show_list(sw_pat_program_t *pat_program_head,sw_check_pid_t  *check_pid_head)
{
	if(pat_program_head==NULL || check_pid_head==NULL)
		return -1;	
	struct list_head *pos,*tmp;
	int program_number=0;
	sw_for_list_loop(pos,tmp,&(pat_program_head->list))
	{
		program_number++;
			printf("No.%d:\n",program_number);
		sw_pat_program_t *p = sw_get_list_entry(pos,sw_pat_program_t,list);
		if(p==NULL)
			continue;
		printf("program_number:0x%x\n",p->program_number);
		printf("program_map_pid:0x%x\n",p->program_map_pid);
		sw_sdt_service_descriptor(p->sdt_data.name);

		switch(p->sdt_data.running_status)
		{
			case 2:
				printf("running_status:after seconds,start!\n");
				break;
			case 3:
				printf("running_status:pause!\n");
				break;
			case 4:
				printf("running_status:playing!\n");
				break;
			default:
				printf("running_status:undefine!\n");
				break;
		}

		switch(p->sdt_data.free_ca_mode)
		{
			case 0:
				printf("free_ca_mode:0x%x(unencrypted)\n",p->sdt_data.free_ca_mode);
				break;
			case 1:
				printf("free_ca_mode:0x%x(encrypted)\n",p->sdt_data.free_ca_mode);
				break;
			default:
				printf("free_ca_mode:0x%x  err\n",p->sdt_data.free_ca_mode);
				break;
			
		}
		struct list_head *ppos=NULL,*ttmp=NULL;
		sw_for_list_loop(ppos,ttmp,&(p->pmt_stream_head->list))
		{
			sw_pmt_stream_t *pp = list_entry(ppos,sw_pmt_stream_t,list);
			if(pp==NULL)
				continue;
			switch(pp->stream_type)
			{
				case 0x02:
					printf("stream_type:0x%x\tVideo(mpeg video v2) or video stream PID:0x%x\n",
										pp->stream_type,pp->elementary_pid);
					break;
				case 0x04:
					printf("stream_type:0x%x\tAudio (BC Audio Codec) PID:0x%x\n",
										pp->stream_type,pp->elementary_pid);
					break;
				case 0x0b:
					printf("stream_type:0x%x\ttype B PID:0x%x\n",
										pp->stream_type,pp->elementary_pid);
					break;
				case 0x05:
					printf("stream_type:0x%x\tprivate_sections PID:0x%x\n",
										pp->stream_type,pp->elementary_pid);
					break;
				case 0x06:
					printf("stream_type:0x%x\tPES packets containing private data PID:0x%x\n",
										pp->stream_type,pp->elementary_pid);
					break;
				default:
					printf("stream_type:%x\n",pp->stream_type);
					printf("uncontain!\n");
					break;
			}
		}
		
		printf("___________________________________________\n");
	}
	return 0;
}

/*
 *brief:根据program_number找到链表pat_program_head中的节点
 *
 *return 成功找到返回该节点地址，失败返回NULL
 */
sw_pat_program_t * sw_find_pat_node(unsigned int program_number,
											sw_pat_program_t *pat_program_head)
{
	struct list_head *pos=NULL,*n=NULL;
	sw_for_list_loop(pos,n,&(pat_program_head->list))
	{
		 sw_pat_program_t *p= list_entry(pos,sw_pat_program_t,list);
		if ( p->program_number == program_number)
		{
			return p;
		}
	}

	return NULL;
}


/*
 *brief:根据type找到链表head中的节点
 *
 *return 成功找到返回该节点地址，失败返回NULL
 */
sw_pmt_stream_t * sw_find_pmt_stream_node(unsigned int type,struct list_head *head)
{
	struct list_head *pos,*n;
	sw_for_list_loop(pos,n,head)
	{
		sw_pmt_stream_t *p= sw_get_list_entry(pos,sw_pmt_stream_t,list);
		if ( p->stream_type== type)
		{
			return p;
		}
	}

	return NULL;
}

inline int sw_add_list_node(struct list_head *new,struct list_head *head)
{
	if(new == NULL || head ==NULL)
		return -1;
	list_add_tail(new,head);
	return 0;
}

inline int sw_del_list_node(struct list_head *entry)
{
	if(entry == NULL)
		return -1;
	list_del(entry);
	return 0;
}
