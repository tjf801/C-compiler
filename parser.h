#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>
#include "lexer.h"
#include "nodes.h"

typedef struct TokenVector {
	size_t length;
	size_t max_size;
	Token **data;
} TokenVector;
TokenVector *new_TokenVector(size_t initial_size);
void TokenVector_free(TokenVector *self);
void TokenVector_push_token(TokenVector *self, Token *token);
Token *TokenVector_get_item(TokenVector *self, int index);

typedef struct Parser {
	Lexer *lexer;
	int position;
	TokenVector *previous_tokens;
	Token *next_token;
} Parser;
Parser *new_Parser(char *text);
Token *Parser_peek_next_token(Parser *self);
Token *Parser_pop_next_token(Parser *self);

NodeBase *Parser_parse_block(Parser *self);
NodeBase *Parser_parse_block_item(Parser *self);
NodeBase *Parser_parse_statement(Parser *self);
NodeBase *Parser_parse_expression_statement(Parser *self);
NodeBase *Parser_parse_jump_statement(Parser *self);

NodeBase *Parser_parse_expression(Parser *self);
NodeBase *Parser_parse_assignment_expression(Parser *self);
NodeBase *Parser_parse_conditional_expression(Parser *self);
NodeBase *Parser_parse_logical_or_expression(Parser *self);
NodeBase *Parser_parse_logical_and_expression(Parser *self);
NodeBase *Parser_parse_inclusive_or_expression(Parser *self);
NodeBase *Parser_parse_exclusive_or_expression(Parser *self);
NodeBase *Parser_parse_and_expression(Parser *self);
NodeBase *Parser_parse_equality_expression(Parser *self);
NodeBase *Parser_parse_relational_expression(Parser *self);
NodeBase *Parser_parse_shift_expression(Parser *self);
NodeBase *Parser_parse_additive_expression(Parser *self);
NodeBase *Parser_parse_multiplicative_expression(Parser *self);
NodeBase *Parser_parse_cast_expression(Parser *self);
NodeBase *Parser_parse_unary_expression(Parser *self);
NodeBase *Parser_parse_postfix_expression(Parser *self);
NodeBase *Parser_parse_primary_expression(Parser *self);

#endif