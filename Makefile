CFLAGS=   -Wfatal-errors -Wall -Wextra
LDFLAGS=  -s
OBJS=     str.o tok.o vec.o lex.o pnode.o reg.o var.o state.o err.o x86_64.o

all: par
%.o: %.c %.h
	$(CC) $< -c $(CFLAGS)
par: main.o $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)
%: %.bob par
	./par $^
	nasm -felf64 a.out -o $*.o
	cc $*.o -o $@ -no-pie
install_vimfiles:
	mkdir -p ~/.vim/ftdetect
	mkdir -p ~/.vim/syntax
	cp ./vimfiles/syntax/bob.vim ~/.vim/syntax/
	cp ./vimfiles/ftdetect/bob.vim ~/.vim/ftdetect/
clean:
	$(RM) a.out *.o $(OBJS) par
