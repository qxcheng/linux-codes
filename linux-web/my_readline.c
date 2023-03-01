
//每次读入一个字节， 并检查该字节是否为换行字符， 
//它确保读取一整行内容。

static int my_readline(int fd, void *buf, unsigned len)
{
	int n, rc;
	char c, *ptr;
	ptr = buf;
	for (n = 1; n < len; n++) {
again:
		if ( (rc = read(fd, &c, 1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			if (n == 1)
				return(0);
			else
				break;
		} else {
			if (errno == EINTR)
				goto again;
			return(-1);
		}
	}
	*ptr = 0;
	return(n);
}