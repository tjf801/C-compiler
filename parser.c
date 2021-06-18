#include "lexer.h"
#include "nodes.h"
#include "parser.h"
#include <stdio.h>
#include <stdarg.h>

static void parser_error(char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	exit(2);
}

TokenVector *new_TokenVector(size_t initial_max_size) {
	TokenVector *T = malloc( sizeof(TokenVector) );
	T->data = malloc(initial_max_size*sizeof(Token*));
	T->length = 0;
	T->max_size = initial_max_size;
	return T;
}
void TokenVector_free(TokenVector *self) {
	free(self->data);
	self->data = NULL;
	self->length = self->max_size = 0;
}
void TokenVector_push(TokenVector *self, Token *token) {
	if (self->length == self->max_size) {
		self->max_size *= 2;
		self->data = realloc(self->data, self->max_size*sizeof(Token*));
	}
	self->data[self->length++] = token;
}
Token *TokenVector_get_item(TokenVector *self, int index) {
	
}

Parser* new_Parser(char *text) {
	Parser *P = (Parser*)malloc(sizeof(Parser));
	P->lexer = new_Lexer(text);
	P->position = 0;
	P->previous_tokens = new_TokenVector(16);
	P->next_token = NULL;
	return P;
}
Token *Parser_peek_next_token(Parser *self) {
	//looks at next token, and if there is none there, lex one
	if (self->next_token==NULL) self->next_token = Lexer_getNextToken(self->lexer);
	return self->next_token;
}
Token *Parser_pop_next_token(Parser *self) {
	if (self->next_token==NULL) self->next_token = Lexer_getNextToken(self->lexer);
	TokenVector_push(self->previous_tokens, self->next_token);
	self->next_token = NULL;
	return self->previous_tokens->data[self->previous_tokens->length-1];
}

NodeBase *Parser_parse_statement(Parser *self) {
	/*
	statement
		: labeled_statement
		| compound_statement
		| expression_statement
		| selection_statement
		| iteration_statement
		| jump_statement
		;
	*/
	Token *next_token = Parser_peek_next_token(self);
	if (next_token->token_base==KeywordTokenBase && ((KeywordToken*)next_token)->type==Return) { //TODO
		return Parser_parse_jump_statement(self);
	}
	else return Parser_parse_expression_statement(self); //TODO
}

NodeBase *Parser_parse_jump_statement(Parser *self) {
	/*
	jump_statement
		: GOTO IDENTIFIER ';'
		| CONTINUE ';'
		| BREAK ';'
		| RETURN ';'
		| RETURN expression ';'
		;
	*/
	Token *next_token = Parser_peek_next_token(self);
	if (next_token->token_base==KeywordTokenBase && ((KeywordToken*)next_token)->type==Return) {
		Parser_pop_next_token(self);
		NodeBase *expression;
		if (!(Parser_peek_next_token(self)->token_base==OperatorTokenBase&&((OperatorToken*)Parser_peek_next_token(self))->type==Semicolon)) {
			expression = Parser_parse_expression(self);
		}
		else expression = NULL;
		if (!(Parser_peek_next_token(self)->token_base==OperatorTokenBase&&((OperatorToken*)Parser_peek_next_token(self))->type==Semicolon)) {
			parser_error("expected semicolon after return statement");
		}
		Parser_pop_next_token(self);
		return (NodeBase*)new_ReturnStatementNode(expression);
	}
	else parser_error("TODO jump statements");
}

NodeBase *Parser_parse_expression_statement(Parser *self) {
	NodeBase *expression = Parser_parse_expression(self);
	Token *next_token = Parser_pop_next_token(self);
	if (!(next_token->token_base==OperatorTokenBase&&((OperatorToken*)next_token)->type==Semicolon)) {
		parser_error("expected a semicolon after expression statement");
	}
	return (NodeBase*)new_ExpressionStatementNode(expression);
}

