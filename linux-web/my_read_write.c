/*
	fd : 套接字
	
*/

int my_read(int fd,void *buff,size_t len)
{
	int cc,total=0;
	while(len>0)
	{
		if((cc = read(fd,(char*)buff,len))<0)
		{
			if(errno == EINTR) //阻塞时收到信号，-1并设置errno
			{
				continue;
			}
			return cc; //套接字出错，-1
		}
		if (cc == 0)   //套接字关闭,0
		{
			break;
		}
		buff=((char*)buff)+cc;
		total+=cc;
		len-=cc;
	}
	return total;
}

int my_write(int fd, const void *buff, size_t len)
{
	ssize_t cc;
	ssize_t total;
	total=0;
	while(len>0)
	{
		if((cc=write(fd,(const char*)buff,len))<0)
		{
			if (errno==EINTR) //阻塞时收到信号，-1并设置errno
			{
				continue;
			}
			return cc; //套接字出错或关闭，-1
		}
		total+=cc;
		buff=((const char*)buff)+cc;
		len=cc;
	}
	return total;
}