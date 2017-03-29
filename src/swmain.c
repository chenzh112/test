#include "swmain.h"
#include "swpat.h"
#include "swlist.h"
#include "swpmt.h"
#include "swsdt.h"


int sw_get_one_packet(int file_fd,unsigned char *buf,int len)
{
	if( file_fd < 0 || buf == NULL )
	{
		printf("param err!\n");
		return -1;
	}
	static int m_read_offest=0;

	int read_len=1;
	while(read_len)
	{
		read_len = pread(file_fd,buf,1,m_read_offest);
		if( buf[0]==0x47 )
		{
			memset(buf,0,len);
			read_len= pread(file_fd,buf,len,m_read_offest);
			char buf_test[2]={0};
			m_read_offest+=read_len;
			pread(file_fd,buf_test,1,m_read_offest);
			if(buf_test[0]==0x47)
			{
				break;
			}
			else
			{
				m_read_offest-=read_len;
				m_read_offest+=1;
			}
		}
		else
		{
			m_read_offest ++;
		}
	}
	if(read_len == 0)
	{
		printf("search end!\n");
		return 0;
	}

	return read_len;

}

int main(int argc,char *argv[])
{
	if( argc!=2 )
	{
		printf("usage:%s file_name!\n",argv[0]);
		return 0;
	}
	sw_pat_program_t *pat_program_head=NULL;
	sw_check_pid_t  *check_pid_head=NULL;
	int file_fd=open(argv[1],O_RDONLY,0);
	if(file_fd == -1)
	{
		perror("open");
		return 0;
	}
	sw_init_list(&pat_program_head,&check_pid_head);
	sw_add_check_pid(0x0,PAT,check_pid_head);
	sw_add_check_pid(0x11,SDT,check_pid_head);


	sw_process(file_fd,pat_program_head,check_pid_head);
	sw_show_list(pat_program_head,check_pid_head);
	sw_free_list(pat_program_head,check_pid_head);
		//sw_show_stream_info();

	close(file_fd);
	return 0;
}
