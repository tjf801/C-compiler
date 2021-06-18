CC = gcc
CFLAGS = -std=c11 -g -fno-common
source_files = $(wildcard *.c)
header_files = $(wildcard *.h)
object_files = $(source_files:.c=.o)

ccompiler: $(object_files)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.c $(header_files)
	$(CC) $(CFLAGS) -c -o main.o main.c
lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c -o lexer.o lexer.c
nodes.o: nodes.c nodes.h
	$(CC) $(CFLAGS) -c -o nodes.o nodes.c
parser.o: parser.c parser.h nodes.h lexer.h
	$(CC) $(CFLAGS) -c -o parser.o parser.c
codegen.o: codegen.c codegen.h parser.h nodes.c lexer.h
	$(CC) $(CFLAGS) -c -o codegen.o codegen.c

clean: $(object_files)
	$(CC) $(CFLAGS) -o ccompiler $(object_files)
	del $(object_files)

test: ccompiler
	"C:\Program Files\Git\bin\bash.exe" test.sh
	del tmp.exe
	del tmp.s

.PHONY: test clean