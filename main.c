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

static void test_lexer(char *text) {
	Lexer *lexer = new_Lexer(text);
	Token *token;
	while ((token=Lexer_getNextToken(lexer))->token_base!=EOFTokenBase) {
		char tokentext[64];
		strncpy(tokentext, token->location, token->length);
		tokentext[token->length] = '\0';
		printf("%d '%s' ", token->length, tokentext);
		if (token->token_base==KeywordTokenBase) {
			printf("kwd\n");
		} else if (token->token_base==IdentifierTokenBase) {
			printf("id\n");
		} else if (token->token_base==ConstantTokenBase) {
			printf("cst %d\n", ((ConstantToken*)token)->data.as_integer_constant);
		} else if (token->token_base==OperatorTokenBase) {
			printf("op\n");
		} else if (token->token_base==StringLiteralTokenBase) {
			printf("str\n");
		}
	}
	
}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
		return 1;
	}
	
	// first argument
	char *p = argv[1];
	
	/*
	// movs the first int into the rax register
	printf("  mov $%ld, %%rax\n", strtol(p, &p, 10));
	
	while (*p) {
		if (*p == '+') {
			++p;
			printf("  add $%ld, %%rax\n", strtol(p, &p, 10));
			continue;
		}
		if (*p == '-') {
			++p;
			printf("  sub $%ld %%rax\n", strtol(p, &p, 10));
			continue;
		}
		
		fprintf(stderr, "unexpected character '%c'\n", *p);
		return 1;
	}
	
	printf("  ret\n");*/
	
	Parser *parser = new_Parser(p);
	NodeBase *tree = Parser_parse_expression(parser);
	codegen(tree);
	
	return 0;
}