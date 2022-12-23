CFLAGS=   -Wfatal-errors -Wall -Wextra
LDFLAGS=  -s
OBJS=     str.o tok.o vec.o lex.o pnode.o reg.o mem.o state.o err.o x86_64.o rpn.o
LIB=      libpar.a
TESTS=    $(basename $(shell ls tests/test_*.par))

.PHONY: FORCE
all: par
help:
	@printf "Targets\n"
	@printf "  all                Build par (default)\n"
	@printf "  clean              Remove build files\n"
	@printf "  tests              Build and run tests\n"
	@printf "  install_vimfiles   Install syntax files in ~/.vim/\n"
%.o: %.c %.h
	$(CC) $< -c $(CFLAGS)
libpar.a: $(OBJS)
	ar rc $@ $^
par: main.o libpar.a
	$(CC) $^ -o $@ $(LDFLAGS)
%.asm: %.par
	./par $<
%.o: %.asm
	nasm -felf64 $<
%: %.o
	cc $*.o -o $@ -no-pie
install_vimfiles:
	mkdir -p ~/.vim/ftdetect
	mkdir -p ~/.vim/syntax
	cp ./vimfiles/syntax/par.vim ~/.vim/syntax/
	cp ./vimfiles/ftdetect/par.vim ~/.vim/ftdetect/
tests: $(TESTS) FORCE
	./tests/run_tests.sh tests
clean:
	$(RM) a.out *.o $(OBJS) par libpar.a $(TESTS)
