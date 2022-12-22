CFLAGS=   -Wfatal-errors -Wall -Wextra
LDFLAGS=  -s
OBJS=     str.o tok.o vec.o lex.o pnode.o reg.o mem.o state.o err.o x86_64.o rpn.o
PAR=      ./par
LIB=      libpar.a
TESTOBJS= $(basename $(shell ls tests/*.par))

all: par
%.o: %.c %.h
	$(CC) $< -c $(CFLAGS)
libpar.a: $(OBJS)
	ar rc $@ $^
par: main.o libpar.a
	$(CC) $^ -o $@ $(LDFLAGS)
%.asm: %.par par
	./par $<
%.o: %.asm par
	nasm -felf64 $<
%: %.o par
	cc $*.o -o $@ -no-pie
install_vimfiles:
	mkdir -p ~/.vim/ftdetect
	mkdir -p ~/.vim/syntax
	cp ./vimfiles/syntax/par.vim ~/.vim/syntax/
	cp ./vimfiles/ftdetect/par.vim ~/.vim/ftdetect/
tests: $(TESTOBJS)
clean:
	$(RM) a.out *.o $(OBJS) par libpar.a $(TESTOBJS)