NodeBase *Parser_parse_expression(Parser *self) {
	return Parser_parse_assignment_expression(self); //TODO
}
NodeBase *Parser_parse_assignment_expression(Parser *self) {
	return Parser_parse_conditional_expression(self); //TODO
}
NodeBase *Parser_parse_conditional_expression(Parser *self) {
	return Parser_parse_logical_or_expression(self); //TODO
}
NodeBase *Parser_parse_logical_or_expression(Parser *self) {
	return Parser_parse_logical_and_expression(self); //TODO
}
NodeBase *Parser_parse_logical_and_expression(Parser *self) {
	return Parser_parse_inclusive_or_expression(self); //TODO
}
NodeBase *Parser_parse_inclusive_or_expression(Parser *self) {
	return Parser_parse_exclusive_or_expression(self); //TODO
}
NodeBase *Parser_parse_exclusive_or_expression(Parser *self) {
	return Parser_parse_and_expression(self); //TODO
}
NodeBase *Parser_parse_and_expression(Parser *self) {
	return Parser_parse_equality_expression(self); //TODO
}
NodeBase *Parser_parse_equality_expression(Parser *self) {
	/*
	equality_expression
		: relational_expression
		| equality_expression EQ_OP relational_expression
		| equality_expression NE_OP relational_expression
		;
	*/
	NodeBase *relational_expression = Parser_parse_relational_expression(self);
	
	Token *next_token = Parser_peek_next_token(self);
	
	while (next_token->token_base==OperatorTokenBase&&(((OperatorToken*)next_token)->type==EqualTo||((OperatorToken*)next_token)->type==NotEqualTo)) {
		OperatorToken *token = (OperatorToken*)Parser_pop_next_token(self);
		NodeBase *next_relational_expression = Parser_parse_relational_expression(self);
		if (token->type==EqualTo) relational_expression = (NodeBase*)new_EqualityNode(relational_expression, next_relational_expression);
		else if (token->type==NotEqualTo) relational_expression = (NodeBase*)new_InequalityNode(relational_expression, next_relational_expression);
		next_token = Parser_peek_next_token(self);
	}
	
	return relational_expression;
}
NodeBase *Parser_parse_relational_expression(Parser *self) {
	/*
	relational_expression
		: shift_expression
		| relational_expression '<' shift_expression
		| relational_expression '>' shift_expression
		| relational_expression LE_OP shift_expression
		| relational_expression GE_OP shift_expression
		;
	*/
	NodeBase *shift_expression = Parser_parse_shift_expression(self);
	
	Token *next_token = Parser_peek_next_token(self);
	
	while (next_token->token_base==OperatorTokenBase&&(((OperatorToken*)next_token)->type==LessThan||((OperatorToken*)next_token)->type==GreaterThan||((OperatorToken*)next_token)->type==LessThanOrEqualTo||((OperatorToken*)next_token)->type==GreaterThanOrEqualTo)) {
		OperatorToken *token = (OperatorToken*)Parser_pop_next_token(self);
		NodeBase *next_shift_expression = Parser_parse_shift_expression(self);
		if (token->type==LessThan) shift_expression = (NodeBase*)new_LessThanNode(shift_expression, next_shift_expression);
		else if (token->type==GreaterThan) shift_expression = (NodeBase*)new_GreaterThanNode(shift_expression, next_shift_expression);
		else if (token->type==LessThanOrEqualTo) shift_expression = (NodeBase*)new_LessThanOrEqualToNode(shift_expression, next_shift_expression);
		else if (token->type==GreaterThanOrEqualTo) shift_expression = (NodeBase*)new_GreaterThanOrEqualToNode(shift_expression, next_shift_expression);
		next_token = Parser_peek_next_token(self);
	}
	
	return shift_expression;
}
NodeBase *Parser_parse_shift_expression(Parser *self) {
	return Parser_parse_additive_expression(self); //TODO
}
NodeBase *Parser_parse_additive_expression(Parser *self) {
	/*
	additive_expression
		: multiplicative_expression
		| additive_expression '+' multiplicative_expression
		| additive_expression '-' multiplicative_expression
		;
	*/
	NodeBase *multiplicative_expression = Parser_parse_multiplicative_expression(self);
	
	Token *next_token = Parser_peek_next_token(self);
	
	while (next_token->token_base==OperatorTokenBase&&(((OperatorToken*)next_token)->type==Plus||((OperatorToken*)next_token)->type==Minus)) {
		OperatorToken *token = (OperatorToken*)Parser_pop_next_token(self);
		NodeBase *next_multiplicative_expression = (NodeBase*)Parser_parse_multiplicative_expression(self);
		if (token->type==Plus) multiplicative_expression = (NodeBase*)new_AddNode(multiplicative_expression, next_multiplicative_expression);
		else if (token->type==Minus) multiplicative_expression = (NodeBase*)new_SubtractNode(multiplicative_expression, next_multiplicative_expression);
		next_token = Parser_peek_next_token(self);
	}
	
	return multiplicative_expression;
}
NodeBase *Parser_parse_multiplicative_expression(Parser *self) {
	/*	
	multiplicative_expression
		: cast_expression
		| multiplicative_expression '*' cast_expression
		| multiplicative_expression '/' cast_expression
		| multiplicative_expression '%' cast_expression
		;
	*/
	NodeBase *cast_expression = Parser_parse_cast_expression(self);
	
	Token *next_token = Parser_peek_next_token(self);
	
	while (next_token->token_base==OperatorTokenBase &&(((OperatorToken*)next_token)->type==Asterisk || ((OperatorToken*)next_token)->type==ForwardSlash||((OperatorToken*)next_token)->type==Percent)) {
		OperatorToken *token = (OperatorToken*)Parser_pop_next_token(self);
		NodeBase *next_cast_expression = Parser_parse_cast_expression(self);
		if (token->type==Asterisk) cast_expression = (NodeBase*)new_MultiplyNode(cast_expression, next_cast_expression);
		else if (token->type==ForwardSlash) cast_expression = (NodeBase*)new_DivideNode(cast_expression, next_cast_expression);
		else if (token->type==Percent) cast_expression = (NodeBase*)new_ModuloNode(cast_expression, next_cast_expression);
		next_token = Parser_peek_next_token(self);
	}
	
	return cast_expression;
}
NodeBase *Parser_parse_cast_expression(Parser *self) {
	return Parser_parse_unary_expression(self); //TODO
}
NodeBase *Parser_parse_unary_expression(Parser *self) {
	/*
	unary_expression
		: postfix_expression
		| INC_OP unary_expression
		| DEC_OP unary_expression
		| unary_operator cast_expression
		| SIZEOF unary_expression
		| SIZEOF '(' type_name ')'
		| ALIGNOF '(' type_name ')'
		;
	
	unary_operator
		: '&'
		| '*'
		| '+'
		| '-'
		| '~'
		| '!'
		;
	*/
	Token *next_token = Parser_peek_next_token(self);
	
	if (next_token->token_base==KeywordTokenBase&&((KeywordToken*)next_token)->type==Alignof) {
		
	}
	else if (next_token->token_base==KeywordTokenBase&&((KeywordToken*)next_token)->type==Sizeof) {
		
	}
	else if (next_token->token_base==OperatorTokenBase &&(((OperatorToken*)next_token)->type==Ampersand||((OperatorToken*)next_token)->type==Asterisk||((OperatorToken*)next_token)->type==Plus||((OperatorToken*)next_token)->type==Minus||((OperatorToken*)next_token)->type==Tilde||((OperatorToken*)next_token)->type==ExclamationPoint)) {
		OperatorToken *token = (OperatorToken*)Parser_pop_next_token(self);
		if (token->type==Minus) {
			return (NodeBase*)new_UnaryMinusNode(Parser_parse_cast_expression(self));
		} else if (token->type==Plus) {
			return (NodeBase*)new_UnaryPlusNode(Parser_parse_cast_expression(self));
		}
		else parser_error("unimplemented unary expression lol");
	}
	else if (next_token->token_base==OperatorTokenBase&&((OperatorToken*)next_token)->type==Decrement) {
		
	}
	else if (next_token->token_base==OperatorTokenBase&&((OperatorToken*)next_token)->type==Increment) {
		
	}
	else {
		return Parser_parse_postfix_expression(self); //TODO
	}
	parser_error("unimplemented unary expression lol (unary_expression)");
}
NodeBase *Parser_parse_postfix_expression(Parser *self) {
	return Parser_parse_primary_expression(self); //TODO
}
NodeBase *Parser_parse_primary_expression(Parser *self) {
	/*
	primary_expression
		: IDENTIFIER
		| constant
		| string
		| '(' expression ')'
		| generic_selection
		;
	*/
	
	Token *token = Parser_peek_next_token(self);
	
	if (token->token_base==ConstantTokenBase) {
		Parser_pop_next_token(self);
		ConstantToken *constant_token = (ConstantToken*)token;
		if (constant_token->type==IntegerTokenType) {
			return (NodeBase*)new_IntegerNode(constant_token->data.as_integer_constant);
		} else {
			parser_error("unsupported constant: TODO");
		}
	}
	else if (token->token_base==OperatorTokenBase) {
		OperatorToken *operator_token = (OperatorToken*)token;
		if (operator_token->type==OpenParenthesis) {
			Parser_pop_next_token(self);
			NodeBase *expression = Parser_parse_expression(self);
			Token *next_token = Parser_pop_next_token(self);
			if (next_token->token_base==OperatorTokenBase&&((OperatorToken*)next_token)->type==CloseParenthesis) {
				return expression;
			} else {
				parser_error("expected closing parenthesis");
			}
		}
	}
	else if (token->token_base==IdentifierTokenBase) {
		
	}
	else if (token->token_base==StringLiteralTokenBase) {
		
	}
	parser_error("unknown token (in parse_primary_expression)");
}

