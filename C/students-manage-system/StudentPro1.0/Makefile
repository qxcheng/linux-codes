
ALL:=test
OBJECT:=main.o menu.o student.o teacher.o save.o admin.o
OPTION:=-g -Wall -lm
CC:=gcc
$(ALL):$(OBJECT)
	$(CC) $^ -o $@ $(OPTION)
%.o:%.c
	$(CC) -c $< -o $@
clean:
	sudo rm -rf $(OBJECT)
