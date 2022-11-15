CFLAGS=   -Wfatal-errors -Wall
LDFLAGS=  -s
OBJS=     str.o tok.o vec.o lex.o pnode.o reg.o

all: p
%.o: %.c %.h
	$(CC) $< -c $(CFLAGS)
p: main.o $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)
clean:
	$(RM) *.o $(OBJS)
