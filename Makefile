CFLAGS=   -Wfatal-errors -Wall
LDFLAGS=  -s
OBJS=     p

all: $(OBJS)
%.o: %.c %.h
	$(CC) $< -c $(CFLAGS)
test: test.o lex.o vec.o str.o par.o ptree.o
	$(CC) $^ -o $@ $(LDFLAGS)
p: main.o lex.o vec.o str.o par.o ptree.o
	$(CC) $^ -o $@ $(LDFLAGS)
clean:
	$(RM) *.o $(OBJS)
