#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	char **pptr;
	char str[46];
	struct hostent *hptr;
	
	if(argc < 2)
	{
		fprintf(stderr, "usage: domain <domain>\n");
		return -1;
	}
	if((hptr = gethostbyname(argv[1])) == NULL)
	{
		fprintf(stderr, "gethostbyname call failed. %s\n",
		hstrerror(h_errno));
		return -1;
	}
	
	printf("official name: %s\n", hptr->h_name);
	for(pptr = hptr->h_aliases; *pptr != NULL; pptr ++)
	{
		printf("\t alias: %s\n", *pptr);
	}
	if(hptr->h_addrtype != AF_INET)
	{
		fprintf(stderr, "Invalid address type %d\n", hptr->h_addrtype);
		return -1;
	}
	pptr = hptr->h_addr_list;
	for(; *pptr != NULL; pptr ++)
	{
		printf("\t address: %s\n", inet_ntop(hptr->h_addrtype, *pptr,
		str, sizeof(str)));
	}
	return 0;
}