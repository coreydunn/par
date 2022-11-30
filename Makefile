CFLAGS=   -Wfatal-errors -Wall -Wextra
LDFLAGS=  -s
OBJS=     str.o tok.o vec.o lex.o pnode.o reg.o mem.o state.o err.o x86_64.o
PAR=      ./par

all: par
%.o: %.c %.h
	$(CC) $< -c $(CFLAGS)
par: main.o $(OBJS)
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
clean:
	$(RM) a.out *.o $(OBJS) par
