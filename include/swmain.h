#ifndef __SWHEAD_H__
#define __SWHEAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>

#include <pthread.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "kernel_list.h"
#include "swpat.h"
#include "swpmt.h"
#include "swsdt.h"


#define PACKET_SIZE_188     188
#define PACKET_SIZE_204     204 

typedef struct
{
	unsigned int stream_type       ; //ָʾ�ض�PID�Ľ�ĿԪ�ذ������͡��ô�PID��elementary PIDָ��  8λ
	unsigned int elementary_pid    ; //����ָʾTS����PIDֵ����ЩTS��������صĽ�ĿԪ��    13λ
	unsigned int es_info_length    ; //ǰ��λbitΪ00������ָʾ��������������ؽ�ĿԪ�ص�byte��  12λ
	unsigned int descriptor;
	struct list_head list;
}sw_pmt_stream_t; 

typedef struct
{
	unsigned int service_id;
	unsigned int etc_schedulu_flag;
	unsigned int etc_present_following_flag;
	unsigned int running_status;
	unsigned int free_ca_mode;
	unsigned int description_loop_length;
	unsigned char name[512];
}sw_sdt_info_t;


typedef struct  //
{
	unsigned int program_number;//��Ŀ��:16 !!ע��:��Ŀ��Ϊ0ʱΪ
	unsigned int reserver;							
	unsigned int program_map_pid;//��Ŀӳ����PID,ÿ����Ŀռһ��:13
	sw_sdt_info_t sdt_data;
	sw_pmt_stream_t *pmt_stream_head;
	struct list_head list;
}sw_pat_program_t;


typedef struct
{
	int pid;
	int type;
	struct list_head list;
}sw_check_pid_t;

#define PAT 0
#define PMT 1
#define SDT 2

#define NULL_PACKET 0x1FFF

//ע������һ��for(;;)�ṹ��ʹ��ʱ�ǵ����{}
#define sw_for_list_loop(pos,n,head) list_for_each_safe(pos, n, head)

#define sw_get_list_entry(ptr,type,member) list_entry(ptr, type, member)

#define sw_init_list_head(ptr) INIT_LIST_HEAD(ptr)

inline int sw_add_list_node(struct list_head *new,struct list_head *head);

inline int sw_del_list_node(struct list_head *entry);

//�����еõ�һ���̶����ȵİ�
int sw_get_one_packet(int file_fd,unsigned char *buf,int len);

//����SDT�������
int sw_parser_sdt_data(unsigned char *data,sw_ts_sdt_t *sdt_data,
									sw_pat_program_t *pat_program_head);
//��SDT����Ϣ�����Ľ���
void sw_sdt_service_descriptor(unsigned char *data);

//����TS�����ݰ��İ�ͷ��Ϣ
int sw_packet_parser_head(unsigned char *data,sw_ts_head_t *pat_head);

//����PAT������
int sw_packet_parser_pat_data(unsigned char *data,sw_ts_pat_t *pat_data,
				sw_pat_program_t *pat_program_head,sw_check_pid_t *check_pid_head);

//����PID�Լ������ͽ��ͱ���Ϣ
int sw_parser_pid(unsigned char *data,int pid,int type,
				sw_pat_program_t *pat_program_head,sw_check_pid_t *check_pid_head);

int sw_process(int file_fd,sw_pat_program_t *pat_program_head,sw_check_pid_t *check_pid_head);

//����PMT�����Ϣ
int sw_parser_pmt_data(unsigned char *data,int pid,sw_ts_pmt_t *pmt_data,
											sw_pat_program_t *pat_program_head);

int sw_init_list(sw_pat_program_t **pat_program_head,sw_check_pid_t  **check_pid_head);

//����program_number����pat_program_head�����еĽڵ�
sw_pat_program_t * sw_find_pat_node(unsigned int program_number,
											sw_pat_program_t *pat_program_head);

int sw_show_list(sw_pat_program_t *pat_program_head,sw_check_pid_t  *check_pid_head);

int sw_add_check_pid(int pid,int type,sw_check_pid_t  *check_pid_head);
sw_pmt_stream_t * sw_find_pmt_stream_node(unsigned int type,struct list_head *head);
int sw_free_list(sw_pat_program_t *pat_program_head,sw_check_pid_t  *check_pid_head);


#endif
