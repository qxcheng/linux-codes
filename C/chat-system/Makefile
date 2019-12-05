SERVER:=s
OBJECT_S:=server.o list.o mysql.o protocol.o

CLIENT:=c
OBJECT_C:=client.o protocol.o

CC:=gcc
OPTION:=-g -Wall -lm -lmysqlclient -lpthread

$(SERVER):$(OBJECT_S)
	$(CC) $^ -o $@ $(OPTION)

$(CLIENT):$(OBJECT_C)
	$(CC) $^ -o $@ $(OPTION)

%.o:%.c
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJECT_S) $(OBJECT_C)
