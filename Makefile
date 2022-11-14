CFLAGS=   -Wfatal-errors -Wall
LDFLAGS=  -s
OBJS=     p

all: $(OBJS) c
%.o: %.c %.h
	$(CC) $< -c $(CFLAGS)
test: test.o lex.o vec.o str.o tok.o
	$(CC) $^ -o $@ $(LDFLAGS)
p: main.o lex.o vec.o str.o tok.o node.o
	$(CC) $^ -o $@ $(LDFLAGS)
clean:
	$(RM) *.o $(OBJS)
