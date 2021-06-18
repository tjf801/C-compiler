#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "nodes.h"
#include "parser.h"
#include "codegen.h"

void error(char *format, ...);
void error_at(char *location, char* format, ...);
void error_token(Token* token, char *format, ...);

//reports an error and exits
void error(char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	exit(1);
}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
		return 1;
	}
	
	// first argument
	char *p = argv[1];
	
	Parser *parser = new_Parser(p);
	NodeBase *tree = Parser_parse_statement(parser);
	codegen(tree);
	
	return 0;
}